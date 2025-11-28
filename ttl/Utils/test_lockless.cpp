#ifdef TEST

#include <atomic>
#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <functional>

#include "LocklessFileAppender.hpp"

size_t messageSize = 150;
void writer(LocklessFileAppender &app, std::atomic<bool> &isStart, std::atomic<bool> &isStop, size_t index)
{
    std::string message(messageSize, 'a');
    message = std::to_string(index) + "-" + message + "\n";

    while(!isStart)
    {
        sleep(0);
    }
    while(!isStop)
    {
      app.writeDataText(message.c_str(), message.size());
      //app.printMutex(message.c_str(), message.size());
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
    LocklessFileAppender app(maxFileSize);
    //app.openFileMutex("./testFile");
    app.openFile("./testFile");

    std::atomic<bool> isStop {false};
    std::atomic<bool> isStart {false};
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
