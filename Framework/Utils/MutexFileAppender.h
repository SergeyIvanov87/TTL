#ifndef MUTEXFILEAPPENDER_H
#define MUTEXFILEAPPENDER_H

#include <stdio.h>

#include <mutex>
#include <string>

class MutexFileAppender
{
public:
    MutexFileAppender(size_t maxSize);
    ~MutexFileAppender();

    void openFile(const std::string &path);
    bool print(const char *message, size_t messageSize);
private:
    int allocateFile(const std::string &path);
    void onDataSizeLimitReachedImpl();
    size_t writeDataImpl(const char *message, size_t messageSize, size_t offset);

    size_t m_maxDataSize;
    std::string m_path;

    std::mutex m_iomutex;
    size_t mm_currSize;
    FILE *mm_file;
};
#endif /* MUTEXFILEAPPENDER_H */
