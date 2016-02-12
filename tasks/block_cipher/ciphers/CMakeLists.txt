cmake_minimum_required(VERSION 2.8)

#include_directories(.)

set(CIPHERS_SRC
    gost89.c
    kuznechik.c
    rijndael.c
    28147_14.c
    salsa20.c
)

add_library(CIPHERS OBJECT ${CIPHERS_SRC})
