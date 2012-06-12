#ifndef _RSA_H
#define _RSA_H

#define RSA_ENCRYPT 0
#define RSA_DECRYPT 1

#define BUFFER_SIZE 512

void rsa_crypt (const char *key_file, const char *input_file, const char *output_file, int mode);
void rsa_generate_key (const char *pub_key_file, const char *prvt_key_file, int length);

void rsa_convert_to_base64 (const char *in_file, const char *out_file);
void rsa_convert_from_base64 (const char *in_file, const char *out_file);

#endif /* _RSA_H */
