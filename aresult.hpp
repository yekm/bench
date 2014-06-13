#ifndef ARESULT_HPP
#define ARESULT_HPP

#include "taskdata.hpp"

class AResult
{
public:
    AResult();
    explicit AResult(std::shared_ptr<TaskData>);
    AResult(const AResult &);
    const TaskData & get_taskdata() const;
    std::shared_ptr<TaskData> get_ptr() const;
private:
    std::shared_ptr<TaskData> m_td;
};

#endif // ARESULT_HPP
