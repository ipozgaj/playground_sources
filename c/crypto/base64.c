/* base64.c - Base64 encoding/decoding as described in RFC1521 
 *
 * (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <string.h>

#include "base64.h"

static const char *base64digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode (const unsigned char *src, unsigned char *dest, int len)
{
	for ( ; len >= 3 ; len -= 3) {
		*dest++ = base64digits[src[0] >> 2];
		*dest++ = base64digits[((src[0] << 4) & 0x30) | (src[1] >> 4)];
		*dest++ = base64digits[((src[1] << 2) & 0x3c) | (src[2] >> 6)];
		*dest++ = base64digits[src[2] & 0x3f];
		src += 3;
	}

	if (len > 0) {
		unsigned char fragment;

		*dest++ = base64digits[src[0] >> 2];
		fragment = (src[0] << 4) & 0x30;

		if (len > 1)
			fragment |= src[1] >> 4;

		*dest++ = base64digits[fragment];
		*dest++ = (len < 2) ? '=' : base64digits[(src[1] << 2) & 0x3c];
		*dest++ = '=';
	}

	*dest = '\0';
}

int base64_decode (const unsigned char *src, unsigned char *dest)
{
	int len = 0;
	unsigned char digit[4];

	if (src[0] == '+' && src[1] == ' ')
		src += 2;

	if (*src == '\n')
		return 0;

	do {
		digit[0] = src[0];

		if (DECODE64 (digit[0]) == ERR)
			return -1;

		digit[1] = src[1];

		if (DECODE64 (digit[1]) == ERR)
			return -1;

		digit[2] = src[2];

		if (digit[2] != '=' && DECODE64 (digit[2]) == ERR)
			return -1;
		
		digit[3] = src[3];

		if (digit[3] != '=' && DECODE64 (digit[3]) == ERR)
			return -1;

		src += 4;
		
		*dest++ = (DECODE64 (digit[0]) << 2) | (DECODE64 (digit[1]) >> 4);
		++len;

		if (digit[2] != '=') {
			*dest++ = ((DECODE64 (digit[1]) << 4) & 0xf0) | (DECODE64 (digit[2]) >> 2);
			++len;

			if (digit[3] != '=') {
				*dest++ = ((DECODE64 (digit[2]) << 6) & 0xc0) | DECODE64 (digit[3]);
				++len;
			}
		}
	} while (*src && *src != '\n' && digit[3] != '=');

	*dest = '\0';
	
	return (len);
}
