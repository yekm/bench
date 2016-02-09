/// @file 
/// @brief Объявление функций реализующих преобразования из алгоритма "Кузнечик"
///
/// @copyright InfoTeCS. All rights reserved.

#ifndef C_28147_14_H
#define C_28147_14_H

void ExpandKey(const unsigned char* masterKey, unsigned char* keys);

void Encrypt_14(const unsigned char* plainText, unsigned char* chipherText, unsigned int lenPlainText, unsigned char* keys);

void Encrypt_14_fast(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys);
void Encrypt_14_fast2(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys);
void Encrypt_14_fast3(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys);

void Decrypt_14(const unsigned char* chipherText, unsigned char* plainText, unsigned int lenPlainText, unsigned char* keys);

void Decrypt_14_fast(const unsigned char* chipherText, unsigned char* plainText, unsigned char* keys);
//void Decrypt_14_fast2(const unsigned char* chipherText, unsigned char* plainText, unsigned char* keys);
void Decrypt_14_fast3(const unsigned char* chipherText, unsigned char* plainText, unsigned char* keys);

#endif
