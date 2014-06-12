#include "status.hpp"

namespace utils {

Status::Status()
    : m_status(SE_OK)
{
}

Status::StatusEnum Status::get_status() const
{
    return m_status;
}

void Status::set_status(Status::StatusEnum s, std::string m)
{
    m_status = s;
    m_error = m;
}

std::string Status::str()
{
    switch (m_status)
    {
    case SE_OK:
        return "Ok";
    case SE_ERROR:
        return m_error;
    case SE_OOM:
        return "Out of memory";
    case SE_TIMEOUT:
        return "Timeout";
    case SE_UNKNOWN_EXCEPTION:
        return "Unknown exception";
    default:
        return "FIXME: something is missing...";
    }
}

} // ns utils
