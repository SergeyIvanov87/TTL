/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LockessWriterBase.hpp
 * Author: user
 *
 * Created on June 27, 2018, 9:52 PM
 */

#ifndef LOCKESSWRITERBASE_HPP
#define LOCKESSWRITERBASE_HPP
#include "StringUtils.h"
#include "LocklessWriterBase.h"
#include <thread>


#define TEMPLATE_ARGS_LIST_DECL  class Impl
#define TEMPLATE_ARGS_LIST_DEF   Impl

template<TEMPLATE_ARGS_LIST_DECL>
LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::LocklessWriterBase(size_t maxSize) :
 m_maxDataSize(maxSize),
 m_currentDataSize(0),
 m_writerThreadsCounter(0)
{
}

template<TEMPLATE_ARGS_LIST_DECL>
inline size_t LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::writeDataText(const char *message, size_t messageSize)
{
    return writeDataMultithreadedImpl(message, messageSize);
}
    
template<TEMPLATE_ARGS_LIST_DECL>
template<class ...Args>
inline size_t LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::writeDataArgs(Args &&...args)
{
    std::string message = makeString(std::forward<Args>(args)...);
    return writeDataMultithreadedImpl(message.c_str(), message.size());
}

template<TEMPLATE_ARGS_LIST_DECL>
inline size_t LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::writeDataSpecific(const char* message, size_t messageSize, size_t currentDataOffset)
{
    return static_cast<TEMPLATE_ARGS_LIST_DEF &>(*this).writeDataImpl(message, messageSize, currentDataOffset);
}

template<TEMPLATE_ARGS_LIST_DECL>
inline void LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::onDataSizeLimitReached()
{
    static_cast<TEMPLATE_ARGS_LIST_DEF &>(*this).onDataSizeLimitReachedImpl();
}

template<TEMPLATE_ARGS_LIST_DECL>
inline size_t LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::writeDataMultithreadedImpl(const char* message, size_t messageSize)
{
    //increase threads count
    m_writerThreadsCounter ++;
    
    //try to reserve buffer size portion to write
    size_t reservedOffset = m_currentDataSize.fetch_add(messageSize);

    //Logic based on current reserved size count
    if(reservedOffset >= m_maxDataSize)
    //(A) block
    {//I reserve size AFTER limit reached - I need to wait while onDataSizeLimitReached() finished in (B) block
        m_writerThreadsCounter --;
        while((reservedOffset = m_currentDataSize.load()) >= m_maxDataSize/* - messageSize*/
                || 
              !m_currentDataSize.compare_exchange_strong(reservedOffset, reservedOffset + messageSize))
        {
            std::this_thread::yield();
        }
        //onDataSizeLimitReached() finished and i reserved size portion here
        m_writerThreadsCounter ++;
        
        //check that portion is enough
        if (reservedOffset < m_maxDataSize && reservedOffset + messageSize >= m_maxDataSize)
        {
            //i am the first threads who detected buffer ending
            //decrease threads count
            m_writerThreadsCounter --;//to check when all writer threads are gone
            size_t zero = 0;
            while(!m_writerThreadsCounter.compare_exchange_strong(zero, 1))
            {
                std::this_thread::yield();
                zero = 0;
            }
        
            size_t writtenBytes = writeDataSpecific(message, m_maxDataSize - reservedOffset, reservedOffset); //write untill end
            
            //notify Impl about buffer ending
            onDataSizeLimitReached();
            
            writeDataSpecific(message + writtenBytes, messageSize - writtenBytes, 0); //write from beginning
            
            //set new size portion from beginning
            m_currentDataSize.store(messageSize - writtenBytes);
            
            //decrease threads count
            m_writerThreadsCounter --;
            return true;
        }
        //write data in regular way below
    }
    else if (reservedOffset < m_maxDataSize && reservedOffset + messageSize >= m_maxDataSize)
    //(B) block - i reserve data, but my message has being finalized buffer
    {
        //decrease threads count
        m_writerThreadsCounter --;//to check when all writer threads are gone in block (C)
        size_t zero = 0;
        while(!m_writerThreadsCounter.compare_exchange_strong(zero, 1))
        {
            std::this_thread::yield();
            zero = 0;
        }
        size_t writtenBytes = writeDataSpecific(message, m_maxDataSize - reservedOffset, reservedOffset); //write untill end
            
        //notify Impl about buffer ending
        onDataSizeLimitReached();
            
        writeDataSpecific(message + writtenBytes, messageSize - writtenBytes, 0); //write from beginning
            
        //set new size portion from beginning
        m_currentDataSize.store(messageSize - writtenBytes);
        
        //decrease threads count
        m_writerThreadsCounter --;
        return true;
    }
  
    //(C) block - regular writing to buffer, from reservedOffset
    //Several thread-workers can be here
    size_t res = writeDataSpecific(message, messageSize, reservedOffset);
    
    //decrease threads count
    m_writerThreadsCounter --;
    return res;
}

#endif /* LOCKESSWRITERBASE_HPP */

