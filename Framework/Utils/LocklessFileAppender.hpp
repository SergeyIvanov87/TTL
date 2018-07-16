/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LocklessFileAppender.hpp
 * Author: user
 *
 * Created on June 23, 2018, 1:56 PM
 */

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
    
    mm_currSize = 0;
    mm_file = nullptr;
}

void LocklessFileAppender::onDataSizeLimitReachedImpl()
{
    
    time_t curTime = time(nullptr);
    std::string newName = m_path + std::to_string(curTime);
    int ret = rename(m_path.c_str(), newName.c_str());
    if(ret)
    {
        std::cout << "Cannot rename: " << strerror(errno) << std::endl;
    }
    openFile(m_path);
    //critical error here if openFile failed
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
            std::cout << "Cannot write to file: " << m_path << ", error: " << strerror(errno) << std::endl;
            abort();
            return 0;
        }
        writtenCount += ret;
        offset += writtenCount;
    }
    return writtenCount;
}

int LocklessFileAppender::openFile(const std::string &path)
{
    if(m_fileFd != -1)
    {
        close(m_fileFd);
    }
    
    m_path = path;
    
    m_fileFd = open(path.c_str(),  O_CREAT | O_WRONLY, S_IRWXO | S_IRWXG | S_IRWXU);
    if(m_fileFd == -1)
    {
        std::cout << "Cannot create file: " << path << ", error: " << strerror(errno) << std::endl;
        return -1;
    }
    
    int ret = fallocate(m_fileFd, 0, 0, m_maxDataSize);
    if(ret)
    {
        std::cout << "Cannot fallocate file: " << strerror(errno) << std::endl;
        return -1;
    }
    return ret;
}
int LocklessFileAppender::openFileMutex(std::string path)
{
    if(mm_file)
    {
        fclose(mm_file);
        mm_file = nullptr;
    }
    //m_fileFd = -1;
    openFile(path);
    close(m_fileFd);
    
    mm_file = fopen(path.c_str(), "w");
    if(!mm_file)
    {
        std::cout << "Cannot create file: " << path << ", error: " << strerror(errno) << std::endl;
        return -1;
    }
    return 1;
}

bool LocklessFileAppender::printMutex(const char *message, size_t messageSize)
{
    {
    std::unique_lock<std::mutex> lock(m_iomutex);
    mm_currSize += messageSize;
    if(mm_currSize >= m_maxDataSize)
    {
         time_t curTime = time(nullptr);
        std::string newName = m_path + std::to_string(curTime);
        int ret = rename(m_path.c_str(), newName.c_str());
        if(ret)
        {
            std::cout << "Cannot rename: " << strerror(errno) << std::endl;
        }
        openFileMutex(m_path);
        mm_currSize = 0;
    }
    fputs(message, mm_file);
    fflush(mm_file);
    }
    return true;  
}


#endif /* LOCKLESSFILEAPPENDER_HPP */

