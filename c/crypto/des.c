/* des.c - DES implementation */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "des.h"
#include "base64.h"

/* initial permutation */
uchar_t t1[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 
	60, 52, 44, 36, 28, 20, 12, 4, 
	62, 54, 46, 38, 30, 22, 14, 6, 
	64, 56, 48, 40, 32, 24, 16, 8, 
	57, 49, 41, 33, 25, 17,  9, 1, 
	59, 51, 43, 35, 27, 19, 11, 3, 
	61, 53, 45, 37, 29, 21, 13, 5, 
	63, 55, 47, 39, 31, 23, 15, 7 
};

/* C block permutation */
uchar_t t2[28] = {
	57, 49, 41, 33, 25, 17,  9, 
	 1, 58, 50, 42, 34, 26, 18, 
	10,  2, 59, 51, 43, 35, 27, 
	19, 11,  3, 60, 52, 44, 36
};

/* D block permutation */
uchar_t t3[28] = {
	63, 55, 47, 39, 31, 23, 15, 
	 7, 62, 54, 46, 38, 30, 22, 
	14,  6, 61, 53, 45, 37, 29, 
	21, 13,  5, 28, 20, 12,  4
};

/* key rotation */
uchar_t t4[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/* C and D block merging */
uchar_t t5[48] = {
	14, 17, 11, 24,  1,  5, 
	 3, 28, 15,  6, 21, 10, 
	23, 19, 12,  4, 26,  8, 
	16,  7, 27, 20, 13,  2, 
	41, 52, 31, 37, 47, 55, 
	30, 40, 51, 45, 33, 48, 
	44, 49, 39, 56, 34, 53, 
	46, 42, 50, 36, 29, 32
};

/* 32 to 48 bits expansion */
uchar_t t6[48] = {
	32,  1,  2,  3,  4,  5, 
	 4,  5,  6,  7,  8,  9, 
	 8,  9, 10, 11, 12, 13, 
	12, 13, 14, 15, 16, 17, 
	16, 17, 18, 19, 20, 21, 
	20, 21, 22, 23, 24, 25, 
	24, 25, 26, 27, 28, 29, 
	28, 29, 30, 31, 32,  1
};

/* function result */
uchar_t t7[32] = {
	16,  7, 20, 21, 29, 12, 28, 17, 
	 1, 15, 23, 26,  5, 18, 31, 10, 
	 2,  8, 24, 14, 32, 27,  3,  9, 
	19, 13, 30,  6, 22, 11,  4, 25
};

/* selection tables */
uchar_t s[8][4][16] = {
	{ 
		{14,  4, 13, 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9, 0,  7}, 
		{ 0, 15,  7, 4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5, 3,  8}, 
		{ 4,  1, 14, 8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10, 5,  0}, 
		{15, 12,  8, 2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0, 6, 13}
	}, 
	{ 
		{15,  1,  8, 14,  6, 11,  3,  4,  9, 7,  2, 13, 12, 0,  5, 10}, 
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12, 0,  1, 10,  6, 9, 11,  5}, 
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5, 8, 12,  6,  9, 3,  2, 15}, 
		{13,  8, 10,  1,  3, 15,  4,  2, 11, 6,  7, 12,  0, 5, 14,  9}
	}, 
	{ 
		{10, 0,  9, 14, 6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8}, 
		{13, 7,  0,  9, 3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1}, 
		{13, 6,  4,  9, 8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7}, 
		{ 1,10, 13,  0, 6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
	}, 
	{ 
		{ 7, 13, 14, 3,  0,  6,  9, 10,  1, 2, 8,  5, 11, 12,  4, 15}, 
		{13,  8, 11, 5,  6, 15,  0,  3,  4, 7, 2, 12,  1, 10, 14,  9}, 
		{10,  6,  9, 0, 12, 11,  7, 13, 15, 1, 3, 14,  5,  2,  8,  4}, 
		{ 3, 15,  0, 6, 10,  1, 13,  8,  9, 4, 5, 11, 12,  7,  2, 14}
	}, 
	{ 
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13, 0, 14,  9}, 
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3, 9,  8,  6}, 
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6, 3,  0, 14}, 
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10, 4,  5,  3}
	}, 
	{ 
		{12,  1, 10, 15, 9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11}, 
		{10, 15,  4,  2, 7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8}, 
		{ 9, 14, 15,  5, 2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6}, 
		{ 4,  3,  2, 12, 9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
	}, 
	{ 
		{ 4, 11,  2, 14, 15, 0,  8, 13,  3, 12,  9,  7,  5, 10, 6,  1}, 
		{13,  0, 11,  7,  4, 9,  1, 10, 14,  3,  5, 12,  2, 15, 8,  6}, 
		{ 1,  4, 11, 13, 12, 3,  7, 14, 10, 15,  6,  8,  0,  5, 9,  2}, 
		{ 6, 11, 13,  8,  1, 4, 10,  7,  9,  5,  0, 15, 14,  2, 3, 12}
	}, 
	{ 
		{13,  2,  8, 4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7}, 
		{ 1, 15, 13, 8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2}, 
		{ 7, 11,  4, 1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8}, 
		{ 2,  1, 14, 7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
	}
};

/* permutates table source to table dest using permutation table t of size n */
void permutate (uchar_t *perm_table, uchar_t *src, uchar_t *dest, int size, int mode) 
{
	int i;
	
	i = size / 8;

	if (i % 8 == 0)
		--i;

	while (i >= 0) {
		dest[i] = '\0';
		--i;
	}

	for (i = 0 ; i < size ; ++i) {
		if (mode == DES_REGULAR_PERMUTATION)
			if (src[(perm_table[i] - 1) / 8] & (0x80 >> ((perm_table[i] - 1) % 8)))
				dest[i / 8] |= 0x80 >> (i % 8);

		if (mode == DES_INVERSE_PERMUTATION)
			if (src[i / 8] & (0x80 >> (i % 8)))
				dest[(perm_table[i] - 1) / 8] |= 0x80 >> ((perm_table[i] - 1) % 8);
	}
}

void f (uchar_t *in_block, uchar_t *out_block, uchar_t *key)
{
	int i;
	unsigned int sest;
	uchar_t er[7];
	uchar_t rez[4] = {0, 0, 0, 0};
	uchar_t row, column;

	permutate (t6, in_block, er, 48, DES_REGULAR_PERMUTATION);

	for (i = 0 ; i < 6 ; ++i)
		er[i] = er[i] ^ key[i];

	er[6] = '\0';

	for (i = 0 ; i < 8; ++i) {
		sest = er[(i * 6) / 8] << ((i * 6) % 8);
		sest |= (er[((i + 1) * 6) / 8] & 0xff) >> (8 - (i * 6) % 8);
		sest = (sest >> 2) & 0x3f; 
		row = ((sest & 0x20) >> 4) | (sest & 1);
		column = (sest >> 1) & 0x0f;
		rez[i / 2] |= (s[i][row][column] & 0x0f) << ((1 - i % 2) * 4);
	}

	permutate (t7, rez, out_block, 32, DES_REGULAR_PERMUTATION);
}

void des_block (uchar_t *in_block, uchar_t *key, uchar_t *out_block, int mode)
{
	int i, j;
	uchar_t permutation[8], func_res[4];
	uchar_t l1[4], r1[4], l2[4], r2[4];

	permutate (t1, in_block, permutation, 64, DES_REGULAR_PERMUTATION);

	for (i = 0 ; i < 4 ; ++i)
		l1[i] = permutation[i];
	
	for (i = 0 ; i < 4 ; ++i)
		r1[i] = permutation[i + 4];

	for (j = 0; j < 16; ++j) {
		for (i = 0 ; i < 4 ; ++i) l2[i] = r1[i];
		
		if (mode == DES_ENCRYPT)
			f (r1, func_res, key + j * 6);

		if (mode == DES_DECRYPT)
			f (r1, func_res, key + (15 - j) * 6);

		for(i = 0 ; i < 4 ; ++i)
			r2[i] = l1[i] ^ func_res[i];
		
		for(i = 0 ; i < 4 ; ++i)
			l1[i] = l2[i];
		
		for(i = 0 ; i < 4 ; ++i)
			r1[i] = r2[i];
	}
	
	for (i = 0 ; i < 4 ; ++i)
		permutation[i] = r1[i];
	
	for (i = 0 ; i < 4 ; ++i)
		permutation[i + 4] = l1[i];

	permutate (t1, permutation, out_block, 64, DES_INVERSE_PERMUTATION);
}


void des_generate_keys (uchar_t *key, uchar_t *key_set)
{
	int i, j, k;
	uchar_t key_c_block[4], key_d_block[4], key_merged[8];
	uchar_t bit, tmp;

	permutate (t2, key, key_c_block, 28, DES_REGULAR_PERMUTATION);
	permutate (t3, key, key_d_block, 28, DES_REGULAR_PERMUTATION);

	for (i = 0 ; i < 16 ; ++i) {
		for (j = 0 ; j < t4[i] ; ++j) {
			bit = 0x80 & key_c_block[3];
			key_c_block[3] <<= 1;
			k = 2;
			
			while (k >= 0) {
				tmp = 0x80 & key_c_block[k];
				key_c_block[k] <<= 1;
				
				if (bit)
					key_c_block[k] |= (bit >> 7);
				
				bit = tmp;
				--k;
			}

			if (bit)
				key_c_block[3] |= (bit >> 3);
			
			bit = 0x80 & key_d_block[3];
			key_d_block[3] <<= 1;
			k = 2;
			
			while (k >= 0) {
				tmp = 0x80 & key_d_block[k];
				key_d_block[k] <<= 1;
				
				if (bit)
					key_d_block[k] |= (bit >> 7);

				bit = tmp;
				--k;
			}

			if (bit)
				key_d_block[3] |= (bit >> 3);
		}

		key_merged[0] = key_c_block[0];
		key_merged[1] = key_c_block[1];
		key_merged[2] = key_c_block[2];
		key_merged[3] = key_c_block[3];
		key_merged[4] = key_d_block[0];
		key_merged[5] = key_d_block[1];
		key_merged[6] = key_d_block[2];
		key_merged[7] = key_d_block[3];

		for(j = 4 ; j <= 7 ; ++j) {
			bit = 0x80 & key_merged[7];
			key_merged[7] <<= 1;
			k = 6;

			while (k >= 4) {
				tmp = 0x80 & key_merged[k];
				key_merged[k] <<= 1;
				
				if (bit)
					key_merged[k] |= (bit >> 7);

				bit = tmp;
				--k;
			}
	
			if (bit)
				key_merged[3] |= (bit >> j);
		}

		permutate (t5, key_merged, key_set + i * 6, 48, DES_REGULAR_PERMUTATION);
	}
}

/* main DES function - encrypts file with given key and writes output to file */
void des_crypt (const char *in_file, const char *out_file, uchar_t *key, int mode)
{
	int i, size, done;
	FILE *in, *out;
	uchar_t in_block[8], out_block[8];
	uchar_t *key_set;
	
	if (!(in = fopen (in_file, "r"))) {
		perror (in_file);
		exit (EXIT_FAILURE);
	}
	
	if (!(out = fopen (out_file, "wb"))) {
		perror (out_file);
		exit (EXIT_FAILURE);
	}
	
	key_set = malloc (16 * 6);
	des_generate_keys (key, key_set);
	
	done = 0;
	
	while (!done && (size = fread (in_block, 1, 8, in)) !=0 ) {
		if (feof (in)) {
			for (i = size ; i < 8 ; ++i)
				in_block[i] = DES_PADDING;
	
			done = 1;
		}

		des_block (in_block, key_set, out_block, mode);
		fwrite (out_block, 8, 1, out);
	}

	fclose (in);
	fclose (out);
	free (key_set);
}

void des_convert_to_base64 (const char *in_file, const char *out_file)
{
	int i, buffer_size, n;
	unsigned char *in_buffer, *out_buffer;
	FILE *in, *out;
	
	if (!(in = fopen (in_file, "rb"))) {
		perror (in_file);
		exit (EXIT_FAILURE);
	}
	
	if (!(out = fopen (out_file, "w"))) {
		perror (out_file);
		exit (EXIT_FAILURE);
	}
	
	fprintf (out, "---BEGIN OS2 CRYPTO DATA---\n");
	fprintf (out, "Description:\n    Crypted file\n\n");
	fprintf (out, "Method:\n    DES\n\n");
	fprintf (out, "File name:\n    %s\n\n", in_file);
	fprintf (out, "Data:\n    ");

	fseek (in, 0L, SEEK_END);
	buffer_size = ftell (in);
	fseek (in, 0L, SEEK_SET);
	
	in_buffer = malloc (buffer_size);
	out_buffer = malloc (buffer_size * 2);
	
	n = fread (in_buffer, 1, buffer_size, in);
	base64_encode (in_buffer, out_buffer, n);
	
	for (i = 0 ; i < strlen (out_buffer) ; ++i) {
		if (!(i % 60) && i != 0)
			fprintf (out, "\n    ");
		
		fprintf (out, "%c", out_buffer[i]);
	}
	
	fprintf (out, "\n---END OS2 CRYPTO DATA---\n");

	free (in_buffer);
	free (out_buffer);
	fclose (in);
	fclose (out);
}

void des_convert_from_base64 (const char *in_file, const char *out_file)
{
	uchar_t *out_buffer, *in_buffer, *tmp_buffer;
	char buffer[256];
	FILE *in, *out;
	int i, j, begin, end, len, n;
	
	if (!(in = fopen (in_file, "r"))) {
		perror (in_file);
		exit (EXIT_FAILURE);
	}
	
	if (!(out = fopen (out_file, "wb"))) {
		perror (out_file);
		exit (EXIT_FAILURE);
	}

	do {
		fgets (buffer, 256, in);
		
		if (!strcmp (buffer, "Data:\n"))
			begin = ftell (in);
		
		if (!strcmp (buffer, "---END OS2 CRYPTO DATA---\n"))
			end = ftell (in) - strlen ("---END OS2 CRYPTO DATA---\n");
	} while (!feof (in));

	len = end - begin - 1;
	
	in_buffer = malloc (len); 
	tmp_buffer = malloc (len); 
	out_buffer = malloc (len); 

	fseek (in, begin, SEEK_SET);
	n = fread (in_buffer, 1, len, in);

	for (i = 0, j = 0 ; i < n ; ++i) {
		if (in_buffer[i] == ' ' || in_buffer[i] == '\n') continue;
		tmp_buffer[j++] = in_buffer[i];
	}

	tmp_buffer[j] = '\0';
	j = base64_decode (tmp_buffer, out_buffer);
	fwrite (out_buffer, 1, j, out);

	fclose (in);
	fclose (out);
	free (in_buffer);
	free (tmp_buffer);
	free (out_buffer);
}

/* generates random DES key to file */
void des_generate_key (const char *filename)
{
	FILE *out;
	
	srand ((unsigned) time (NULL));
	
	if (!(out = fopen (filename, "w"))) {
		perror (filename);
		exit (EXIT_FAILURE);
	}

	fprintf (out, "---BEGIN OS2 CRYPTO DATA---\n");
	fprintf (out, "Description:\n    Secret key\n\n");
	fprintf (out, "Method:\n    DES\n\n");
	fprintf (out, "Secret key:\n    ");
	fprintf (out, "%0x%0x\n", rand(), rand());
	
	fprintf (out, "---END OS2 CRYPTO DATA---\n");

	fclose (out);
}


/* returns DES key from well formed key file */
char *des_get_key_from_file (const char *filename)
{
	FILE *in;
	char buffer[256];
	uchar_t *key = malloc (9);
	unsigned int key1, key2;
	
	if (!key) {
		perror ("malloc");
		exit (EXIT_FAILURE);
	}
	
	if (!(in = fopen (filename, "r"))) {
		perror (filename);
		exit (EXIT_FAILURE);
	}
	
	do {
		fgets (buffer, 256, in);
		if (!strcmp (buffer, "Secret key:\n")) {
			fgets (buffer, 256, in);
			buffer[20] = '\0';
			sscanf (buffer, "%8x%8x", &key1, &key2);
			break;
		}
	} while (!feof (in));
	
	key[3] = * ((uchar_t *) &key1);
	key[2] = * (((uchar_t *) &key1) + 1);
	key[1] = * (((uchar_t *) &key1) + 2);
	key[0] = * (((uchar_t *) &key1) + 3);
	key[7] = * ((uchar_t *) &key2);
	key[6] = * (((uchar_t *) &key2) + 1);
	key[5] = * (((uchar_t *) &key2) + 2);
	key[4] = * (((uchar_t *) &key2) + 3);
	key[8] = '\0';
	
	return key;
}
