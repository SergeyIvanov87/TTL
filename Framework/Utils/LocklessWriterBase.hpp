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
inline void LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::onResourceReplenishing()
{
    static_cast<TEMPLATE_ARGS_LIST_DEF &>(*this).onResourceReplenishingImpl();
}

template<TEMPLATE_ARGS_LIST_DECL>
inline size_t LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::writeDataMultithreadedImpl(const char* message, size_t messageSize)
{
    /* increase worker threads counter */
    m_writerThreadsCounter ++;

    /* reserve a writing position by each threads */
    size_t reservedOffset = m_currentDataSize.fetch_add(messageSize);

    /* based on `reservedOffset` value and other conditions there are several situations possible:
     * 1) all threads write their data in parallel.
     * 2) the new thread arrives and overflows the file
     * 3) several new threads come after and find out there are is place to write data in the file
     *
     * Thread from 2) is called changing-epoch-thread (CE-thread)
     * and must wait for all writing operations submitted from different thread finish
     *
     * Threads from 1) are called previous-epoch-threads (PE-threads)
     * and must finish their submitted writing operations
     *
     * Threads from 3) are called new-epoch-threads (NE-threads)
     * and must wait for CE-thread finish a resource replenishing
     * */

     /********** The NE-threads recognition condition **********/
    if(reservedOffset >= m_maxDataSize)
    {
        m_writerThreadsCounter --;
        while((reservedOffset = m_currentDataSize.load()) >= m_maxDataSize
              /* (1) wait untill resource become replenished */
                ||
              !m_currentDataSize.compare_exchange_strong(reservedOffset, reservedOffset + messageSize)
              /* (2) multiple NE-threads write-competition */)
        {
            std::this_thread::yield();
        }
        /* NE-thread "competition" winner start writing new file upon epoch changed */
        m_writerThreadsCounter ++;

        /***** Here we have to check on double epoch new CE-thread appearance **********/
        /***** (See the CE-thread recognition condition below) *******/
        if (reservedOffset < m_maxDataSize && reservedOffset + messageSize >= m_maxDataSize)
        {
            waitForWorkersDone();

            /* execution barrier for new CE-thread has been reach */
            /* new CE-thread is the single thread here, no data-races are possible */
            size_t writtenBytes = writeDataSpecific(message, m_maxDataSize - reservedOffset, reservedOffset); //write untill end

            //notify Impl about buffer ending
            onResourceReplenishing();

            writtenBytes += writeDataSpecific(message + writtenBytes, messageSize - writtenBytes, 0); //write from beginning

            /* restore invariant (unblock other NE-threads from waiting) */
            m_currentDataSize.store(messageSize - writtenBytes);

            /* decrease worker threads counter */
            m_writerThreadsCounter --;
            return writtenBytes;
        }
        //write data in regular way below
    }
    /********** The CE-thread recognition condition **********/
    else if (reservedOffset < m_maxDataSize                     /* (1) has free space before */
             && reservedOffset + messageSize >= m_maxDataSize)  /* (2) no free space after */
    {
        waitForWorkersDone();

        /* execution barrier for CE-thread has been reach */
        /* CE-thread is the single thread here, no data-races are possible */
        size_t writtenBytes = writeDataSpecific(message, m_maxDataSize - reservedOffset, reservedOffset); // write until end is reached

        /* resource replenishing notification */
        onResourceReplenishing();

        writtenBytes += writeDataSpecific(message + writtenBytes, messageSize - writtenBytes, 0); // write from new beginning

        /* restore invariant (unblock NE-threads from waiting) */
        m_currentDataSize.store(messageSize - writtenBytes);

        /* decrease worker threads counter */
        m_writerThreadsCounter --;
        return writtenBytes;
    }

    /* multiple worker threads can be here */
    /* may be considered as PE-threads, since CE-thread appeared */
    /* PE-threads have to finish their writing & CE-thread must wait */
    size_t res = writeDataSpecific(message, messageSize, reservedOffset);

    /* decrease worker threads counter (unblock CE-thread from waiting) */
    m_writerThreadsCounter --;
    return res;
}

template<TEMPLATE_ARGS_LIST_DECL>
inline void LocklessWriterBase<TEMPLATE_ARGS_LIST_DEF>::waitForWorkersDone()
{
    /* decrease worker threads counter */
    m_writerThreadsCounter --;
    size_t expected_PE_threads_count = 0;
    while(!m_writerThreadsCounter.compare_exchange_strong(expected_PE_threads_count, 1))
    {
        std::this_thread::yield();
        expected_PE_threads_count = 0;   /* recharge `expected` again */
    }
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF
#endif /* LOCKESSWRITERBASE_HPP */
