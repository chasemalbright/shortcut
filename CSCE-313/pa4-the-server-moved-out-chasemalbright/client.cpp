// your PA3 client code here
#include <fstream>
#include <iostream>
#include <thread>
#include <sys/time.h>
#include <sys/wait.h>

#include "BoundedBuffer.h"
#include "common.h"
#include "Histogram.h"
#include "HistogramCollection.h"
#include "TCPRequestChannel.h"

// ecgno to use for datamsgs
#define EGCNO 1
#define ZERO 0.0

using namespace std;

struct PatientData
{
    int pno;
    double response;
};

// TODO
void patient_thread_function(BoundedBuffer *buffer, int threadCount, int personNumber)
{ // 1
    // functionality of the patient threads
    datamsg msg(personNumber, ZERO, EGCNO); // new data message deafult with 0.0 and 1

    for (int i = 0; i < threadCount; i++)
    {
        buffer->push((char *)&msg, sizeof(msg)); // push to bb
        msg.seconds = msg.seconds + 0.004;       // increment time
    }
}
/* FROM PA1*/
// void writePatientData(FIFORequestChannel *channel, int p){
// 	ofstream fout;
// 	fout.open("received/x1.csv");
// 	double ecg1;
// 	double ecg2;
// 	double t;

// 	for (int i = 0; i < 1000; i++){
// 		t = (0.004 * i);
// 		if (t > 59.996){
// 			break;
// 		}
// 		ecg1 = writePatientPoint(channel,p,t,1);
// 		ecg2 = writePatientPoint(channel,p,t,2);
// 		fout << t << "," << ecg1 << "," << ecg2 << std::endl;
// 	}
// 	fout.close();
// }

// TODO void worker_thread_function (BoundedBuffer * response_buffer,BoundedBuffer * request_buffer, FIFORequestChannel * channel, int size)
void worker_thread_function(BoundedBuffer *response_buffer, BoundedBuffer *request_buffer, TCPRequestChannel *channel, int size)
{ // 2
    // functionality of the worker threads
    MESSAGE_TYPE *msg;
    double response = 0;

    // buffers
    char bufferMessage[2048];
    char receiveBuffer[size];

    while (1)
    {

        request_buffer->pop(bufferMessage, sizeof(bufferMessage)); // pop and write mesage from pop to bM

        msg = (MESSAGE_TYPE *)bufferMessage; // cast msg type to bM

        if (*msg == FILE_MSG)
        { // int 2
            // handle file message

            filemsg *filemessage = (filemsg *)bufferMessage; // cast to fm
            // create dir + file
            string filename = (char *)(filemessage + 1); // +1 for \n

            int filesize = filename.size() + sizeof(filemsg) + 1; // size of total

            filename = "received/" + filename; // put into recieved directory like in PA1

            // get data to and from channels
            channel->cwrite(bufferMessage, filesize);
            channel->cread(receiveBuffer, size);

            // need to open file
            FILE *outfile = fopen(filename.c_str(), "r+");

            fseek(outfile, filemessage->offset, SEEK_SET);

            fwrite(receiveBuffer, 1, filemessage->length, outfile); // write to file

            fclose(outfile);
        }
        else if (*msg == DATA_MSG)
        { // int 1
            // handle a data message
            channel->cwrite(bufferMessage, sizeof(datamsg));
            channel->cread(&response, sizeof(double));
            PatientData pd = {((datamsg *)bufferMessage)->person, response};

            response_buffer->push((char *)&pd, sizeof(pd)); // push to resp
        }
        else if (*msg == QUIT_MSG)
        { // int 4
            // handle quit message
            channel->cwrite(msg, sizeof(QUIT_MSG));
            break;
        }
    }
}

// TODO
void histogram_thread_function(HistogramCollection *hc, BoundedBuffer *reponse_buffer)
{ // 3
    // functionality of the histogram threads

    while (1)
    {
        char buf[sizeof(PatientData)];
        reponse_buffer->pop(buf, sizeof(PatientData)); // patient data

        PatientData *pd = (PatientData *)buf;
        if ((pd->pno == -1) && (pd->response == -1.0))
        {
            break;
        }
        hc->update(pd->pno, pd->response);
    }
}

// TODO

void file_thread_function(int filesize, BoundedBuffer *request_buffer, string fname, int m) // last thing to do!!!
{

    int len = sizeof(filemsg) + fname.size();
    len += 1;

    int offset = 0; // capture remaining offset

    int num = floor(filesize / m); // handle first messages and then on extra one for leftover

    for (int i = 0; i < num; i++)
    {
        filemsg temp(offset, m); // accumulated changes to use the correct offset // push to request buffer
        char tempBuf[len];       // temp mem

        memcpy(tempBuf, &temp, sizeof(filemsg));
        strcpy(tempBuf + sizeof(filemsg), fname.c_str());

        request_buffer->push(tempBuf, len);
        offset += m; // keep track of offset
    }
    // take care of left over bit
    if ((filesize % m) != 0) // minh says that you need to handle the extra piece like in PA1
    {
        int remainder = filesize - offset; // calc offset
        filemsg last(offset, remainder);

        char tempBuf[len]; // temp mem

        memcpy(tempBuf, &last, sizeof(filemsg));          // copy into buff
        strcpy(sizeof(filemsg) + tempBuf, fname.c_str()); // strcpy filename

        request_buffer->push(tempBuf, len); // push to buffer
    }
}

int main(int argc, char *argv[])
{
    int n = 1000;        // default number of requests per "patient"
    int p = 10;          // number of patients [1:,15:]
    int w = 100;         // default number of worker threads
    int h = 20;          // default number of histogram threads
    int b = 20;          // default capacity of the request buffer (should be changed)
    int m = MAX_MESSAGE; // default capacity of the message buffer
    string f = "";       // name of file to be transferred

    string r; // add aditional arguments
    string a;

    // read arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:p:w:h:b:m:f:a:r:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            n = stoi(optarg);
            break;
        case 'p':
            p = stoi(optarg);
            break;
        case 'w':
            w = stoi(optarg);
            break;
        case 'h':
            h = stoi(optarg);
            break;
        case 'b':
            b = stoi(optarg);
            break;
        case 'm':
            m = stoi(optarg);
            break;
        case 'f':
            f = optarg;
            break;
        case 'a':
            a = optarg;
            break;
        case 'r':
            r = optarg;
            break;
        }
    }

    // // fork and exec the server
    // int pid = fork();
    // if (pid == 0)
    // {
    //     execl("./server", "./server", "-m", (char *)to_string(m).c_str(), nullptr);
    // }

    // initialize overhead (including the control channel)
    TCPRequestChannel *chan = new TCPRequestChannel(a, r);
    BoundedBuffer request_buffer(b);
    BoundedBuffer response_buffer(b);
    HistogramCollection hc;

    // making histograms and adding to collection
    for (int i = 0; i < p; i++)
    {
        Histogram *h = new Histogram(10, -2.0, 2.0);
        hc.add(h);
    }

    // record start time
    struct timeval start, end;
    gettimeofday(&start, 0);

    // TODO

    /* create all threads here */

    // thread patients[p];
    // for (int i = 0; i < p; i++){
    //     patients[i] = thread(patient_thread_function, &request_buffer, n, i+1);
    // }

    // file trhead////////////////////////////////////////////////////////////////////////////////////////////////////////////
    thread filethread[1];
    thread patients[p]; // declare in this scope outside
    thread hcs[h];

    if (f == "") // mutallu exclusive
    {
        for (int i = 0; i < p; i++)
        {
            patients[i] = thread(patient_thread_function, &request_buffer, n, i + 1);
        }
        for (int i = 0; i < h; i++)
        {
            hcs[i] = thread(histogram_thread_function, &hc, &response_buffer);
        }
    }
    else
    {
        // declare a file pointert
        FILE *outfile;
        string filename = "received/" + f;
        outfile = fopen(filename.c_str(), "w+"); // with dir?

        filemsg fm(0, 0); // dummy filemesg
        int len = sizeof(filemsg) + (f.size() + 1);
        char buf[len];                            // buff of len size
        memcpy(buf, &fm, sizeof(filemsg));        // copy message
        strcpy(buf + sizeof(filemsg), f.c_str()); // copy in buffer
        chan->cwrite(buf, len);                   // write to channel
        // getting the file size above

        __int64_t getsize;
        chan->cread(&getsize, sizeof(__int64_t));

        filethread[0] = thread(file_thread_function, getsize, &request_buffer, f, m); // threading

        fclose(outfile);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    vector<TCPRequestChannel *> channels; // change to tcp

    thread workers[w];
    // char buf1[1024];
    for (int i = 0; i < w; i++)
    {

        TCPRequestChannel *c = new TCPRequestChannel(a, r); // create new tcp

        channels.push_back(c);

        workers[i] = thread(worker_thread_function, &response_buffer, &request_buffer, c, m);
    }
    // *******************************************************************************************************************************
    /* join all threads here */
    if (f == "") // patient and fil are mutually exlcuive
    {
        for (int i = 0; i < p; i++)
        {
            patients[i].join();
        }
    }
    else
    {
        filethread[0].join();
    }

    // file trheadv

    for (int i = 0; i < w; i++) // worker thread happens no matter what
    {
        // datamsg d(0, 0.0, 0);
        // d.MTYPE = QIT_MSG
        MESSAGE_TYPE d = QUIT_MSG;
        request_buffer.push((char *)&d, sizeof(MESSAGE_TYPE));
    }

    for (int i = 0; i < w; i++)
    {
        workers[i].join();
    }

    if (f == "") // join histo threads if !file
    {
        for (int i = 0; i < h; i++)
        {
            PatientData pd; // struct

            pd.response = -1.0; // impossible values to determine if qut
            pd.pno = -1;

            response_buffer.push((char *)&pd, sizeof(PatientData));
        }
        for (int i = 0; i < h; i++)
        {
            hcs[i].join();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // record end time
    gettimeofday(&end, 0);

    // print the results
    if (f == "")
    {
        hc.print();
    }
    int secs = ((1e6 * end.tv_sec - 1e6 * start.tv_sec) + (end.tv_usec - start.tv_usec)) / ((int)1e6);
    int usecs = (int)((1e6 * end.tv_sec - 1e6 * start.tv_sec) + (end.tv_usec - start.tv_usec)) % ((int)1e6);
    std::cout << "Took " << secs << " seconds and " << usecs << " micro seconds" << endl;

    // quit
    // deallocate and close
    for (int i = 0; i < w; i++)
    {
        // datamsg d(0, 0.0, 0);
        // d.MTYPE = QUIT_MSG
        MESSAGE_TYPE m = QUIT_MSG;
        channels[i]->cwrite(&m, sizeof(MESSAGE_TYPE)); //*****
        delete channels[i];
    }

    // quit and deallocate and close the chane
    // datamsg d(0, 0.0, 0);
    // d.MTYPE = QUIT_MSG
    MESSAGE_TYPE q = QUIT_MSG;
    chan->cwrite((char *)&q, sizeof(MESSAGE_TYPE));
    std::cout << "All Done!" << endl;
    delete chan;

    // wait for server to exit
    // wait(0)
    wait(nullptr);
}