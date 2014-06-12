#ifndef TASKDATA_H
#define TASKDATA_H

#include <string>
#include <memory>
#include <iostream>

class TaskData : public std::enable_shared_from_this<TaskData>
{
public:
    virtual ~TaskData() = 0;

    std::string get_name() const;
    std::size_t get_n() const;

    virtual std::shared_ptr<TaskData> clone() = 0;

    /* clone_copy should return same data because algorithm may rely on it
     **/
    virtual std::shared_ptr<TaskData> clone_copy() const = 0;
/*
    {
        // cout << warning
    }
*/

protected:
    TaskData(const std::string & name, std::size_t);

private:
    const std::string m_name;
    const std::size_t m_n;
};

#endif // TASKDATA_H
