#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>

#include "rsa.h"
#include "base64.h"

void convert_to_mpz (mpz_t mpz, char *buffer, int size)
{
	int i;
	mpz_set_ui (mpz, 0);

	for (i = 0 ; i < size ; ++i) {
		mpz_mul_ui (mpz, mpz, 256);
		mpz_add_ui (mpz, mpz, ((unsigned int) buffer[i]) & 0x0000ff);
	}
}

void convert_from_mpz (char *buffer, mpz_t mpz, int size)
{
	int i;
	mpz_t r;

	mpz_init (r);

	for (i = size - 1 ; i > -1 ; --i)
		buffer[i] = mpz_fdiv_qr_ui (mpz, r, mpz, 256);
}

void rsa_generate_key (const char *pub_key_file, const char *prvt_key_file, int length)
{
	int i, print_len, len = length / 2;
	FILE *pub_out, *prvt_out;
	char buffer[len/2];
	gmp_randstate_t state;
	mpz_t random_num, p, q, n, z, a, b, d, e;

	if ((pub_out = fopen (pub_key_file, "w")) == NULL) {
		perror (pub_key_file);
		exit (EXIT_FAILURE);
	}
	
	if ((prvt_out = fopen (prvt_key_file, "w")) == NULL) {
		perror (prvt_key_file);
		exit (EXIT_FAILURE);
	}
	
	mpz_init (random_num);
	mpz_init (p);
	mpz_init (q);
	mpz_init (n);
	mpz_init (z);
	mpz_init (a);
	mpz_init (b);
	mpz_init (d);
	mpz_init (e);

	/* p generation */
	gmp_randinit (state, GMP_RAND_ALG_LC, 128);
	gmp_randseed_ui (state, time (NULL));
	mpz_urandomb (random_num, state, len);
	mpz_nextprime (p, random_num);

	/* q generation */
	gmp_randinit (state, GMP_RAND_ALG_LC, 128);
	gmp_randseed_ui (state, time (NULL));
	mpz_urandomb (random_num, state, len);
	mpz_nextprime (q, random_num);
	mpz_nextprime (q, q);

	/* n = pq calculation */
	mpz_mul (n, p, q);

	/* z = (p - 1) * (q - 1) calculation */
	mpz_sub_ui (a, p, 1);
	mpz_sub_ui (b, q, 1);
	mpz_mul (z, a, b);

	/* get e (relativy prime and less then z) parameter */
	mpz_set (e, z);
	
	while (mpz_cmp (e, z) >= 0) {
		gmp_randinit (state, GMP_RAND_ALG_LC, 128);
		gmp_randseed_ui (state, time (NULL));
		mpz_urandomm (random_num, state, z);

		mpz_nextprime (e, random_num);
		mpz_gcd (a, e, z);
		
		if (mpz_get_si (a) == 1)
			break;
	}

	/* e*d mod z = 1, get d */
	mpz_invert (d, e, z);

	/* write public key to file */
	fprintf (pub_out, "---BEGIN OS2 CRYPTO DATA---\n");
	fprintf (pub_out, "Description:\n    Public key\n\n");
	fprintf (pub_out, "Method:\n    RSA\n\n");
	fprintf (pub_out, "Key length:\n    %04x\n\n", length);
	fprintf (pub_out, "Modulus:");
	
	print_len = gmp_snprintf (buffer, len/2 + 1, "%0Zx", n);
	for (i = 0 ; i < print_len ; ++i) {
		if (!(i % 60))
			fprintf (pub_out, "\n    %c", buffer[i]);
		else
			fprintf (pub_out, "%c", buffer[i]);
	}
	
	fprintf (pub_out, "\n\nPublic exponent:");

	print_len = gmp_snprintf (buffer, len/2 + 1, "%0Zx", e);
	for (i = 0 ; i < print_len ; ++i) {
		if (!(i % 60))
			fprintf (pub_out, "\n    %c", buffer[i]);
		else
			fprintf (pub_out, "%c", buffer[i]);
	}

	fprintf (pub_out, "\n---END OS2 CRYPTO DATA---\n");
	
	/* write private key to file */
	fprintf (prvt_out, "---BEGIN OS2 CRYPTO DATA---\n");
	fprintf (prvt_out, "Description:\n    Private key\n\n");
	fprintf (prvt_out, "Method:\n    RSA\n\n");
	fprintf (prvt_out, "Key length:\n    %04x\n\n", length);
	fprintf (prvt_out, "Modulus:");
	
	print_len = gmp_snprintf (buffer, len/2 + 1, "%0Zx", n);
	for (i = 0 ; i < print_len ; ++i) {
		if (!(i % 60))
			fprintf (prvt_out, "\n    %c", buffer[i]);
		else
			fprintf (prvt_out, "%c", buffer[i]);
	}
	
	fprintf (prvt_out, "\n\nPrivate exponent:");
	
	print_len = gmp_snprintf (buffer, len/2 + 1, "%0Zx", d);
	for (i = 0 ; i < print_len ; ++i) {
		if (!(i % 60))
			fprintf (prvt_out, "\n    %c", buffer[i]);
		else
			fprintf (prvt_out, "%c", buffer[i]);
	}

	fprintf (prvt_out, "\n---END OS2 CRYPTO DATA---\n");

	fclose (pub_out);
	fclose (prvt_out);
}

void rsa_strip_key (const char *in_file, const char *out_file, const char *str)
{
	char buffer[512];

	sprintf (buffer, "./strip_key.pl %s %s %s", in_file, out_file, str);
	system (buffer);
}

void rsa_crypt (const char *key_file, const char *input_file, const char *output_file, int mode)
{
	mpz_t a, b, n, key;
	int i, block_size, block_size_out, size;
	FILE *in, *out, *fkey, *fmod;
	char buffer[BUFFER_SIZE];
	
	char *tmp_key_file = "/tmp/key_file";
	char *tmp_mod_file = "/tmp/mod_file";
	
	rsa_strip_key (key_file, tmp_key_file, "exponent");
	rsa_strip_key (key_file, tmp_mod_file, "Modulus");
	
	mpz_init (a);
	mpz_init (b);
	mpz_init (n);
	mpz_init (key);

	/* open files */
	if ((fkey = fopen (tmp_key_file, "r")) == NULL) {
		perror (tmp_key_file);
		exit (EXIT_FAILURE);
	}
	
	if ((fmod = fopen (tmp_mod_file, "r")) == NULL) {
		perror (tmp_mod_file);
		exit (EXIT_FAILURE);
	}

	if ((in = fopen (input_file, "r")) == NULL) {
		perror (input_file);
		exit (EXIT_FAILURE);
	}

	if ((out = fopen (output_file, "w")) == NULL) {
		perror (output_file);
		exit (EXIT_FAILURE);
	}
	
	/* get keys from file */
	mpz_inp_str (n, fmod, 16);
	mpz_inp_str (key, fkey, 16);

	i = mpz_sizeinbase (n, 2);
	
	block_size = i / 8;
	block_size_out = block_size + 1;
	
	if (mode == RSA_DECRYPT) {
		--block_size_out;
		++block_size;
	}

	while (1) {
		/* get block from input file */
		size = fread (buffer, 1, block_size, in);

		if (!size) 
			break;

		if (size < block_size)
			for (i = size; i < block_size; ++i)
				buffer[i] = '\0';

		convert_to_mpz (a, buffer, block_size);
		mpz_powm (b, a, key, n);
		convert_from_mpz (buffer, b, block_size_out);

		/* save crypted output */
		fwrite (buffer, 1, block_size_out, out);
	}

	fclose (fkey);
	fclose (fmod);
	fclose (in);
	fclose (out);

	//unlink (tmp_mod_file);
	//unlink (tmp_key_file);
}

void rsa_convert_to_base64 (const char *in_file, const char *out_file)
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
	fprintf (out, "Method:\n    RSA\n\n");
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

void rsa_convert_from_base64 (const char *in_file, const char *out_file)
{
	unsigned char *out_buffer, *in_buffer, *tmp_buffer;
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
