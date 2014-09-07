#ifndef ARESULT_HPP
#define ARESULT_HPP

#include "taskdata.hpp"

struct CustomResult
{
    virtual ~CustomResult();
};

class AResult
{
public:
    AResult();
    explicit AResult(std::shared_ptr<TaskData>);
    AResult(const AResult &);
    const TaskData & get_taskdata() const;
    const CustomResult & get_custom_result() const;
    void set_custom_result(std::unique_ptr<CustomResult> && res);
    virtual ~AResult();
private:
    std::shared_ptr<TaskData> m_td;
    std::unique_ptr<CustomResult> m_result;
};

#endif // ARESULT_HPP
