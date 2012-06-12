#include <stdio.h>
#include <stdlib.h>

#include "sha.h"

#define UINT 0xFFFFFFFFUL
#define ROTL(X, N) ((((X) << N) | ((X) >> (32 - N))) & UINT)

int file_end;
int finished = 0;
unsigned int starting_word;

unsigned int K (int t)
{
	if (t >= 0 && t < 20)
		return 0x5A827999;

	if (t >= 20 && t < 40)
		return 0x6ED9EBA1;

	if (t >= 40 && t < 60)
		return 0x8F1BBCDC;
	
	return 0xCA62C1D6;
}

unsigned int f_sha1 (int t, unsigned int b, unsigned int c, unsigned int d)
{
	if (t <= 19)
		return ((b & c) | ((~b) & d));

	if ((t >= 40) && (t <= 59))
		return ((b & c) | (b & d) | (c & d));
        
	return (b ^ c ^ d);
}

unsigned int get_padding (FILE *fp, int x, int y)
{
	int getbyte;

	if (!file_end) {
		getbyte = getc (fp);

		if (getbyte != EOF) {
			starting_word += 8;
			return (((unsigned int) getbyte) << (8 * (3 - y)));
		} else {
			file_end = 1;
			return (128UL << (8 * (3 - y)));
		}
	}

	if (x < 14) return 0;
	if ((x == 14) && (y == 0)) file_end = 2;
	if (file_end == 1) return 0;
	if (x == 14) {
		switch (y) {
			case 0:
				return (unsigned int) ((starting_word & 0xFF00000000000000U) >> 32);
				
			case 1: 
				return (unsigned int) ((starting_word & 0x00FF000000000000U) >> 32);
				
			case 2: 
				return (unsigned int) ((starting_word & 0x0000FF0000000000U) >> 32);
				
			case 3: 
				return (unsigned int) ((starting_word & 0x000000FF00000000U) >> 32);
		}
	} else if (x == 15) {
		switch (y) {
			case 0: 
				return (unsigned long int) (starting_word & 0xFF000000Ul);
					
			case 1: 
				return (unsigned long int) (starting_word & 0x00FF0000Ul);
					
			case 2: 
				return (unsigned long int) (starting_word & 0x0000FF00Ul);
					
			case 3: 
				finished = 1;
				return (unsigned long int) (starting_word & 0x000000FFUl);
		}
	}

	return 0;
}

char *sha1 (const char *filename, const char *output)
{
	FILE *in, *out;
	int i, j, t = 0;
	unsigned int a, b, c, d, e, w[80], temp;
	unsigned int h[] = {0x67452301UL, 0xEFCDAB89UL, 0x98BADCFEUL, 0x10325476UL, 0xC3D2E1F0UL};

	char *hash = malloc (100);

	finished = 0;
	starting_word = 0;
	file_end = 0;
	
	if ((in = fopen (filename, "rb")) == NULL) {
		perror (filename);
		exit (EXIT_FAILURE);
	}

	if ((out = fopen (output, "w")) == NULL) {
		perror (output);
		exit (EXIT_FAILURE);
	}


	while (!finished) {
		for (i = 0 ; i < 16 ; ++i)
			for (j = 0, w[i] = 0 ; j < 4 ; ++j)
				w[i] |= get_padding (in, i, j);

		for (t = 16 ; t < 80; ++t)
			w[t] = ROTL (w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16], 1);

		a = h[0];
		b = h[1];
		c = h[2];
		d = h[3];
		e = h[4];
	
		for(t = 0 ; t < 80; ++t) {
			temp = (ROTL (a, 5) + f_sha1 (t, b, c, d) + e + w[t] + K (t)) & UINT;
			e = d;
			d = c;
			c = ROTL(b, 30);
			b = a;
			a = temp;
		}

		h[0] = (h[0] + a) & UINT;
		h[1] = (h[1] + b) & UINT;
		h[2] = (h[2] + c) & UINT;
		h[3] = (h[3] + d) & UINT;
		h[4] = (h[4] + e) & UINT;
	}

	fprintf (out, "---BEGIN OS2 CRYPTO DATA---\n");
	fprintf (out, "Description:\n    Signature\n\n");
	fprintf (out, "File name:\n    %s\n\n", filename);
	fprintf (out, "Method:\n    SHA1\n\n");
	fprintf (out, "Key length:\n    AO\n\n");
	fprintf (out, "Signature:\n    %08x%08x%08x%08x%08x\n", h[0], h[1], h[2], h[3], h[4]);
	fprintf (out, "---END OS2 CRYPTO DATA---\n");

	sprintf (hash, "%08x:%08x:%08x:%08x:%08x", h[0], h[1], h[2], h[3], h[4]);

	fclose (in);
	fclose (out);

	return hash;
}
