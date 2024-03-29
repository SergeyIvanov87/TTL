#ifdef TEST

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <functional>

#include "MutexFileAppender.hpp"

size_t messageSize = 150;
void writer(MutexFileAppender &app,bool &isStart, bool &isStop, size_t index)
{
    std::string message(messageSize, 'a');
    message = std::to_string(index) + "-" + message + "\n";

    while(!isStart)
    {
        sleep(0);
    }
    while(!isStop)
    {
      app.print(message.c_str(), message.size());
    }
}

size_t threads_count = 24;
int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        std::cout << "threadsCount maxFileSize oneMessageSize" << std::endl;
        return 0;
    }
    threads_count = atoll(argv[1]);
    size_t maxFileSize = atoll(argv[2]);
    messageSize = atoll(argv[3]);
    MutexFileAppender app(maxFileSize);
    app.openFile("./testFile");

     bool isStop = false, isStart = false;
    std::vector<std::thread> threadArray;
    for(size_t i = 0 ; i < threads_count; i++)
    {
        threadArray.push_back(std::thread(writer, std::ref(app), std::ref(isStart), std::ref(isStop), i));
    }
    isStart = true;
    sleep(60);
    isStop = true;
    for(auto &t : threadArray)
    {
        t.join();
    }
}
#endif
