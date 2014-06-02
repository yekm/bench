#include "algorithmstatdecorator.h"
#include "utils/timer.h"

#include <cassert>

AlgorithmStatDecorator::AlgorithmStatDecorator(std::unique_ptr<Algorithm>&& a)
    : Algorithm("WAIT. OH SHI~")
    , m_alg(std::move(a))
    , m_status(AS_OK)
{
}

void AlgorithmStatDecorator::run(TaskData & td)
{
    //assert(m_status == AS_OK);
    if (m_status != AS_OK)
        return;
    utils::Timer::timediff_type d;
    try
    {
        utils::Timer timer;
        m_alg->run(td);
        d = timer.get();
    }
    catch (const Algorithm::AlgExeption & e)
    {
        m_status = AS_ERROR;
        m_error_sring = e.what();
        return;
    }
    catch (const std::bad_alloc & e)
    {
        m_status = AS_OOM;
        return;
    }
    catch (...)
    {
        m_status = AS_EXEPTION;
        return;
    }
    m_stat_run[td.get_n()].add(d);
    return;
}

void AlgorithmStatDecorator::prepare(const TaskData & td)
{
    // TODO: catch
    //assert(m_status == AS_OK);
    if (m_status != AS_OK)
        return;
    utils::Timer timer;
    m_alg->prepare(td);
    double d = timer.get();
    m_stat_prepare[td.get_n()].add(d);
}

void AlgorithmStatDecorator::check(const TaskData & td)
{
    //assert(m_status == AS_OK);
    if (m_status != AS_OK)
        return;
    try
    {
        m_alg->check(td);
    }
    catch (const Algorithm::AlgExeption & e)
    {
        m_status = AS_ERROR;
        m_error_sring = e.what();
    }
    catch (...)
    {
        m_status = AS_EXEPTION;
    }
}

AlgorithmStatDecorator::AlgorithmStatus AlgorithmStatDecorator::get_status()
{
    return m_status;
}
