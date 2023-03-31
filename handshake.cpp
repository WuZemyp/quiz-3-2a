
#include <iostream>
#include <thread>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
std::mutex m;
std::condition_variable c;
bool done=false;
bool done2=true;
// include additional necessary headers

void query(int count) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    for(int i=0;i<count;++i)
    {
        std::unique_lock<std::mutex> ul(m);
        c.wait(ul,[]{return done2;});
        std::cout<<"["<<i<<"] "<<"SYN ...";
        done=true;
        done2=false;
        ul.unlock();
        c.notify_one();
    }
}

void response(int count) {
    // Should print "ACK"
    for(int i=0;i<count;++i)
    {
        std::unique_lock<std::mutex> ul(m);
        c.wait(ul,[]{return done;});
        std::cout<<"ACK"<<std::endl;
        done=false;
        done2=true;
        ul.unlock();
        c.notify_one();
        
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }
    int count=atoi(argv[1]);
    std::thread thread1=std::thread(query,count);
    std::thread thread2=std::thread(response,count);
    thread1.join();
    thread2.join();
    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
   
    return 0;
}