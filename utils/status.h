#ifndef STATUS_H
#define STATUS_H

#include <string>

namespace utils
{

class Status
{
public:
    enum StatusEnum
    {
        SE_OK,
        SE_ERROR,
        SE_UNKNOWN_EXCEPTION,
        SE_OOM,
        SE_TIMEOUT,
    };

    Status();
    std::string str();
    StatusEnum get_status() const;
    void set_status(StatusEnum s, std::string m = "");

private:
    StatusEnum m_status;
    std::string m_error;
};

} // ns utils

#endif // STATUS_H
