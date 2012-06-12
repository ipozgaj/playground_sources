#ifndef _DES_H
#define _DES_H

#define DES_ENCRYPT 0
#define DES_DECRYPT 1

#define DES_REGULAR_PERMUTATION 0
#define DES_INVERSE_PERMUTATION 1

#define DES_PADDING ' '

typedef unsigned char uchar_t;

void des_crypt (const char *in_file, const char *out_file, uchar_t *key, int mode);
void des_convert_to_base64 (const char *in_file, const char *out_file);
void des_convert_from_base64 (const char *in_file, const char *out_file);
void des_generate_key (const char *filename);
char *des_get_key_from_file (const char *filename);

#endif /* _DES_H */
