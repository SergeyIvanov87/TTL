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
    inline void onResourceReplenishing();
    inline size_t writeDataMultithreadedImpl(const char* message, size_t messageSize);
    inline size_t writeDataSpecific(const char* message, size_t messageSize, size_t currentDataOffset);
    size_t m_maxDataSize;
    std::atomic<size_t> m_currentDataSize;
    std::atomic<size_t> m_writerThreadsCounter;
private:
    inline void waitForWorkersDone();
};


#endif /* LOCKESSWRITERBASE_H */
