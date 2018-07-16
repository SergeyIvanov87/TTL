/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LocklessFileAppender.h
 * Author: user
 *
 * Created on June 23, 2018, 1:54 PM
 */

#ifndef LOCKLESSFILEAPPENDER_H
#define LOCKLESSFILEAPPENDER_H
#include "LocklessWriterBase.hpp"
#include <mutex>

class LocklessFileAppender : public LocklessWriterBase<LocklessFileAppender>
{
public:
    using Base = LocklessWriterBase<LocklessFileAppender>;
    friend class LocklessWriterBase<LocklessFileAppender>;
    using Base::m_maxDataSize;
    LocklessFileAppender(size_t maxSize);
    
    int openFile(const std::string &path);
    int openFileMutex(std::string path);
    bool printMutex(const char *message, size_t messageSize);
private:
    void onDataSizeLimitReachedImpl();
    size_t writeDataImpl(const char *message, size_t messageSize, size_t offset);
    std::string m_path;
    int m_fileFd;
    
    std::mutex m_iomutex;
    size_t mm_currSize;
    FILE *mm_file;

};


#endif /* LOCKLESSFILEAPPENDER_H */

