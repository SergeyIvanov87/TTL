#ifndef LOCKLESSFILEAPPENDER_HPP
#define LOCKLESSFILEAPPENDER_HPP
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cassert>
#include "LocklessFileAppender.h"

LocklessFileAppender::LocklessFileAppender(size_t maxSize):
    LocklessWriterBase<LocklessFileAppender>(maxSize),
    m_path(),
    m_fileFd(-1)
{
}

LocklessFileAppender::~LocklessFileAppender()
{
    close(m_fileFd);
}

void LocklessFileAppender::onDataSizeLimitReachedImpl()
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

size_t LocklessFileAppender::writeDataImpl(const char *message, size_t messageSize, size_t offset)
{
    //write own data
    size_t writtenCount = 0;
    while(writtenCount != messageSize)
    {
        ssize_t ret = pwrite(m_fileFd, message + writtenCount, messageSize - writtenCount, offset);
        if(ret == -1)
        {
            abort();
            throw std::runtime_error("Cannot write to file: " + m_path  + ", error: " + strerror(errno));
        }
        writtenCount += ret;
        offset += writtenCount;
    }
    return writtenCount;
}

void LocklessFileAppender::openFile(const std::string &path)
{
    if(m_fileFd != -1)
    {
        close(m_fileFd);
    }

    m_path = path;

    m_fileFd = open(path.c_str(),  O_CREAT | O_WRONLY, S_IRWXO | S_IRWXG | S_IRWXU);
    if(m_fileFd == -1)
    {
        throw std::runtime_error("Cannot create file: " + path  + ", error: " + strerror(errno));
    }

    if(!fallocate(m_fileFd, 0, 0, m_maxDataSize))
    {
        throw std::runtime_error(std::string("Cannot perform `fallocate` on file, error: ") + strerror(errno));
    }
}
#endif /* LOCKLESSFILEAPPENDER_HPP */
