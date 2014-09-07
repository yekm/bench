#ifndef VEC_TASK_H
#define VEC_TASK_H

#include "task.hpp"
#include "common/quickrandomdata.hpp"
#include "bexception.hpp"

struct VecResult : CustomResult
{
    std::size_t n;
    std::vector<float> lengths;
};

class VecTask : public Task
{
public:
    typedef float item_type;
    typedef GenericData<std::vector<item_type>> g_type;

    static constexpr int num_vectors = 1e6;
    static constexpr std::size_t max_vec_len = 128;

    VecTask()
        : Task(std::to_string(num_vectors) + " vector lengths", utils::PlotSettings(utils::PlotSettings::AS_LOGX, "vector length"))
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n)
    {
        return std::make_shared<common::RandomData<item_type>>(n*num_vectors, -10, 10);
        //return std::make_shared<common::SortedRandomData<item_type>>(n*num_vectors, -10, 10);
    }

    virtual bool get_n(std::size_t & n)
    {
        if (n < 2)
        {
            n = 2;
            return true;
        }
        if (n >= max_vec_len)
            return false;

        n *= 2;
        return true;
    }

    virtual void validate(const AResult & r)
    {
        const VecResult & vr = static_cast<const VecResult&>(r.get_custom_result());

        std::size_t c = vr.lengths.size(); // number of vectors
        int N = r.get_taskdata().get_n() / num_vectors; // vector coords count
        const float * v = & (static_cast<const VecTask::g_type&>(r.get_taskdata()).get_const())[0];

        for (std::size_t i = 0; i < c; i++)
        {
            double sum = 0;
            for (int j = 0; j < N; j++)
                sum += v[i*N+j] * v[i*N+j];

            // allow some rounding error.
            double delta = fabs(vr.lengths[i] - sqrt(sum));
            if (delta > 1e-4)
            {
                std::cout << "theirs:" << vr.lengths[i] << " check:" << sqrt(sum) << " delta:" << delta;
                throw BException("validation failed");
            }
        }
    }
};

#endif // VEC_TASK_H
