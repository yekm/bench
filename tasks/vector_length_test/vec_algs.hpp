#ifndef VEC_ALGS_H
#define VEC_ALGS_H

#include "algorithm.hpp"
#include "vec_task.hpp"
#include "mpl.hpp"
#include <algorithm>
#include "utils/dbg.hpp"
#include "vecresult.hpp"
#include "bexception.hpp"

class vec_template : public Algorithm
{
public:
    vec_template()
        : Algorithm("template unrolling")
    {}

private:
    template <int N>
    inline void switcher(int v, const VecTask::g_type::container_type &d, std::vector<float> & l)
    {
        if (v == N)
            count<N>(&d[0], d.size()/N, l);
        else
            switcher<N/2>(v, d, l);
    }

    template <int N>
    void count(const float * v, std::size_t c, std::vector<float> & l)
    {
        D() << "count<" << N << ">";
        for (std::size_t i = 0; i < c; i++)
        {
            l.push_back(mpl::vlen<N>::value(v+i*N));
        }
    }

    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const VecTask::g_type::container_type &d = static_cast<VecTask::g_type&>(td).get_const();
        std::unique_ptr<VecResult> res(new VecResult(*r.get(), td.get_n()));
        res->lengths.reserve(VecTask::num_vectors);
        D() << "vector: " << d;
        switcher<VecTask::max_vec_len>(td.get_n()/VecTask::num_vectors, d, res->lengths);
        D() << "lengths: " << res->lengths;
        r = std::move(res);
    }
};

template <>
inline void vec_template::switcher<1>(int, const VecTask::g_type::container_type &, std::vector<float> &)
{
    throw BException("unsupported vector length " + get_name());
}


class vec_loop : public Algorithm
{
public:
    vec_loop()
        : Algorithm("loop unrolling")
    {}

private:
    template <int N>
    inline void switcher(int v, const VecTask::g_type::container_type &d, std::vector<float> & l)
    {
        if (v == N)
            count<N>(&d[0], d.size()/N, l);
        else
            switcher<N/2>(v, d, l);
    }

    template <int N>
    void count(const float * v, std::size_t c, std::vector<float> & l)
    {
        for (std::size_t i = 0; i < c; i++)
        {
            float sum = 0;
            for (int j = 0; j < N; j++)
                sum += v[i*N+j] * v[i*N+j];
            l.push_back(sqrt(sum));
        }
    }

    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const VecTask::g_type::container_type &d = static_cast<VecTask::g_type&>(td).get_const();
        std::unique_ptr<VecResult> res(new VecResult(*r.get(), td.get_n()));
        res->lengths.reserve(VecTask::num_vectors);
        D() << "vector: " << d;
        switcher<VecTask::max_vec_len>(td.get_n()/VecTask::num_vectors, d, res->lengths);
        D() << "lengths: " << res->lengths;
        r = std::move(res);
    }
};

template <>
inline void vec_loop::switcher<1>(int, const VecTask::g_type::container_type &, std::vector<float> &)
{
    throw BException("unsupported vector length " + get_name());
}

class vec_handmade : public Algorithm
{
public:
    vec_handmade()
        : Algorithm("handmade unrolling")
    {}

private:
    template <int N>
    inline void switcher(int v, const VecTask::g_type::container_type &d, std::vector<float> & l)
    {
        if (v == N)
            count<N>(&d[0], d.size()/N, l);
        else
            switcher<N/2>(v, d, l);
    }

    template <int N>
    void count(const float *, std::size_t, std::vector<float> &)
    {
        throw BException("unsupported vector length" + get_name());
    }

    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const VecTask::g_type::container_type &d = static_cast<VecTask::g_type&>(td).get_const();
        std::unique_ptr<VecResult> res(new VecResult(*r.get(), td.get_n()));
        res->lengths.reserve(VecTask::num_vectors);
        D() << "vector: " << d;
        switcher<VecTask::max_vec_len>(td.get_n()/VecTask::num_vectors, d, res->lengths);
        D() << "lengths: " << res->lengths;
        r = std::move(res);
    }
};

template <>
inline void vec_handmade::switcher<1>(int, const VecTask::g_type::container_type &, std::vector<float> &)
{
    throw BException("unsupported vector length" + get_name());
}

template<> void vec_handmade::count<2>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 2;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum = v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1];
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<4>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 4;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum = v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3];
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<8>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 8;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum =
                v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3] +
                v[i*N+4]*v[i*N+4] + v[i*N+5]*v[i*N+5] + v[i*N+6]*v[i*N+6] + v[i*N+7]*v[i*N+7]
                ;
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<16>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 16;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum =
                v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3] +
                v[i*N+4]*v[i*N+4] + v[i*N+5]*v[i*N+5] + v[i*N+6]*v[i*N+6] + v[i*N+7]*v[i*N+7] +
                v[i*N+ 8]*v[i*N+ 8] + v[i*N+ 9]*v[i*N+ 9] + v[i*N+10]*v[i*N+10] + v[i*N+11]*v[i*N+11] +
                v[i*N+12]*v[i*N+12] + v[i*N+13]*v[i*N+13] + v[i*N+14]*v[i*N+14] + v[i*N+15]*v[i*N+15]
                ;
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<32>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 32;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum =
                v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3] +
                v[i*N+4]*v[i*N+4] + v[i*N+5]*v[i*N+5] + v[i*N+6]*v[i*N+6] + v[i*N+7]*v[i*N+7] +
                v[i*N+ 8]*v[i*N+ 8] + v[i*N+ 9]*v[i*N+ 9] + v[i*N+10]*v[i*N+10] + v[i*N+11]*v[i*N+11] +
                v[i*N+12]*v[i*N+12] + v[i*N+13]*v[i*N+13] + v[i*N+14]*v[i*N+14] + v[i*N+15]*v[i*N+15] +
                v[i*N+16]*v[i*N+16] + v[i*N+17]*v[i*N+17] + v[i*N+18]*v[i*N+18] + v[i*N+19]*v[i*N+19] +
                v[i*N+20]*v[i*N+20] + v[i*N+21]*v[i*N+21] + v[i*N+22]*v[i*N+22] + v[i*N+23]*v[i*N+23] +
                v[i*N+24]*v[i*N+24] + v[i*N+25]*v[i*N+25] + v[i*N+26]*v[i*N+26] + v[i*N+27]*v[i*N+27] +
                v[i*N+28]*v[i*N+28] + v[i*N+29]*v[i*N+29] + v[i*N+30]*v[i*N+30] + v[i*N+31]*v[i*N+31]
                ;
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<64>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 64;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum =
                v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3] +
                v[i*N+4]*v[i*N+4] + v[i*N+5]*v[i*N+5] + v[i*N+6]*v[i*N+6] + v[i*N+7]*v[i*N+7] +
                v[i*N+8]*v[i*N+8] + v[i*N+9]*v[i*N+9] + v[i*N+10]*v[i*N+10] + v[i*N+11]*v[i*N+11] +
                v[i*N+12]*v[i*N+12] + v[i*N+13]*v[i*N+13] + v[i*N+14]*v[i*N+14] + v[i*N+15]*v[i*N+15] +
                v[i*N+16]*v[i*N+16] + v[i*N+17]*v[i*N+17] + v[i*N+18]*v[i*N+18] + v[i*N+19]*v[i*N+19] +
                v[i*N+20]*v[i*N+20] + v[i*N+21]*v[i*N+21] + v[i*N+22]*v[i*N+22] + v[i*N+23]*v[i*N+23] +
                v[i*N+24]*v[i*N+24] + v[i*N+25]*v[i*N+25] + v[i*N+26]*v[i*N+26] + v[i*N+27]*v[i*N+27] +
                v[i*N+28]*v[i*N+28] + v[i*N+29]*v[i*N+29] + v[i*N+30]*v[i*N+30] + v[i*N+31]*v[i*N+31] +
                v[i*N+32]*v[i*N+32] + v[i*N+33]*v[i*N+33] + v[i*N+34]*v[i*N+34] + v[i*N+35]*v[i*N+35] +
                v[i*N+36]*v[i*N+36] + v[i*N+37]*v[i*N+37] + v[i*N+38]*v[i*N+38] + v[i*N+39]*v[i*N+39] +
                v[i*N+40]*v[i*N+40] + v[i*N+41]*v[i*N+41] + v[i*N+42]*v[i*N+42] + v[i*N+43]*v[i*N+43] +
                v[i*N+44]*v[i*N+44] + v[i*N+45]*v[i*N+45] + v[i*N+46]*v[i*N+46] + v[i*N+47]*v[i*N+47] +
                v[i*N+48]*v[i*N+48] + v[i*N+49]*v[i*N+49] + v[i*N+50]*v[i*N+50] + v[i*N+51]*v[i*N+51] +
                v[i*N+52]*v[i*N+52] + v[i*N+53]*v[i*N+53] + v[i*N+54]*v[i*N+54] + v[i*N+55]*v[i*N+55] +
                v[i*N+56]*v[i*N+56] + v[i*N+57]*v[i*N+57] + v[i*N+58]*v[i*N+58] + v[i*N+59]*v[i*N+59] +
                v[i*N+60]*v[i*N+60] + v[i*N+61]*v[i*N+61] + v[i*N+62]*v[i*N+62] + v[i*N+63]*v[i*N+63]
                ;
        l.push_back(sqrt(sum));
    }
}

template<> void vec_handmade::count<128>(const float * v, std::size_t c, std::vector<float> & l)
{
    constexpr int N = 128;
    for (std::size_t i = 0; i < c; i++)
    {
        float sum =
                v[i*N+0]*v[i*N+0] + v[i*N+1]*v[i*N+1] + v[i*N+2]*v[i*N+2] + v[i*N+3]*v[i*N+3] +
                v[i*N+4]*v[i*N+4] + v[i*N+5]*v[i*N+5] + v[i*N+6]*v[i*N+6] + v[i*N+7]*v[i*N+7] +
                v[i*N+8]*v[i*N+8] + v[i*N+9]*v[i*N+9] + v[i*N+10]*v[i*N+10] + v[i*N+11]*v[i*N+11] +
                v[i*N+12]*v[i*N+12] + v[i*N+13]*v[i*N+13] + v[i*N+14]*v[i*N+14] + v[i*N+15]*v[i*N+15] +
                v[i*N+16]*v[i*N+16] + v[i*N+17]*v[i*N+17] + v[i*N+18]*v[i*N+18] + v[i*N+19]*v[i*N+19] +
                v[i*N+20]*v[i*N+20] + v[i*N+21]*v[i*N+21] + v[i*N+22]*v[i*N+22] + v[i*N+23]*v[i*N+23] +
                v[i*N+24]*v[i*N+24] + v[i*N+25]*v[i*N+25] + v[i*N+26]*v[i*N+26] + v[i*N+27]*v[i*N+27] +
                v[i*N+28]*v[i*N+28] + v[i*N+29]*v[i*N+29] + v[i*N+30]*v[i*N+30] + v[i*N+31]*v[i*N+31] +
                v[i*N+32]*v[i*N+32] + v[i*N+33]*v[i*N+33] + v[i*N+34]*v[i*N+34] + v[i*N+35]*v[i*N+35] +
                v[i*N+36]*v[i*N+36] + v[i*N+37]*v[i*N+37] + v[i*N+38]*v[i*N+38] + v[i*N+39]*v[i*N+39] +
                v[i*N+40]*v[i*N+40] + v[i*N+41]*v[i*N+41] + v[i*N+42]*v[i*N+42] + v[i*N+43]*v[i*N+43] +
                v[i*N+44]*v[i*N+44] + v[i*N+45]*v[i*N+45] + v[i*N+46]*v[i*N+46] + v[i*N+47]*v[i*N+47] +
                v[i*N+48]*v[i*N+48] + v[i*N+49]*v[i*N+49] + v[i*N+50]*v[i*N+50] + v[i*N+51]*v[i*N+51] +
                v[i*N+52]*v[i*N+52] + v[i*N+53]*v[i*N+53] + v[i*N+54]*v[i*N+54] + v[i*N+55]*v[i*N+55] +
                v[i*N+56]*v[i*N+56] + v[i*N+57]*v[i*N+57] + v[i*N+58]*v[i*N+58] + v[i*N+59]*v[i*N+59] +
                v[i*N+60]*v[i*N+60] + v[i*N+61]*v[i*N+61] + v[i*N+62]*v[i*N+62] + v[i*N+63]*v[i*N+63] +
                v[i*N+64]*v[i*N+64] + v[i*N+65]*v[i*N+65] + v[i*N+66]*v[i*N+66] + v[i*N+67]*v[i*N+67] +
                v[i*N+68]*v[i*N+68] + v[i*N+69]*v[i*N+69] + v[i*N+70]*v[i*N+70] + v[i*N+71]*v[i*N+71] +
                v[i*N+72]*v[i*N+72] + v[i*N+73]*v[i*N+73] + v[i*N+74]*v[i*N+74] + v[i*N+75]*v[i*N+75] +
                v[i*N+76]*v[i*N+76] + v[i*N+77]*v[i*N+77] + v[i*N+78]*v[i*N+78] + v[i*N+79]*v[i*N+79] +
                v[i*N+80]*v[i*N+80] + v[i*N+81]*v[i*N+81] + v[i*N+82]*v[i*N+82] + v[i*N+83]*v[i*N+83] +
                v[i*N+84]*v[i*N+84] + v[i*N+85]*v[i*N+85] + v[i*N+86]*v[i*N+86] + v[i*N+87]*v[i*N+87] +
                v[i*N+88]*v[i*N+88] + v[i*N+89]*v[i*N+89] + v[i*N+90]*v[i*N+90] + v[i*N+91]*v[i*N+91] +
                v[i*N+92]*v[i*N+92] + v[i*N+93]*v[i*N+93] + v[i*N+94]*v[i*N+94] + v[i*N+95]*v[i*N+95] +
                v[i*N+96]*v[i*N+96] + v[i*N+97]*v[i*N+97] + v[i*N+98]*v[i*N+98] + v[i*N+99]*v[i*N+99] +
                v[i*N+100]*v[i*N+100] + v[i*N+101]*v[i*N+101] + v[i*N+102]*v[i*N+102] + v[i*N+103]*v[i*N+103] +
                v[i*N+104]*v[i*N+104] + v[i*N+105]*v[i*N+105] + v[i*N+106]*v[i*N+106] + v[i*N+107]*v[i*N+107] +
                v[i*N+108]*v[i*N+108] + v[i*N+109]*v[i*N+109] + v[i*N+110]*v[i*N+110] + v[i*N+111]*v[i*N+111] +
                v[i*N+112]*v[i*N+112] + v[i*N+113]*v[i*N+113] + v[i*N+114]*v[i*N+114] + v[i*N+115]*v[i*N+115] +
                v[i*N+116]*v[i*N+116] + v[i*N+117]*v[i*N+117] + v[i*N+118]*v[i*N+118] + v[i*N+119]*v[i*N+119] +
                v[i*N+120]*v[i*N+120] + v[i*N+121]*v[i*N+121] + v[i*N+122]*v[i*N+122] + v[i*N+123]*v[i*N+123] +
                v[i*N+124]*v[i*N+124] + v[i*N+125]*v[i*N+125] + v[i*N+126]*v[i*N+126] + v[i*N+127]*v[i*N+127]
                ;
        l.push_back(sqrt(sum));
    }
}

#endif // VEC_ALGS_H
