#ifndef _UTILS_H
#define _UTILS_H

#include <ctype.h>

#define ERR -1
#define DECODE64(c) (isascii (c) ? base64val[c] : ERR)


static const char base64val[] = {
	ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,
	ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,
	ERR, ERR, ERR, ERR,  ERR, ERR, ERR, ERR,  ERR, ERR, ERR,  62,  ERR, ERR, ERR,  63,
	 52,  53,  54,  55,   56,  57,  58,  59,   60,  61, ERR, ERR,  ERR, ERR, ERR, ERR,
	ERR,   0,   1,   2,    3,   4,   5,   6,    7,   8,   9,  10,   11,  12,  13,  14,
	 15,  16,  17,  18,   19,  20,  21,  22,   23,  24,  25, ERR,  ERR, ERR, ERR, ERR,
	ERR,  26,  27,  28,   29,  30,  31,  32,   33,  34,  35,  36,   37,  38,  39,  40,
	 41,  42,  43,  44,   45,  46,  47,  48,   49,  50,  51, ERR,  ERR, ERR, ERR, ERR
};

void base64_encode (const unsigned char *src, unsigned char *dest, int len);
int base64_decode (const unsigned char *src, unsigned char *dest);

#endif /* _UTILS_H */
