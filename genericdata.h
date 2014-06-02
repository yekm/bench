#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "taskdata.h"
#include "utils/dbg.h"

template <typename T>
class GenericData : public TaskData
{
public:
    typedef T container_type;

    const T & get_const() const
    {
        return m_data;
    }

    T & get_mutable()
    {
        m_valid = false;
        return m_data;
    }

    virtual std::shared_ptr<TaskData> clone()
    {
        if (m_valid)
        {
            D() << "clone shared";
            return shared_from_this();
        }
        else
        {
            D() << "clone copy";
            return clone_copy();
        }
    }

protected:
    GenericData(const std::string & name, std::size_t n)
        : TaskData(name, n)
        , m_valid(true)
    {}
    T m_data;

private:
    GenericData(const GenericData &) = delete;
    GenericData(GenericData &&) = delete;
    GenericData operator=(const GenericData&) = delete;
    GenericData operator=(GenericData &&) = delete;
    bool m_valid;
};

#endif // GENERICDATA_H
