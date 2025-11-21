#ifndef MUTEXFILEAPPENDER_HPP
#define MUTEXFILEAPPENDER_HPP
#include "MutexFileAppender.h"

MutexFileAppender::MutexFileAppender(size_t maxSize):
    m_maxDataSize(maxSize),
    m_path(),
    m_iomutex(),
    mm_currSize(),
    mm_file()
{
}

MutexFileAppender::~MutexFileAppender()
{
    fflush(mm_file);
}

int MutexFileAppender::allocateFile(const std::string &path)
{
    int fileFd = open(path.c_str(),  O_CREAT | O_WRONLY, S_IRWXO | S_IRWXG | S_IRWXU);
    if(fileFd == -1)
    {
        std::cout << "Cannot create file: " << path << ", error: " << strerror(errno) << std::endl;
        return -1;
    }

    int ret = fallocate(fileFd, 0, 0, m_maxDataSize);
    if(ret)
    {
        std::cout << "Cannot fallocate file: " << strerror(errno) << std::endl;
        close(fileFd);
        return -1;
    }
    m_path = path;
    return fileFd;
}

void MutexFileAppender::openFile(const std::string &path)
{
    if(mm_file)
    {
        fclose(mm_file);
        mm_file = nullptr;
    }
    int file_fd = allocateFile(path);
    mm_file = fdopen(file_fd, "w");
    if(!mm_file)
    {
        if (file_fd != -1)
        {
            close(file_fd);
        }
        throw std::runtime_error("Cannot create file: " + path  + ", error: " + strerror(errno));
    }
    mm_currSize = 0;
}

void MutexFileAppender::onResourceReplenishingImpl()
{
    time_t curTime = time(nullptr);
    std::string newName = m_path + std::to_string(curTime);
    int ret = rename(m_path.c_str(), newName.c_str());
    if(ret)
    {
        throw std::runtime_error("Cannot rename file: " + m_path  + " on: " + newName + ", error: " + strerror(errno));
    }
    openFile(m_path);
}

bool MutexFileAppender::print(const char *message, size_t messageSize)
{
    {
        std::unique_lock<std::mutex> lock(m_iomutex);
        mm_currSize += messageSize;
        if(mm_currSize >= m_maxDataSize)
        {
            onResourceReplenishingImpl();
        }
        fputs(message, mm_file);
        fflush(mm_file);
    }
    return true;
}

#endif // MUTEXFILEAPPENDER_HPP
