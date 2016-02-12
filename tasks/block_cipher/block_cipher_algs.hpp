#ifndef BLOCK_CIPHER_ALGS_H
#define BLOCK_CIPHER_ALGS_H

#include "algorithm.hpp"
#include "block_cipher_task.hpp"
#include "common/randomdata.hpp"
//#include "utils/dbg.hpp"

extern "C"
{
#include "ciphers/gost89.h"
#include "ciphers/kuznechik.h"
#include "ciphers/rijndael.h"
#include "ciphers/28147_14.h"
#include "ciphers/ecrypt-sync.h"
}

#include <string>
#include <vector>

#if defined(__arm__)

#include <arm_neon.h>
typedef uint8x16_t __m128i;

#elif defined (__SSE4_1__)

#include <mmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

#endif


template <typename T>
class BlockCipher : public Algorithm
{
public:
    typedef T ctx_type;
    typedef unsigned char data_type;

    BlockCipher(std::string name, int keylen, int blocklen)
        : Algorithm(name + ", " +
                    std::to_string(keylen) + " bytes key, " +
                    std::to_string(blocklen) + " bytes block.")
        , m_keylen(keylen)
        , m_blocklen(blocklen)
    {
        m_key = common::RandomData<data_type>(m_keylen).get_const();
    }
protected:
    std::vector<data_type> m_key;
    size_t m_keylen, m_blocklen;
    T m_ctx;

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        block_cipher::g_type::container_type &d =
                static_cast<block_cipher::g_type&>(td).get_mutable();
        cipher(d);
    }

    virtual void cipher(block_cipher::g_type::container_type & data)
    {
        int blocks = data.size() / m_blocklen;
        data_type * in = data.data();
        for (int i=0; i<blocks; i++)
        {
            do_block(in);
            in += m_blocklen;
        }
    }

    virtual void do_block(data_type * data) = 0;
};

class Gost89 : public BlockCipher<gost_ctx>
{
public:
    Gost89()
        : BlockCipher("GOST 28147-89 encryption from openssl",
                      32, 8)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        gost_init(&m_ctx, NULL);
        gost_key(&m_ctx, m_key.data());
    }

    virtual void do_block(data_type * data) override
    {
        gostcrypt(&m_ctx, data, data);
    }
};

#if 0
class KuzSimple : public BlockCipher<kuz_key_t>
{
public:
    KuzSimple()
        : BlockCipher("GOST 34.12-2015 'Kuznechik' encryption by Markku-Juhani O. Saarinen",
                      32, 16)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        kuz_set_encrypt_key(&m_ctx, m_key.data());
    }

    virtual void do_block(data_type * data) override
    {
        kuz_encrypt_block(&m_ctx, data);
    }
};
#endif

class KuzSIMD : public BlockCipher<kuz_key_t>
{
public:
    KuzSIMD()
        : BlockCipher("GOST 34.12-2015 'Kuznechik' encryption by Markku-Juhani O. Saarinen SIMD SSE4/NEON",
                      32, 16)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        kuz_init_tables();
        kuz_set_encrypt_key(&m_ctx, m_key.data());
    }

    virtual void do_block(data_type * data) override
    {
        kuz_encrypt_block(&m_ctx, data);
    }
};

template <int KL, int BL=16>
class AES : public BlockCipher<rijndael_ctx>
{
public:
    AES()
        : BlockCipher("AES encryption from openssh",
                      KL, BL)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        m_ctx.Nr = rijndaelKeySetupEnc(m_ctx.ek, m_key.data(), m_keylen*8);
    }

    virtual void do_block(data_type * data) override
    {
        rijndaelEncrypt(m_ctx.ek, m_ctx.Nr, data, data);
    }
};

template <int KL, int BL=16>
class Salsa20 : public BlockCipher<ECRYPT_ctx>
{
public:
    Salsa20()
        : BlockCipher("Salsa20 'merged' version from djb",
                      KL, BL)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        ECRYPT_keysetup(&m_ctx, m_key.data(), m_keylen*8, 0);
        ECRYPT_ivsetup(&m_ctx, m_key.data());
    }

    virtual void do_block(data_type * data) override
    {
        ECRYPT_encrypt_bytes(&m_ctx, data, data, m_blocklen);
    }
};

class KuzTC26_fast3 : public BlockCipher<unsigned char[10*16]>
{
public:
    KuzTC26_fast3()
        : BlockCipher("GOST 34.12-2015 'Kuznechik' encryption from TC26 unrolled version",
                      32, 16)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        ExpandKey(m_key.data(), m_ctx);
    }

    virtual void do_block(data_type * data) override
    {
        Encrypt_14_fast3(data, data, m_ctx);
    }
};

class KuzTC26 : public BlockCipher<unsigned char[10*16]>
{
public:
    KuzTC26()
        : BlockCipher("GOST 34.12-2015 'Kuznechik' encryption from TC26",
                      32, 16)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        ExpandKey(m_key.data(), m_ctx);
    }

    virtual void do_block(data_type * data) override
    {
        Encrypt_14_fast(data, data, m_ctx);
    }
};

class PerbyteXOR : public BlockCipher<bool>
{
public:
    PerbyteXOR()
        : BlockCipher("Per-byte XOR", 16, 16)
    {}
private:
    virtual void do_block(data_type * data) override
    {
        for (size_t i=0; i<m_keylen; i++)
            data[i] ^= m_key.at(i);
    }
};

class SIMDXOR : public BlockCipher<bool>
{
public:
    SIMDXOR()
        : BlockCipher("SSE XOR", 16, 16)
    {}
private:
    virtual void do_block(data_type * data) override
    {
        *((__m128i *) data) ^= *((__m128i *) m_key.data());
    }
};


#endif // BLOCK_CIPHER_ALGS_H
