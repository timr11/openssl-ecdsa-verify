#include "openssl/ec.h"
#include "openssl/obj_mac.h"
/* #include "internal/deprecated.h" */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Randomly-generated 512 bytes of data.
 * dd if=/dev/urandom of=data.raw count=1
 */
unsigned char data_raw[] = {
	0xc4, 0x86, 0x1b, 0x1b, 0xca, 0x22, 0xe0, 0xdf, 0x83, 0xa1, 0x98, 0x0b,
	0x30, 0xa5, 0x4c, 0xc0, 0x1c, 0xf6, 0x68, 0x13, 0x5e, 0x93, 0x67, 0x12,
	0xac, 0x2d, 0xf8, 0xbd, 0x6b, 0x44, 0x39, 0x0e, 0x15, 0x34, 0xcd, 0xc8,
	0x0b, 0xe8, 0x5f, 0x92, 0x09, 0xf9, 0x04, 0x70, 0x87, 0x83, 0xbc, 0xfe,
	0xe7, 0x92, 0x64, 0x74, 0xc5, 0x8f, 0xc6, 0xbe, 0x64, 0x30, 0x1b, 0x6a,
	0x3b, 0x17, 0x8c, 0x76, 0x6a, 0x61, 0x31, 0xe2, 0x43, 0xa1, 0xc7, 0xc0,
	0x2a, 0x79, 0x58, 0x9f, 0xcc, 0xa1, 0x3d, 0xbe, 0x5f, 0x44, 0xa6, 0x5e,
	0x9c, 0x64, 0x9a, 0xff, 0x84, 0x9c, 0x23, 0x5e, 0xa9, 0x8e, 0xb7, 0xfa,
	0x68, 0xfc, 0x41, 0x1b, 0xbd, 0x55, 0x47, 0x21, 0xda, 0xa4, 0xca, 0x26,
	0x42, 0xca, 0xc0, 0x10, 0x90, 0xd0, 0x84, 0xce, 0xba, 0x99, 0x55, 0xc0,
	0x96, 0xb6, 0xcf, 0xd6, 0x7f, 0xad, 0x11, 0x63, 0x3e, 0x1d, 0xef, 0x84,
	0xa0, 0x91, 0xeb, 0xe1, 0x7b, 0x12, 0xed, 0x60, 0x0a, 0x28, 0x87, 0x07,
	0x50, 0xa6, 0xd8, 0x20, 0x96, 0x31, 0xdf, 0x60, 0x06, 0xb5, 0x9c, 0x7b,
	0x7a, 0x2b, 0x85, 0x44, 0x00, 0x8c, 0xe8, 0xa5, 0x84, 0x59, 0xab, 0x0e,
	0x3a, 0x90, 0x59, 0xca, 0xe8, 0xe7, 0xe4, 0xeb, 0xf6, 0x02, 0x38, 0xbd,
	0x58, 0xb9, 0x21, 0xd2, 0x3d, 0xc5, 0xf1, 0xa2, 0x24, 0xbe, 0xa3, 0xcd,
	0x52, 0x17, 0xa6, 0x16, 0xd1, 0xfb, 0x54, 0x60, 0x11, 0x93, 0xd2, 0x21,
	0xba, 0x32, 0xbf, 0x1a, 0x3c, 0x37, 0xc2, 0x70, 0xb6, 0xf9, 0xa3, 0x01,
	0xda, 0x9b, 0x88, 0x21, 0x8f, 0x54, 0x19, 0x38, 0xbc, 0x5f, 0x7d, 0x84,
	0xfb, 0x22, 0xc0, 0xd7, 0x7a, 0xc8, 0xc7, 0x1f, 0xd6, 0x72, 0x15, 0x58,
	0x9d, 0x85, 0xd6, 0x49, 0x71, 0xee, 0xe8, 0x34, 0x52, 0xb2, 0x69, 0xb1,
	0xb5, 0x6a, 0x02, 0xbb, 0xf1, 0x8e, 0x8b, 0xc5, 0xbf, 0xf7, 0x91, 0x00,
	0x61, 0x35, 0x4e, 0x45, 0x0b, 0xe3, 0x61, 0xde, 0xc0, 0x16, 0x9e, 0xc3,
	0x69, 0x6a, 0xa8, 0x93, 0x03, 0x26, 0xea, 0x65, 0xdc, 0x57, 0x4f, 0x65,
	0xc4, 0x80, 0xc9, 0xb6, 0x6c, 0xa9, 0xfa, 0x35, 0x53, 0xd7, 0x05, 0xa7,
	0x64, 0x79, 0x07, 0xc1, 0x45, 0x41, 0x6d, 0x3f, 0xad, 0xd2, 0x85, 0xe8,
	0x44, 0xa9, 0xc0, 0x67, 0x9b, 0xb5, 0x24, 0xda, 0xdc, 0xd0, 0xa5, 0xeb,
	0x44, 0x54, 0xdc, 0x00, 0xbe, 0xbd, 0x08, 0xbb, 0x03, 0x9a, 0x5d, 0x97,
	0x46, 0x4e, 0xa0, 0x5f, 0xd2, 0x8d, 0x50, 0x90, 0xd6, 0x60, 0x4b, 0xe1,
	0xe3, 0x58, 0x29, 0x8b, 0x73, 0x45, 0x8d, 0xdb, 0xaa, 0xe2, 0xd9, 0xa5,
	0xca, 0x70, 0x8e, 0x5e, 0xe2, 0xd7, 0x21, 0xf1, 0x02, 0x67, 0x23, 0x59,
	0xd2, 0xdf, 0xdc, 0x8d, 0xe4, 0x8d, 0xdd, 0xa7, 0x6c, 0xca, 0x46, 0xb7,
	0x39, 0x68, 0x4a, 0x97, 0xfa, 0xfa, 0x37, 0x64, 0xca, 0x30, 0x3d, 0x03,
	0x03, 0x91, 0x57, 0xec, 0x50, 0xc8, 0x95, 0x0e, 0x4c, 0x96, 0x46, 0x68,
	0x89, 0xff, 0x50, 0x5a, 0xb3, 0xad, 0xee, 0x73, 0xd7, 0x52, 0xf6, 0x63,
	0x9c, 0x47, 0xde, 0x42, 0xd5, 0xc0, 0xb4, 0x52, 0x97, 0x74, 0x61, 0x55,
	0x29, 0x63, 0x38, 0x16, 0x6b, 0xf2, 0xe9, 0x45, 0x19, 0xba, 0xb4, 0x39,
	0x1a, 0xe7, 0xb3, 0xf8, 0xdf, 0x50, 0x6d, 0xee, 0x4d, 0x38, 0xc3, 0xb7,
	0x1e, 0x06, 0x05, 0x97, 0x31, 0x67, 0x4c, 0xcf, 0x4c, 0x70, 0x0b, 0x09,
	0xe7, 0x28, 0x49, 0x00, 0x1e, 0x0f, 0x9a, 0xf5, 0x39, 0xa9, 0xff, 0x92,
	0xce, 0x2c, 0xde, 0x3a, 0xb1, 0xf3, 0x6e, 0x28, 0xb7, 0x84, 0xaf, 0xa8,
	0x05, 0x58, 0x6f, 0xac, 0xfd, 0x92, 0x0d, 0xb3, 0xe2, 0xae, 0xb9, 0x15,
	0x20, 0x4c, 0x15, 0x69, 0xff, 0x55, 0x19, 0x68
};
unsigned int data_raw_len = 512;

/*
 * This array contains the DER-encoded signature of the above randomly generated data.
 * openssl dgst -sha256 -sign private.key -out data.enc data.raw
 */
unsigned char data_enc_valid[] = {
  0x30, 0x44, 0x02, 0x20, 0x2e, 0xac, 0x64, 0x56, 0xaa, 0xa1, 0x96, 0xdb,
  0x1f, 0x1d, 0xfd, 0xda, 0x57, 0x81, 0x59, 0x73, 0xd8, 0x32, 0x8d, 0xf2,
  0xca, 0x63, 0x67, 0xc2, 0x47, 0xe2, 0xbb, 0xbb, 0x79, 0xc8, 0x93, 0x90,
  0x02, 0x20, 0x14, 0x43, 0x07, 0xc0, 0xa1, 0x4b, 0x92, 0xa4, 0x12, 0x65,
  0xe3, 0x88, 0x93, 0x34, 0x6a, 0x74, 0xc5, 0x10, 0x87, 0xbb, 0xb8, 0xb7,
  0x94, 0xc2, 0x15, 0xec, 0x6d, 0xcd, 0x7d, 0xf1, 0xc1, 0x55
};
unsigned int data_enc_valid_len = 70;

/*
 * This array contains the above valid signature with the last byte
 * changed from 0x55 to 0x56, making it invalid.
 */
unsigned char data_enc_invalid[] = {
  0x30, 0x44, 0x02, 0x20, 0x2e, 0xac, 0x64, 0x56, 0xaa, 0xa1, 0x96, 0xdb,
  0x1f, 0x1d, 0xfd, 0xda, 0x57, 0x81, 0x59, 0x73, 0xd8, 0x32, 0x8d, 0xf2,
  0xca, 0x63, 0x67, 0xc2, 0x47, 0xe2, 0xbb, 0xbb, 0x79, 0xc8, 0x93, 0x90,
  0x02, 0x20, 0x14, 0x43, 0x07, 0xc0, 0xa1, 0x4b, 0x92, 0xa4, 0x12, 0x65,
  0xe3, 0x88, 0x93, 0x34, 0x6a, 0x74, 0xc5, 0x10, 0x87, 0xbb, 0xb8, 0xb7,
  0x94, 0xc2, 0x15, 0xec, 0x6d, 0xcd, 0x7d, 0xf1, 0xc1, 0x56
};
unsigned int data_enc_invalid_len = 70;

/*
 * This array contains the digest of the original randomly generated data.
 * sha256sum data.raw
 */
unsigned char dgst[] = {
	0x2c, 0xb2, 0x2f, 0x69, 0x0b, 0xa5, 0xe8, 0xa8, 0x83, 0x6c, 0x2e, 0xf4,
	0xa6, 0xf6, 0x69, 0x9a, 0x4d, 0x8b, 0xd8, 0xea, 0x46, 0xdb, 0xe1, 0x24,
	0x59, 0xd1, 0x85, 0x47, 0x15, 0xd7, 0x0b, 0xf7
};
unsigned int dgst_len = 32;

unsigned char pub_key_x[] = {
	0x35, 0xb1, 0xf5, 0x14, 0x76, 0x4a, 0x7d, 0xca, 0x1e, 0xa7, 0xb4, 0xd8,
	0xcc, 0x45, 0xb0, 0xc7, 0xc6, 0xcd, 0x29, 0x16, 0x11, 0x0e, 0x9c, 0x4f,
	0xfd, 0x45, 0x05, 0x13, 0x84, 0xc8, 0xb9, 0x1f
};
unsigned int pub_key_x_len = 32;

unsigned char pub_key_y[] = {
	0x71, 0x7d, 0x76, 0xb6, 0xac, 0x60, 0xf2, 0xff, 0x60, 0x72, 0x14, 0x1e,
	0x94, 0xa8, 0x16, 0xd4, 0x8f, 0xa5, 0x9c, 0xc4, 0x74, 0x88, 0x4f, 0x61,
	0x88, 0x4c, 0x04, 0x97, 0xa7, 0x82, 0xcf, 0xe4
};
unsigned int pub_key_y_len = 32;

int main(void) {
	BIGNUM *x;
	BIGNUM *y;
	EC_KEY *eckey;
	EC_GROUP *ecgroup;
	ECDSA_SIG *s_valid;
	ECDSA_SIG *s_invalid;
	const unsigned char *sig_ptr_valid = data_enc_valid;
	const unsigned char *sig_ptr_invalid = data_enc_invalid;

	/* Initialize key */
	eckey = EC_KEY_new();

	/* Initialize group for prime256v1 curve */
	ecgroup = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);

	/* Set key group */
	EC_KEY_set_group(eckey, ecgroup);

	/* Set x and y coordinates of public key to finish eckey setup */
	x = BN_bin2bn(pub_key_x, pub_key_x_len, NULL);
	y = BN_bin2bn(pub_key_y, pub_key_y_len, NULL);
	EC_KEY_set_public_key_affine_coordinates(eckey, x, y);

	/* Initialize ECDSA_SIG signatures */
	s_valid = ECDSA_SIG_new();
	s_invalid = ECDSA_SIG_new();

	/* Setup signatures from raw bytes */
	d2i_ECDSA_SIG(&s_valid, &sig_ptr_valid, data_enc_valid_len);
	d2i_ECDSA_SIG(&s_invalid, &sig_ptr_invalid, data_enc_invalid_len);

	assert(ECDSA_do_verify(dgst, dgst_len, s_valid, eckey) == 1);
	printf("Valid signature verified (correctly)\n");
	assert(ECDSA_do_verify(dgst, dgst_len, s_invalid, eckey) == 0);
	printf("Invalid signature not verified (correctly)\n");
	exit(EXIT_SUCCESS);
}
