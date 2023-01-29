#include "RoundRobin.h"

/*
This is a constructor for RoundRobin Scheduler, you should use the extractProcessInfo function first
to load process information to process_info and then sort process by arrival time;

Also initialize time_quantum
*/
RoundRobin::RoundRobin(string file, int time_quantum)
{
	extractProcessInfo(file);
	set_time_quantum(time_quantum);

}

// Schedule tasks based on RoundRobin Rule
// the jobs are put in the order the arrived
// Make sure you print out the information like we put in the document
void RoundRobin::schedule_tasks()
{	
	int currTime = 0; // time var to keep track of time


	while(!(processVec.empty())){ // want to keep rotating through front of queue till it is empty

		shared_ptr<Process> currProcessePtr = processVec.front(); // set to first process in queue

		if ( currTime < currProcessePtr->get_arrival_time()){ // if time less than arrival
			print(currTime, -1, false); // print process
			currTime++; // increment currrtime
			continue; // new loop
		}

		processVec.pop(); // pop front

		int runTime; // run time var

		if(currProcessePtr->get_remaining_time() < time_quantum){ // remaining time
			runTime = currProcessePtr->get_remaining_time();
		} else { // == either remaining time or timequantum
			runTime = time_quantum;
		}

		currProcessePtr->Run(runTime); // call run fucntion

		for (int i = 0; i < runTime; i++){ // print per run time
			print(currTime, currProcessePtr->getPid(), false); // try false
			currTime++;
		}
		// for (int i = 0; i < runTime; i++){ // 
		// 	print(currTime, currProcessePtr->getPid(), true); 
		// 	currTime+;
		// }

		if (currProcessePtr->is_Completed()){
			print(currTime,currProcessePtr->getPid(), true); // if completed print
		} else {
			processVec.push(currProcessePtr); // if not completed push to quueu
		}

	}
}

/***************************
ALL FUNCTIONS UNDER THIS LINE ARE COMPLETED FOR YOU
You can modify them if you'd like, though :)
***************************/

// Default constructor
RoundRobin::RoundRobin()
{
	time_quantum = 0;
}

// Time quantum setter
void RoundRobin::set_time_quantum(int quantum)
{
	this->time_quantum = quantum;
}

// Time quantum getter
int RoundRobin::get_time_quantum()
{
	return time_quantum;
}

// Print function for outputting system time as part of the schedule tasks function
void RoundRobin::print(int system_time, int pid, bool isComplete)
{
	string s_pid = pid == -1 ? "NOP" : to_string(pid);
	cout << "System Time [" << system_time << "].........Process[PID=" << s_pid << "] ";
	if (isComplete)
		cout << "finished its job!" << endl;
	else
		cout << "is Running" << endl;
}

// Read a process file to extract process information
// All content goes to proces_info vector
void RoundRobin::extractProcessInfo(string file)
{
	// open file
	ifstream processFile(file);
	if (!processFile.is_open())
	{
		perror("could not open file");
		exit(1);
	}

	// read contents and populate process_info vector
	string curr_line, temp_num;
	int curr_pid, curr_arrival_time, curr_burst_time;
	while (getline(processFile, curr_line))
	{
		// use string stream to seperate by comma
		stringstream ss(curr_line);
		getline(ss, temp_num, ',');
		curr_pid = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_arrival_time = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_burst_time = stoi(temp_num);
		shared_ptr<Process> p(new Process(curr_pid, curr_arrival_time, curr_burst_time));

		processVec.push(p);
	}

	// close file
	processFile.close();
}