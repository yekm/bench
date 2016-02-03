#include "status.hpp"

namespace utils {

Status::Status()
    : m_status(StatusEnum::SE_OK)
{
}

Status::StatusEnum Status::get_status() const
{
    return m_status;
}

void Status::set_status(Status::StatusEnum s, std::string m)
{
    m_status = s;
    m_message = m;
}

std::string Status::str()
{
    switch (m_status)
    {
    case StatusEnum::SE_OK:
        return "Ok";
    case StatusEnum::SE_ERROR:
        return "Error (" + m_message + ")";
    case StatusEnum::SE_OOM:
        return "Out of memory (" + m_message + ")";
    case StatusEnum::SE_TIMEOUT:
        return "Timeout";
    case StatusEnum::SE_EXCEPTION:
        return "Exception (" + m_message + ")";
    case StatusEnum::SE_UNKNOWN_EXCEPTION:
        return "Unknown exception";
    case StatusEnum::SE_SKIP:
        return "Skipped";
    default:
        return "FIXME: something is missing...";
    }
}

bool Status::ok()
{
    return m_status == StatusEnum::SE_OK;
}

bool Status::ok_timeout()
{
    return m_status == StatusEnum::SE_OK || m_status == StatusEnum::SE_TIMEOUT;
}

} // ns utils
