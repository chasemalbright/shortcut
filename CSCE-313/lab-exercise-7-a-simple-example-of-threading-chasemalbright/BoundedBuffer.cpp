#include "BoundedBuffer.h"

using namespace std;

BoundedBuffer::BoundedBuffer(int _cap) : cap(_cap)
{
    // modify as needed
    // shouldnt change needs just cap
}

BoundedBuffer::~BoundedBuffer()
{
    // modify as needed
    // shouldnt need to change, using vector and q ~ no mem
}

// TODO
void BoundedBuffer::push(char *msg, int size)
{
    // 1. Convert the incoming byte sequence given by msg and size into a vector<char>
    vector<char> byteSequence(msg, msg + size);
    // 2. Wait until there is room in the queue (i.e., queue length is less than cap)
    // use unique lock?
    unique_lock<mutex> ul(m); // pass it the privat var m LOCK
    // use notify_all and wait
    // checkOverflow.wait(ul, q.size() < cap);
    pushReady.wait(ul, [this]
                   { return (int)q.size() < cap; });

    // 3. Then push the vector at the end of the queue
    q.push(byteSequence);

    // 4. Wake up threads that were waiting for push
    ul.unlock(); // UNLOCK

    popReady.notify_one(); // notify signal
}

// TODO
int BoundedBuffer::pop(char *msg, int size)
{

    // 0 create unique lock?
    unique_lock<mutex> ul(m);

    // 1. Wait until the queue has at least 1 item
    popReady.wait(ul, [this]
                  { return !q.empty(); });

    // 2. Pop the front item of the queue. The popped item is a vector<char>
    // store first before pop
    vector<char> byteSequence = q.front();
    q.pop();
    ul.unlock(); // unlock
    // 3. Convert the popped vector<char> into a char*, copy that into msg; assert that the vector<char>'s length is <= size
    assert((int)byteSequence.size() <= size);
    memcpy(msg, byteSequence.data(),byteSequence.size()); // convert vector<char> into a char* with .data()

    // 4. Wake up threads that were waiting for pop
    pushReady.notify_one(); // notify signal
    // 5. Return the vector's length to the caller so that they know how many bytes were popped
    return byteSequence.size();
}

size_t BoundedBuffer::size()
{ // dont change
    return q.size();
}
