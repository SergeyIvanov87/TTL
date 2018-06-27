/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LockessWriterBase.h
 * Author: user
 *
 * Created on June 27, 2018, 9:44 PM
 */

#ifndef LOCKESSWRITERBASE_H
#define LOCKESSWRITERBASE_H
#include <atomic>

template<class Impl>
class LocklessWriterBase
{
public:
    explicit LocklessWriterBase(size_t maxSize);
    ~LocklessWriterBase() = default;
    
    inline size_t writeDataText(const char *message, size_t messageSize);
    
    template<class ...Args>
    inline size_t writeDataArgs(Args &&...args);
protected:
    inline void onDataSizeLimitReached();    
    inline size_t writeDataMultithreadedImpl(const char* message, size_t messageSize);
    inline size_t writeDataSpecific(const char* message, size_t messageSize, size_t currentDataOffset);
    size_t m_maxDataSize;
    std::atomic<size_t> m_currentDataSize;
    std::atomic<size_t> m_writerThreadsCounter;
};
    

#endif /* LOCKESSWRITERBASE_H */

