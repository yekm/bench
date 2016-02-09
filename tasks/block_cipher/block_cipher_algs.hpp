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
}

#include <string>
#include <vector>

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

    virtual void cipher(std::vector<data_type> & data)
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

class KuzSSE4 : public BlockCipher<kuz_key_t>
{
public:
    KuzSSE4()
        : BlockCipher("GOST 34.12-2015 'Kuznechik' encryption by Markku-Juhani O. Saarinen SEE4",
                      32, 16)
    {}
private:
    virtual void do_prepare(const TaskData &) override
    {
        kuz_init_sse4();
        kuz_set_encrypt_key_sse4(&m_ctx, m_key.data());
    }

    virtual void do_block(data_type * data) override
    {
        kuz_encrypt_block_sse4(&m_ctx, data);
    }
};

template <int N>
class AES : public BlockCipher<rijndael_ctx>
{
public:
    AES()
        : BlockCipher("AES encryption from openssh",
                      N, 16)
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


#endif // BLOCK_CIPHER_ALGS_H
