#ifndef RESULT_CODES
#define RESULT_CODES
#include <string.h>
#include <string>
#include <exception>
#include <ttl/include/Utils/StringUtils.h>

namespace urc
{

enum class ResultCodes
{
    RESULT_OK,
    RESULT_BAD,
    RESULT_NOT_ENOUGH_RESOURCES,
    RESULT_CONTINUE_SERVICE,
    RESULT_DIE,
    RESULT_CHECK,

    RESULT_SYSTEM_IO_ERROR,
    RESULT_SYSTEM_RESOURCE_LOCATION_ERROR,

    RESULT_MAX_CODE
};

struct ResultDescription : public std::exception
{
    ResultDescription(): m_errorCode(ResultCodes::RESULT_OK), m_description() {}
    ResultDescription(ResultCodes errorCode): m_errorCode(errorCode), m_description() {}

    template <class ...Args>
    ResultDescription(ResultCodes errorCode, Args &&...args) :
        m_errorCode(errorCode), m_description(makeString(std::forward<Args>(args)...)){}

    virtual const char* what() const noexcept
    {
        return m_description.c_str();
    }
    ResultCodes errorCode() const
    {
        return m_errorCode;
    }
    std::string to_string() const
    {
        return makeString(m_description, ". ResultCode: ", (int)m_errorCode);
    }
private:
    ResultCodes m_errorCode;
    std::string m_description;
};

struct SystemError : public ResultDescription
{
    SystemError() = default;
    SystemError(ResultCodes errorCode) : ResultDescription(errorCode) {}

    template <class ...Args>
    SystemError(ResultCodes errorCode, Args &&...args) :
        ResultDescription(errorCode, std::forward<Args>(args)...) {}
};

struct FileOpenError : public SystemError
{
    FileOpenError(const std::string &path, int errorCode) :
        SystemError(ResultCodes::RESULT_SYSTEM_IO_ERROR, "Cannot open: \"", path , "\" Error: \"", strerror(errorCode)) {}
};

struct MissingResourceError : public SystemError
{
    MissingResourceError(const std::string &name, const std::string &reason = std::string()) :
        SystemError(ResultCodes::RESULT_SYSTEM_RESOURCE_LOCATION_ERROR, "Cannot get resource: \"", name, "\"", (reason.empty() ? "" : " Error: \"" + reason + "\"")) {}
    MissingResourceError(const std::string &name, const unsigned char *reason) :
        MissingResourceError(name, reinterpret_cast<const char *>(reason)) {}
};
}
#endif //RESULT_CODES
