#ifndef STATUS_H
#define STATUS_H

#include <string>

namespace utils
{

class Status
{
public:
    enum class StatusEnum
    {
        SE_OK,
        SE_ERROR,
        SE_EXCEPTION,
        SE_UNKNOWN_EXCEPTION,
        SE_OOM,
        SE_TIMEOUT,
        SE_SKIP,
    };

    Status();
    std::string str();
    StatusEnum get_status() const;
    bool ok();
    bool ok_timeout();
    void set_status(StatusEnum s, std::string m = "");

private:
    StatusEnum m_status;
    std::string m_message;
};

} // ns utils

#endif // STATUS_H
