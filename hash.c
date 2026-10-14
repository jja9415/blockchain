#include "hash.h"

unsigned char* SSHA(const unsigned char* msg, size_t length) {
    unsigned char A, B, C, D, E; //Initial Seed Value
    A = 56;
    B = 99;
    C = 102;
    D = 67;
    E = 76;

    for (int i = 0; i < length; i++) {
        for (int round = 0; round < 8; round++) {
            unsigned char g = (B & C) | (C & D);
            unsigned char newA = E;
            unsigned char newB = A;
            unsigned char newC = (A >> 2) + E;
            unsigned char newD = (A >> 2) ^ (B >> 1);
            unsigned char newE = (B >> 1) + g + msg[i];
            A = newA;
            B = newB;
            C = newC;
			D = newD;
            E = newE;
        }
      
    }
    unsigned char* digest = (unsigned char*)malloc(DIGEST_SIZE * sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = E;
    return digest;
}

uint32_t SSHA2(const char* msg, size_t len) {
    uint32_t A = 0x67452301;
	uint32_t B = 0xEFCDAB89;
	uint32_t C = 0x98BADCFE;
	uint32_t D = 0x10325476;
	uint32_t E = 0xC3D2E1F0;

    for (size_t i = 0; i < len; i++) {
        uint32_t msg_byte = (uint8_t)msg[i];
        uint32_t shift_a2 = A >> 2;
		uint32_t shift_b1 = B >> 1;

        uint32_t bit_and1 = B & C;
		uint32_t bit_and2 = C & D; 
		uint32_t bit_or = bit_and1 | bit_and2;
		uint32_t bit_xor = shift_a2 ^ shift_b1;

		uint32_t next_A = E;
		uint32_t next_B = A;
        uint32_t next_C = shift_b1 + E;
		uint32_t next_D = bit_xor;
        uint32_t next_E = bit_or + msg_byte + bit_xor;

        A = next_A;
		B = next_B;
		C = next_C;
		D = next_D;
		E = next_E;
    }
	return A ^ B ^ C ^ D ^ E;
}

int digest_equal(struct Digest digest1, struct Digest digest2) {
    return ((digest1.hash0 == digest2.hash0) &&
        (digest1.hash1 == digest2.hash1) &&
        (digest1.hash2 == digest2.hash2) &&
        (digest1.hash3 == digest2.hash3) &&
        (digest1.hash4 == digest2.hash4));
    
}

void printDigest(struct Digest digest) {
    printf("%d %d %d %d %d\n", digest.hash0, digest.hash1, digest.hash2, digest.hash3, digest.hash4);
}