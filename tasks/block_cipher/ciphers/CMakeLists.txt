cmake_minimum_required(VERSION 2.8)

set(CIPHERS_SRC
    gost89.c
    kuznechik_128bit.c
    kuznechik_8bit.c
    rijndael.c
    28147_14.c
)

add_library(CIPHERS OBJECT ${CIPHERS_SRC})
