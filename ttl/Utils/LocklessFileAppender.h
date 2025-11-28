#ifndef LOCKLESSFILEAPPENDER_H
#define LOCKLESSFILEAPPENDER_H
#include "LocklessWriterBase.hpp"

class LocklessFileAppender : public LocklessWriterBase<LocklessFileAppender>
{
public:
    using Base = LocklessWriterBase<LocklessFileAppender>;
    friend class LocklessWriterBase<LocklessFileAppender>;
    using Base::m_maxDataSize;
    LocklessFileAppender(size_t maxSize);
    ~LocklessFileAppender();

    void openFile(const std::string &path);
private:
    void onResourceReplenishingImpl();
    size_t writeDataImpl(const char *message, size_t messageSize, size_t offset);
    std::string m_path;
    int m_fileFd;
};


#endif /* LOCKLESSFILEAPPENDER_H */
