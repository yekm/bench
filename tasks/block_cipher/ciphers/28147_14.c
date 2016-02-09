/// @file
/// @brief Реализация алгоритма "Кузнечик"
///
/// @copyright InfoTeCS. All rights reserved.

#include <memory.h>

#include "28147_14.h"
#include "table.h"

/// @brief Нелинейное биективное преобразование множества двоичных векторов.
static const unsigned char kPi[256] =
{
	252, 238, 221,  17, 207, 110,  49,  22, 251, 196, 250, 218,  35, 197,   4,  77,
	233, 119, 240, 219, 147,  46, 153, 186,  23,  54, 241, 187,  20, 205,  95, 193,
	249,  24, 101,  90, 226,  92, 239,  33, 129,  28,  60,  66,	139,   1, 142,  79,
	5, 132,   2, 174, 227, 106, 143, 160,   6,  11, 237, 152, 127, 212, 211,  31,
	235,  52,  44,  81,	234, 200,  72, 171, 242,  42, 104, 162, 253,  58, 206, 204,
	181, 112,  14,  86,   8,  12, 118,  18, 191, 114,  19,  71, 156, 183,  93, 135,
	21, 161, 150,  41,  16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
	50, 117,  25,  61, 255,  53, 138, 126, 109,  84, 198, 128, 195, 189,  13,  87,
	223, 245,  36, 169,  62, 168,  67, 201, 215, 121, 214, 246, 124,  34, 185,   3,
	224,  15, 236, 222, 122, 148, 176, 188, 220, 232,  40,  80,  78,  51,  10,  74,
	167, 151,  96, 115,  30,   0,  98,  68,  26, 184,  56, 130, 100, 159,  38,  65,
	173,  69,  70, 146,  39,  94,  85,  47, 140, 163, 165, 125, 105, 213, 149,  59,
	7,  88, 179,  64, 134, 172,  29, 247,  48,  55, 107, 228,	136, 217, 231, 137,
	225,  27, 131,  73,  76,  63, 248, 254, 141,  83, 170, 144, 202, 216, 133,  97,
	32, 113, 103, 164,  45,  43,   9,  91, 203, 155,  37, 208, 190, 229, 108,  82,
	89, 166, 116, 210, 230, 244, 180, 192,	209, 102, 175, 194,  57,  75,  99, 182
};

/// @brief Обратное нелинейное биективное преобразование множества двоичных векторов.
static const unsigned char kReversePi[256] =
{
	0xa5,0x2d,0x32,0x8f,0x0e,0x30,0x38,0xc0,0x54,0xe6,0x9e,0x39,0x55,0x7e,0x52,0x91,
	0x64,0x03,0x57,0x5a,0x1c,0x60,0x07,0x18,0x21,0x72,0xa8,0xd1,0x29,0xc6,0xa4,0x3f,
	0xe0,0x27,0x8d,0x0c,0x82,0xea,0xae,0xb4,0x9a,0x63,0x49,0xe5,0x42,0xe4,0x15,0xb7,
	0xc8,0x06,0x70,0x9d,0x41,0x75,0x19,0xc9,0xaa,0xfc,0x4d,0xbf,0x2a,0x73,0x84,0xd5,
	0xc3,0xaf,0x2b,0x86,0xa7,0xb1,0xb2,0x5b,0x46,0xd3,0x9f,0xfd,0xd4,0x0f,0x9c,0x2f,
	0x9b,0x43,0xef,0xd9,0x79,0xb6,0x53,0x7f,0xc1,0xf0,0x23,0xe7,0x25,0x5e,0xb5,0x1e,
	0xa2,0xdf,0xa6,0xfe,0xac,0x22,0xf9,0xe2,0x4a,0xbc,0x35,0xca,0xee,0x78,0x05,0x6b,
	0x51,0xe1,0x59,0xa3,0xf2,0x71,0x56,0x11,0x6a,0x89,0x94,0x65,0x8c,0xbb,0x77,0x3c,
	0x7b,0x28,0xab,0xd2,0x31,0xde,0xc4,0x5f,0xcc,0xcf,0x76,0x2c,0xb8,0xd8,0x2e,0x36,
	0xdb,0x69,0xb3,0x14,0x95,0xbe,0x62,0xa1,0x3b,0x16,0x66,0xe9,0x5c,0x6c,0x6d,0xad,
	0x37,0x61,0x4b,0xb9,0xe3,0xba,0xf1,0xa0,0x85,0x83,0xda,0x47,0xc5,0xb0,0x33,0xfa,
	0x96,0x6f,0x6e,0xc2,0xf6,0x50,0xff,0x5d,0xa9,0x8e,0x17,0x1b,0x97,0x7d,0xec,0x58,
	0xf7,0x1f,0xfb,0x7c,0x09,0x0d,0x7a,0x67,0x45,0x87,0xdc,0xe8,0x4f,0x1d,0x4e,0x04,
	0xeb,0xf8,0xf3,0x3e,0x3d,0xbd,0x8a,0x88,0xdd,0xcd,0x0b,0x13,0x98,0x02,0x93,0x80,
	0x90,0xd0,0x24,0x34,0xcb,0xed,0xf4,0xce,0x99,0x10,0x44,0x40,0x92,0x3a,0x01,0x26,
	0x12,0x1a,0x48,0x68,0xf5,0x81,0x8b,0xc7,0xd6,0x20,0x0a,0x08,0x00,0x4c,0xd7,0x74
};

/// @brief Коэфициенты умножения в приобразовании l
static const  unsigned char kB[16] = {148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148, 1};

void funcX(unsigned char* a, unsigned char* b, unsigned char* outdata)
{
	unsigned int i;

	for(i = 0; i < 16; ++i)
	{
		outdata[i] = a[i] ^ b[i];
	}
}

void funcS(unsigned char* indata, unsigned char* outdata)
{
	size_t i = 0;

	for(i = 0; i < 16; ++i)
	{
		outdata[i] = kPi[indata[i]];
	}
}

void funcReverseS(unsigned char* indata, unsigned char*  outdata)
{
	unsigned int i;

	for(i = 0; i < 16; ++i)
	{
		outdata[i] = kReversePi[indata[i]];
	}
}

void funcR(unsigned char* indata, unsigned char* outdata)
{
	size_t i;
	unsigned char sum = 0;

	for(i = 0; i < 16; ++i)
	{
		sum ^= multTable[indata[i]*256 + kB[i]];
	}

	outdata[0] = sum;
	memcpy(outdata+1, indata, 15);
}

void funcReverseR(unsigned char* indata, unsigned char* outdata)
{
	unsigned char tmp[16] = {0};
	unsigned char sum = 0;
	unsigned int i;

	memcpy(tmp, indata+1, 15);
	tmp[15] = indata[0];


	for(i = 0; i < 16; ++i)
	{
		sum ^= multTable[tmp[i]*256 + kB[i]];
	}

	memcpy(outdata, tmp, 15);
	outdata[15] = sum;
}

void funcL(unsigned char* indata, unsigned char* outdata)
{
	unsigned char tmp[16];
	int i = 0;

	memcpy(tmp, indata, 16);

	for(i = 0; i < 16; ++i)
	{
		funcR(tmp, outdata);
		memcpy(tmp, outdata, 16);
	}
}

void funcReverseL(unsigned char* indata, unsigned char* outdata)
{
	unsigned char tmp[16];
	unsigned int i;

	memcpy(tmp, indata, 16);

	for(i = 0; i < 16; ++i)
	{
		funcReverseR(tmp, outdata);
		memcpy(tmp, outdata, 16);
	}
}

void funcLSX(unsigned char* a, unsigned char* b, unsigned char* outdata)
{
	unsigned char temp1[16] = {0};
	unsigned char temp2[16] = {0};

	funcX(a, b, temp1);
	funcS(temp1, temp2);
	funcL(temp2, outdata);
}

void funcReverseLSX(unsigned char* a, unsigned char* b, unsigned char* outdata)
{
	unsigned char temp1[16] = {0};
	unsigned char temp2[16] = {0};

	funcX(a, b, temp1);
	funcReverseL(temp1, temp2);
	funcReverseS(temp2, outdata);
}

void funcF(unsigned char* inputKey, unsigned char* inputKeySecond, unsigned char* iterationConst, unsigned char* outputKey, unsigned char* outputKeySecond)
{
	unsigned char temp1[16] = {0};
	unsigned char temp2[16] = {0};

	funcLSX(inputKey, iterationConst, temp1);
	funcX(temp1, inputKeySecond, temp2);

	memcpy(outputKeySecond, inputKey, 16);
	memcpy(outputKey, temp2, 16);
}

void funcC(unsigned char number, unsigned char* output)
{
	unsigned char tempI[16] = {0};
	tempI[15] = number;
	funcL(tempI, output);
}

void ExpandKey(const unsigned char* masterKey, unsigned char* keys)
{
	unsigned char C[16] = {0};
	unsigned char temp1[16] = {0};
	unsigned char temp2[16] = {0};
	unsigned char j, i;

	memcpy(keys, masterKey, 16);
	memcpy(keys + 16, masterKey + 16, 16);


	for(j = 0; j < 4; ++j)
	{
		memcpy(temp1, keys + j * 2 * 16, 16);
		memcpy(temp2, keys + (j * 2 + 1) * 16, 16);

		for( i = 1; i < 8; ++i )
		{
			funcC(j*8+i, C);
			funcF(temp1, temp2, C, temp1, temp2);
		}

		funcC(j*8+8, C);
		funcF(temp1, temp2, C, temp1, temp2);

		memcpy(keys + (j * 2 + 2) * 16, temp1, 16);
		memcpy(keys + (j * 2 + 3) * 16, temp2, 16);

	}
}

void Encrypt_14(const unsigned char* plainText, unsigned char* chipherText, unsigned int lenPlainText, unsigned char* keys)
{
	unsigned char xTemp[16] = {0};
	unsigned char yTemp[16] = {0};
	int i;

	memcpy(xTemp, plainText, 16);

	for(i = 0; i < 9; ++i)
	{
		funcLSX(xTemp, keys + 16*i, yTemp);
		memcpy(xTemp, yTemp, 16);
	}
	funcX(yTemp, keys+9*16, chipherText);
}

void Decrypt_14(const unsigned char* chipherText, unsigned char* plainText, unsigned int lenPlainText, unsigned char* keys)
{
	unsigned char xTemp[16] = {0};
	unsigned char yTemp[16] = {0};
	int i;

	memcpy(xTemp, chipherText, 16);
	for(i = 0; i < 9; ++i)
	{
		funcReverseLSX(xTemp, keys+(9-i)*16, yTemp);
		memcpy(xTemp, yTemp, 16);
	}
	funcX(yTemp, keys, plainText);
}

void Decrypt_14_fast(const unsigned char* chipherText, unsigned char* plainText, unsigned char* keys)
{
	unsigned char d1[16+16] = {0};

	memcpy(d1, chipherText, 16);

	int i;
	for(i = 0; i < 9; ++i)
	{
		unsigned char* b = keys+(9-i)*16;
		int xi;
		for(xi = 0; xi < 16; ++xi)
		{
			d1[xi] = d1[xi] ^ b[xi];
		}

		for(xi = 0; xi < 16; ++xi)
		{
			unsigned char sum = 0;

			d1[16+xi] = d1[xi];

			int ri;
			for(ri = 0; ri < 16; ++ri)
			{
				sum ^= multTable[d1[ri+xi+1]*256 + kB[ri]];
			}
			d1[16+xi] = sum;
		}

		for(xi = 0; xi < 16; ++xi)
		{
			d1[xi] = kReversePi[d1[16+xi]];
		}
	}
	for(i = 0; i < 16; ++i)
	{
		plainText[i] = d1[i] ^ keys[i];
	}
}

static inline unsigned char g(unsigned char e, unsigned char k)
{
	return multTable[ e*256 + kB[k] ];
}

void Decrypt_14_fast3(const unsigned char* chipherText, unsigned char* plainText, unsigned char* keys)
{
	unsigned char d1[16] = {0};

	memcpy(d1, chipherText, 16);

	int i;
	for(i = 0; i < 9; ++i)
	{
		unsigned char* b = keys+(9-i)*16;
		int xi;
		for(xi = 0; xi < 16; ++xi)
		{
			d1[xi] = d1[xi] ^ b[xi];
		}

		unsigned char sum = 0;

		sum  = g(d1[ 0], 15);
		sum ^= g(d1[ 1],  0);
		sum ^= g(d1[ 2],  1);
		sum ^= g(d1[ 3],  2);
		sum ^= g(d1[ 4],  3);
		sum ^= g(d1[ 5],  4);
		sum ^= g(d1[ 6],  5);
		sum ^= g(d1[ 7],  6);
		sum ^= g(d1[ 8],  7);
		sum ^= g(d1[ 9],  8);
		sum ^= g(d1[10],  9);
		sum ^= g(d1[11], 10);
		sum ^= g(d1[12], 11);
		sum ^= g(d1[13], 12);
		sum ^= g(d1[14], 13);
		sum ^= g(d1[15], 14);
		d1[0] = sum;

		sum  = g(d1[ 0], 14);
		sum ^= g(d1[ 1], 15);
		sum ^= g(d1[ 2],  0);
		sum ^= g(d1[ 3],  1);
		sum ^= g(d1[ 4],  2);
		sum ^= g(d1[ 5],  3);
		sum ^= g(d1[ 6],  4);
		sum ^= g(d1[ 7],  5);
		sum ^= g(d1[ 8],  6);
		sum ^= g(d1[ 9],  7);
		sum ^= g(d1[10],  8);
		sum ^= g(d1[11],  9);
		sum ^= g(d1[12], 10);
		sum ^= g(d1[13], 11);
		sum ^= g(d1[14], 12);
		sum ^= g(d1[15], 13);
		d1[1] = sum;

		sum  = g(d1[ 0], 13);
		sum ^= g(d1[ 1], 14);
		sum ^= g(d1[ 2], 15);
		sum ^= g(d1[ 3],  0);
		sum ^= g(d1[ 4],  1);
		sum ^= g(d1[ 5],  2);
		sum ^= g(d1[ 6],  3);
		sum ^= g(d1[ 7],  4);
		sum ^= g(d1[ 8],  5);
		sum ^= g(d1[ 9],  6);
		sum ^= g(d1[10],  7);
		sum ^= g(d1[11],  8);
		sum ^= g(d1[12],  9);
		sum ^= g(d1[13], 10);
		sum ^= g(d1[14], 11);
		sum ^= g(d1[15], 12);
		d1[2] = sum;

		sum  = g(d1[ 0], 12);
		sum ^= g(d1[ 1], 13);
		sum ^= g(d1[ 2], 14);
		sum ^= g(d1[ 3], 15);
		sum ^= g(d1[ 4],  0);
		sum ^= g(d1[ 5],  1);
		sum ^= g(d1[ 6],  2);
		sum ^= g(d1[ 7],  3);
		sum ^= g(d1[ 8],  4);
		sum ^= g(d1[ 9],  5);
		sum ^= g(d1[10],  6);
		sum ^= g(d1[11],  7);
		sum ^= g(d1[12],  8);
		sum ^= g(d1[13],  9);
		sum ^= g(d1[14], 10);
		sum ^= g(d1[15], 11);
		d1[3] = sum;

		sum  = g(d1[ 0], 11);
		sum ^= g(d1[ 1], 12);
		sum ^= g(d1[ 2], 13);
		sum ^= g(d1[ 3], 14);
		sum ^= g(d1[ 4], 15);
		sum ^= g(d1[ 5],  0);
		sum ^= g(d1[ 6],  1);
		sum ^= g(d1[ 7],  2);
		sum ^= g(d1[ 8],  3);
		sum ^= g(d1[ 9],  4);
		sum ^= g(d1[10],  5);
		sum ^= g(d1[11],  6);
		sum ^= g(d1[12],  7);
		sum ^= g(d1[13],  8);
		sum ^= g(d1[14],  9);
		sum ^= g(d1[15], 10);
		d1[4] = sum;

		sum  = g(d1[ 0], 10);
		sum ^= g(d1[ 1], 11);
		sum ^= g(d1[ 2], 12);
		sum ^= g(d1[ 3], 13);
		sum ^= g(d1[ 4], 14);
		sum ^= g(d1[ 5], 15);
		sum ^= g(d1[ 6],  0);
		sum ^= g(d1[ 7],  1);
		sum ^= g(d1[ 8],  2);
		sum ^= g(d1[ 9],  3);
		sum ^= g(d1[10],  4);
		sum ^= g(d1[11],  5);
		sum ^= g(d1[12],  6);
		sum ^= g(d1[13],  7);
		sum ^= g(d1[14],  8);
		sum ^= g(d1[15],  9);
		d1[5] = sum;

		sum  = g(d1[ 0],  9);
		sum ^= g(d1[ 1], 10);
		sum ^= g(d1[ 2], 11);
		sum ^= g(d1[ 3], 12);
		sum ^= g(d1[ 4], 13);
		sum ^= g(d1[ 5], 14);
		sum ^= g(d1[ 6], 15);
		sum ^= g(d1[ 7],  0);
		sum ^= g(d1[ 8],  1);
		sum ^= g(d1[ 9],  2);
		sum ^= g(d1[10],  3);
		sum ^= g(d1[11],  4);
		sum ^= g(d1[12],  5);
		sum ^= g(d1[13],  6);
		sum ^= g(d1[14],  7);
		sum ^= g(d1[15],  8);
		d1[6] = sum;

		sum  = g(d1[ 0],  8);
		sum ^= g(d1[ 1],  9);
		sum ^= g(d1[ 2], 10);
		sum ^= g(d1[ 3], 11);
		sum ^= g(d1[ 4], 12);
		sum ^= g(d1[ 5], 13);
		sum ^= g(d1[ 6], 14);
		sum ^= g(d1[ 7], 15);
		sum ^= g(d1[ 8],  0);
		sum ^= g(d1[ 9],  1);
		sum ^= g(d1[10],  2);
		sum ^= g(d1[11],  3);
		sum ^= g(d1[12],  4);
		sum ^= g(d1[13],  5);
		sum ^= g(d1[14],  6);
		sum ^= g(d1[15],  7);
		d1[7] = sum;

		sum  = g(d1[ 0],  7);
		sum ^= g(d1[ 1],  8);
		sum ^= g(d1[ 2],  9);
		sum ^= g(d1[ 3], 10);
		sum ^= g(d1[ 4], 11);
		sum ^= g(d1[ 5], 12);
		sum ^= g(d1[ 6], 13);
		sum ^= g(d1[ 7], 14);
		sum ^= g(d1[ 8], 15);
		sum ^= g(d1[ 9],  0);
		sum ^= g(d1[10],  1);
		sum ^= g(d1[11],  2);
		sum ^= g(d1[12],  3);
		sum ^= g(d1[13],  4);
		sum ^= g(d1[14],  5);
		sum ^= g(d1[15],  6);
		d1[8] = sum;

		sum  = g(d1[ 0],  6);
		sum ^= g(d1[ 1],  7);
		sum ^= g(d1[ 2],  8);
		sum ^= g(d1[ 3],  9);
		sum ^= g(d1[ 4], 10);
		sum ^= g(d1[ 5], 11);
		sum ^= g(d1[ 6], 12);
		sum ^= g(d1[ 7], 13);
		sum ^= g(d1[ 8], 14);
		sum ^= g(d1[ 9], 15);
		sum ^= g(d1[10],  0);
		sum ^= g(d1[11],  1);
		sum ^= g(d1[12],  2);
		sum ^= g(d1[13],  3);
		sum ^= g(d1[14],  4);
		sum ^= g(d1[15],  5);
		d1[9] = sum;

		sum  = g(d1[ 0],  5);
		sum ^= g(d1[ 1],  6);
		sum ^= g(d1[ 2],  7);
		sum ^= g(d1[ 3],  8);
		sum ^= g(d1[ 4],  9);
		sum ^= g(d1[ 5], 10);
		sum ^= g(d1[ 6], 11);
		sum ^= g(d1[ 7], 12);
		sum ^= g(d1[ 8], 13);
		sum ^= g(d1[ 9], 14);
		sum ^= g(d1[10], 15);
		sum ^= g(d1[11],  0);
		sum ^= g(d1[12],  1);
		sum ^= g(d1[13],  2);
		sum ^= g(d1[14],  3);
		sum ^= g(d1[15],  4);
		d1[10] = sum;

		sum  = g(d1[ 0],  4);
		sum ^= g(d1[ 1],  5);
		sum ^= g(d1[ 2],  6);
		sum ^= g(d1[ 3],  7);
		sum ^= g(d1[ 4],  8);
		sum ^= g(d1[ 5],  9);
		sum ^= g(d1[ 6], 10);
		sum ^= g(d1[ 7], 11);
		sum ^= g(d1[ 8], 12);
		sum ^= g(d1[ 9], 13);
		sum ^= g(d1[10], 14);
		sum ^= g(d1[11], 15);
		sum ^= g(d1[12],  0);
		sum ^= g(d1[13],  1);
		sum ^= g(d1[14],  2);
		sum ^= g(d1[15],  3);
		d1[11] = sum;

		sum  = g(d1[ 0],  3);
		sum ^= g(d1[ 1],  4);
		sum ^= g(d1[ 2],  5);
		sum ^= g(d1[ 3],  6);
		sum ^= g(d1[ 4],  7);
		sum ^= g(d1[ 5],  8);
		sum ^= g(d1[ 6],  9);
		sum ^= g(d1[ 7], 10);
		sum ^= g(d1[ 8], 11);
		sum ^= g(d1[ 9], 12);
		sum ^= g(d1[10], 13);
		sum ^= g(d1[11], 14);
		sum ^= g(d1[12], 15);
		sum ^= g(d1[13],  0);
		sum ^= g(d1[14],  1);
		sum ^= g(d1[15],  2);
		d1[12] = sum;

		sum  = g(d1[ 0],  2);
		sum ^= g(d1[ 1],  3);
		sum ^= g(d1[ 2],  4);
		sum ^= g(d1[ 3],  5);
		sum ^= g(d1[ 4],  6);
		sum ^= g(d1[ 5],  7);
		sum ^= g(d1[ 6],  8);
		sum ^= g(d1[ 7],  9);
		sum ^= g(d1[ 8], 10);
		sum ^= g(d1[ 9], 11);
		sum ^= g(d1[10], 12);
		sum ^= g(d1[11], 13);
		sum ^= g(d1[12], 14);
		sum ^= g(d1[13], 15);
		sum ^= g(d1[14],  0);
		sum ^= g(d1[15],  1);
		d1[13] = sum;

		sum  = g(d1[ 0],  1);
		sum ^= g(d1[ 1],  2);
		sum ^= g(d1[ 2],  3);
		sum ^= g(d1[ 3],  4);
		sum ^= g(d1[ 4],  5);
		sum ^= g(d1[ 5],  6);
		sum ^= g(d1[ 6],  7);
		sum ^= g(d1[ 7],  8);
		sum ^= g(d1[ 8],  9);
		sum ^= g(d1[ 9], 10);
		sum ^= g(d1[10], 11);
		sum ^= g(d1[11], 12);
		sum ^= g(d1[12], 13);
		sum ^= g(d1[13], 14);
		sum ^= g(d1[14], 15);
		sum ^= g(d1[15],  0);
		d1[14] = sum;

		sum  = g(d1[ 0],  0);
		sum ^= g(d1[ 1],  1);
		sum ^= g(d1[ 2],  2);
		sum ^= g(d1[ 3],  3);
		sum ^= g(d1[ 4],  4);
		sum ^= g(d1[ 5],  5);
		sum ^= g(d1[ 6],  6);
		sum ^= g(d1[ 7],  7);
		sum ^= g(d1[ 8],  8);
		sum ^= g(d1[ 9],  9);
		sum ^= g(d1[10], 10);
		sum ^= g(d1[11], 11);
		sum ^= g(d1[12], 12);
		sum ^= g(d1[13], 13);
		sum ^= g(d1[14], 14);
		sum ^= g(d1[15], 15);
		d1[15] = sum;

		for(xi = 0; xi < 16; ++xi)
		{
			d1[xi] = kReversePi[d1[xi]];
		}
	}
	for(i = 0; i < 16; ++i)
	{
		plainText[i] = d1[i] ^ keys[i];
	}
}

void Encrypt_14_fast(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys)
{
	unsigned char e1[16+16] = {0};

	memcpy(e1, plainText, 16);

	int i;
	for(i = 0; i < 9; ++i)
	{
		unsigned char* b = keys + 16*i;

		int xi;
		for(xi = 0; xi < 16; ++xi)
		{
			(e1+16)[xi] = kPi[ e1[xi] ^ b[xi] ];
		}

		for(xi = 0; xi < 16; ++xi)
		{
			unsigned char sum = 0;

			int ri;
			for(ri = 0; ri < 16; ++ri)
			{
				sum ^= multTable[ e1[ri-xi+16]*256 + kB[ri] ];
			}
			e1[15-xi] = sum;
		}
	}
	unsigned char* b = keys+9*16;
	for(i = 0; i < 16; ++i)
	{
		chipherText[i] = e1[i] ^ b[i];
	}
}

void Encrypt_14_fast2(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys)
{
	unsigned char e1[16] = {0};

	memcpy(e1, plainText, 16);

	int i;
	for(i = 0; i < 9; ++i)
	{
		unsigned char* b = keys + 16*i;

		int xi;
		for(xi = 0; xi < 16; ++xi)
		{
			e1[xi] = kPi[ e1[xi] ^ b[xi] ];
		}

		unsigned char sum = 0;
		for(xi=0; xi<16 ; ++xi)
			sum ^= g(e1[xi], xi);
		e1[15] = sum;

		for(xi=1; xi<16; ++xi)
		{
			sum = 0;
			// xi is also number of sums in tail
			int ti, b;
			for(ti=16-xi, b=0; ti<16; ++ti, ++b) // sum sums
				sum ^= g(e1[ti], b);
			int si; // start index
			for(si=0; si<16-xi; ++si, ++b)
				sum ^= g(e1[si], b);
			e1[15-xi] = sum;
		}
	}
	unsigned char* b = keys+9*16;
	for(i = 0; i < 16; ++i)
	{
		chipherText[i] = e1[i] ^ b[i];
	}
}

void Encrypt_14_fast3(const unsigned char* plainText, unsigned char* chipherText, unsigned char* keys)
{
	unsigned char e1[16] = {0};

	memcpy(e1, plainText, 16);

	int i;
	for(i = 0; i < 9; ++i)
	{
		unsigned char* b = keys + 16*i;

		int xi;
		for(xi = 0; xi < 16; ++xi)
		{
			e1[xi] = kPi[ e1[xi] ^ b[xi] ];
		}

		unsigned char sum = 0;

		sum  = g(e1[ 0],  0);
		sum ^= g(e1[ 1],  1);
		sum ^= g(e1[ 2],  2);
		sum ^= g(e1[ 3],  3);
		sum ^= g(e1[ 4],  4);
		sum ^= g(e1[ 5],  5);
		sum ^= g(e1[ 6],  6);
		sum ^= g(e1[ 7],  7);
		sum ^= g(e1[ 8],  8);
		sum ^= g(e1[ 9],  9);
		sum ^= g(e1[10], 10);
		sum ^= g(e1[11], 11);
		sum ^= g(e1[12], 12);
		sum ^= g(e1[13], 13);
		sum ^= g(e1[14], 14);
		sum ^= g(e1[15], 15);
		e1[15] = sum;

		sum  = g(e1[15],  0);
		sum ^= g(e1[ 0],  1);
		sum ^= g(e1[ 1],  2);
		sum ^= g(e1[ 2],  3);
		sum ^= g(e1[ 3],  4);
		sum ^= g(e1[ 4],  5);
		sum ^= g(e1[ 5],  6);
		sum ^= g(e1[ 6],  7);
		sum ^= g(e1[ 7],  8);
		sum ^= g(e1[ 8],  9);
		sum ^= g(e1[ 9], 10);
		sum ^= g(e1[10], 11);
		sum ^= g(e1[11], 12);
		sum ^= g(e1[12], 13);
		sum ^= g(e1[13], 14);
		sum ^= g(e1[14], 15);
		e1[14] = sum;

		sum  = g(e1[14],  0);
		sum ^= g(e1[15],  1);
		sum ^= g(e1[ 0],  2);
		sum ^= g(e1[ 1],  3);
		sum ^= g(e1[ 2],  4);
		sum ^= g(e1[ 3],  5);
		sum ^= g(e1[ 4],  6);
		sum ^= g(e1[ 5],  7);
		sum ^= g(e1[ 6],  8);
		sum ^= g(e1[ 7],  9);
		sum ^= g(e1[ 8], 10);
		sum ^= g(e1[ 9], 11);
		sum ^= g(e1[10], 12);
		sum ^= g(e1[11], 13);
		sum ^= g(e1[12], 14);
		sum ^= g(e1[13], 15);
		e1[13] = sum;

		sum  = g(e1[13],  0);
		sum ^= g(e1[14],  1);
		sum ^= g(e1[15],  2);
		sum ^= g(e1[ 0],  3);
		sum ^= g(e1[ 1],  4);
		sum ^= g(e1[ 2],  5);
		sum ^= g(e1[ 3],  6);
		sum ^= g(e1[ 4],  7);
		sum ^= g(e1[ 5],  8);
		sum ^= g(e1[ 6],  9);
		sum ^= g(e1[ 7], 10);
		sum ^= g(e1[ 8], 11);
		sum ^= g(e1[ 9], 12);
		sum ^= g(e1[10], 13);
		sum ^= g(e1[11], 14);
		sum ^= g(e1[12], 15);
		e1[12] = sum;

		sum  = g(e1[12],  0);
		sum ^= g(e1[13],  1);
		sum ^= g(e1[14],  2);
		sum ^= g(e1[15],  3);
		sum ^= g(e1[ 0],  4);
		sum ^= g(e1[ 1],  5);
		sum ^= g(e1[ 2],  6);
		sum ^= g(e1[ 3],  7);
		sum ^= g(e1[ 4],  8);
		sum ^= g(e1[ 5],  9);
		sum ^= g(e1[ 6], 10);
		sum ^= g(e1[ 7], 11);
		sum ^= g(e1[ 8], 12);
		sum ^= g(e1[ 9], 13);
		sum ^= g(e1[10], 14);
		sum ^= g(e1[11], 15);
		e1[11] = sum;

		sum  = g(e1[11],  0);
		sum ^= g(e1[12],  1);
		sum ^= g(e1[13],  2);
		sum ^= g(e1[14],  3);
		sum ^= g(e1[15],  4);
		sum ^= g(e1[ 0],  5);
		sum ^= g(e1[ 1],  6);
		sum ^= g(e1[ 2],  7);
		sum ^= g(e1[ 3],  8);
		sum ^= g(e1[ 4],  9);
		sum ^= g(e1[ 5], 10);
		sum ^= g(e1[ 6], 11);
		sum ^= g(e1[ 7], 12);
		sum ^= g(e1[ 8], 13);
		sum ^= g(e1[ 9], 14);
		sum ^= g(e1[10], 15);
		e1[10] = sum;

		sum  = g(e1[10],  0);
		sum ^= g(e1[11],  1);
		sum ^= g(e1[12],  2);
		sum ^= g(e1[13],  3);
		sum ^= g(e1[14],  4);
		sum ^= g(e1[15],  5);
		sum ^= g(e1[ 0],  6);
		sum ^= g(e1[ 1],  7);
		sum ^= g(e1[ 2],  8);
		sum ^= g(e1[ 3],  9);
		sum ^= g(e1[ 4], 10);
		sum ^= g(e1[ 5], 11);
		sum ^= g(e1[ 6], 12);
		sum ^= g(e1[ 7], 13);
		sum ^= g(e1[ 8], 14);
		sum ^= g(e1[ 9], 15);
		e1[9] = sum;

		sum  = g(e1[ 9],  0);
		sum ^= g(e1[10],  1);
		sum ^= g(e1[11],  2);
		sum ^= g(e1[12],  3);
		sum ^= g(e1[13],  4);
		sum ^= g(e1[14],  5);
		sum ^= g(e1[15],  6);
		sum ^= g(e1[ 0],  7);
		sum ^= g(e1[ 1],  8);
		sum ^= g(e1[ 2],  9);
		sum ^= g(e1[ 3], 10);
		sum ^= g(e1[ 4], 11);
		sum ^= g(e1[ 5], 12);
		sum ^= g(e1[ 6], 13);
		sum ^= g(e1[ 7], 14);
		sum ^= g(e1[ 8], 15);
		e1[8] = sum;

		sum  = g(e1[ 8],  0);
		sum ^= g(e1[ 9],  1);
		sum ^= g(e1[10],  2);
		sum ^= g(e1[11],  3);
		sum ^= g(e1[12],  4);
		sum ^= g(e1[13],  5);
		sum ^= g(e1[14],  6);
		sum ^= g(e1[15],  7);
		sum ^= g(e1[ 0],  8);
		sum ^= g(e1[ 1],  9);
		sum ^= g(e1[ 2], 10);
		sum ^= g(e1[ 3], 11);
		sum ^= g(e1[ 4], 12);
		sum ^= g(e1[ 5], 13);
		sum ^= g(e1[ 6], 14);
		sum ^= g(e1[ 7], 15);
		e1[7] = sum;

		sum  = g(e1[ 7],  0);
		sum ^= g(e1[ 8],  1);
		sum ^= g(e1[ 9],  2);
		sum ^= g(e1[10],  3);
		sum ^= g(e1[11],  4);
		sum ^= g(e1[12],  5);
		sum ^= g(e1[13],  6);
		sum ^= g(e1[14],  7);
		sum ^= g(e1[15],  8);
		sum ^= g(e1[ 0],  9);
		sum ^= g(e1[ 1], 10);
		sum ^= g(e1[ 2], 11);
		sum ^= g(e1[ 3], 12);
		sum ^= g(e1[ 4], 13);
		sum ^= g(e1[ 5], 14);
		sum ^= g(e1[ 6], 15);
		e1[6] = sum;

		sum  = g(e1[ 6],  0);
		sum ^= g(e1[ 7],  1);
		sum ^= g(e1[ 8],  2);
		sum ^= g(e1[ 9],  3);
		sum ^= g(e1[10],  4);
		sum ^= g(e1[11],  5);
		sum ^= g(e1[12],  6);
		sum ^= g(e1[13],  7);
		sum ^= g(e1[14],  8);
		sum ^= g(e1[15],  9);
		sum ^= g(e1[ 0], 10);
		sum ^= g(e1[ 1], 11);
		sum ^= g(e1[ 2], 12);
		sum ^= g(e1[ 3], 13);
		sum ^= g(e1[ 4], 14);
		sum ^= g(e1[ 5], 15);
		e1[5] = sum;

		sum  = g(e1[ 5],  0);
		sum ^= g(e1[ 6],  1);
		sum ^= g(e1[ 7],  2);
		sum ^= g(e1[ 8],  3);
		sum ^= g(e1[ 9],  4);
		sum ^= g(e1[10],  5);
		sum ^= g(e1[11],  6);
		sum ^= g(e1[12],  7);
		sum ^= g(e1[13],  8);
		sum ^= g(e1[14],  9);
		sum ^= g(e1[15], 10);
		sum ^= g(e1[ 0], 11);
		sum ^= g(e1[ 1], 12);
		sum ^= g(e1[ 2], 13);
		sum ^= g(e1[ 3], 14);
		sum ^= g(e1[ 4], 15);
		e1[4] = sum;

		sum  = g(e1[ 4],  0);
		sum ^= g(e1[ 5],  1);
		sum ^= g(e1[ 6],  2);
		sum ^= g(e1[ 7],  3);
		sum ^= g(e1[ 8],  4);
		sum ^= g(e1[ 9],  5);
		sum ^= g(e1[10],  6);
		sum ^= g(e1[11],  7);
		sum ^= g(e1[12],  8);
		sum ^= g(e1[13],  9);
		sum ^= g(e1[14], 10);
		sum ^= g(e1[15], 11);
		sum ^= g(e1[ 0], 12);
		sum ^= g(e1[ 1], 13);
		sum ^= g(e1[ 2], 14);
		sum ^= g(e1[ 3], 15);
		e1[3] = sum;

		sum  = g(e1[ 3],  0);
		sum ^= g(e1[ 4],  1);
		sum ^= g(e1[ 5],  2);
		sum ^= g(e1[ 6],  3);
		sum ^= g(e1[ 7],  4);
		sum ^= g(e1[ 8],  5);
		sum ^= g(e1[ 9],  6);
		sum ^= g(e1[10],  7);
		sum ^= g(e1[11],  8);
		sum ^= g(e1[12],  9);
		sum ^= g(e1[13], 10);
		sum ^= g(e1[14], 11);
		sum ^= g(e1[15], 12);
		sum ^= g(e1[ 0], 13);
		sum ^= g(e1[ 1], 14);
		sum ^= g(e1[ 2], 15);
		e1[2] = sum;

		sum  = g(e1[ 2],  0);
		sum ^= g(e1[ 3],  1);
		sum ^= g(e1[ 4],  2);
		sum ^= g(e1[ 5],  3);
		sum ^= g(e1[ 6],  4);
		sum ^= g(e1[ 7],  5);
		sum ^= g(e1[ 8],  6);
		sum ^= g(e1[ 9],  7);
		sum ^= g(e1[10],  8);
		sum ^= g(e1[11],  9);
		sum ^= g(e1[12], 10);
		sum ^= g(e1[13], 11);
		sum ^= g(e1[14], 12);
		sum ^= g(e1[15], 13);
		sum ^= g(e1[ 0], 14);
		sum ^= g(e1[ 1], 15);
		e1[1] = sum;

		sum  = g(e1[ 1],  0);
		sum ^= g(e1[ 2],  1);
		sum ^= g(e1[ 3],  2);
		sum ^= g(e1[ 4],  3);
		sum ^= g(e1[ 5],  4);
		sum ^= g(e1[ 6],  5);
		sum ^= g(e1[ 7],  6);
		sum ^= g(e1[ 8],  7);
		sum ^= g(e1[ 9],  8);
		sum ^= g(e1[10],  9);
		sum ^= g(e1[11], 10);
		sum ^= g(e1[12], 11);
		sum ^= g(e1[13], 12);
		sum ^= g(e1[14], 13);
		sum ^= g(e1[15], 14);
		sum ^= g(e1[ 0], 15);
		e1[0] = sum;
	}
	unsigned char* b = keys+9*16;
	for(i = 0; i < 16; ++i)
	{
		chipherText[i] = e1[i] ^ b[i];
	}
}
