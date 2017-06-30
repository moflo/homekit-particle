//
//  SRPClient.cpp
//  Homekit
//
//  Created by d. nye on 6/28/17.
//  Copyright © 2017 Mobile Flow LLC. All rights reserved.
//

#include "SRPClient.h"


SRPClient::SRPClient()
{
}

SRPClient::~SRPClient()
{
}

int SRPClient::getChallenge(uint8_t **salt,uint16_t *salt_len, uint8_t **key, uint16_t *key_len)
{
    
    
    size_t len_s = 16;
    size_t len_v = 0;
    
    const char *username = "alice";
    const char *password = "password123";
    
    SRP_HashAlgorithm alg = SRP_SHA512;
    SRP_NGType ng_type = SRP_NG_3072;   // was SRP_NG_1024; // TEST_NG;
    
    // The test vectors from
    // https://tools.ietf.org/html/rfc5054#appendix-B
    
    static const uint8_t srp_5054_salt[] = {
        0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E, 0xB5, 0xA7, 0x27, 0x67, 0x3A, 0x24,
        0x41, 0xEE
    };
    
    static const uint8_t srp_3072_salt[] = {
        0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E, 0xB5, 0xA7, 0x27, 0x67, 0x3A, 0x24,
        0x41, 0xEE
    };
    
    unsigned char *bytes_s = 0;
    bytes_s = (unsigned char *)malloc(sizeof(srp_3072_salt));
    memcpy(bytes_s, srp_3072_salt, sizeof(srp_3072_salt));
    
    unsigned char *bytes_v = 0;
    
    srp_create_salted_verification_key(alg,
                                       ng_type,
                                       username,
                                       (const unsigned char *)password,
                                       strlen(password),
                                       &bytes_s,
                                       &len_s,
                                       &bytes_v,
                                       &len_v,
                                       NULL,
                                       NULL);
    
    *salt = bytes_s;
    *salt_len = sizeof(srp_3072_salt);
    *key = bytes_v;
    *key_len = len_v;
    
    return 0;
}

int SRPClient::verifySession(uint8_t **serverKeyProof, uint16_t *proof_len, uint8_t *clientPublicKey, uint16_t client_key_len, uint8_t *clientKeyProof, uint16_t client_proof_len)
{
    struct SRPVerifier *ver;
    struct SRPUser *usr;
    
    size_t len_s = 16;
    size_t len_v = client_proof_len;
    size_t len_A = 0;
    size_t len_B = 0;
    
    const char *username = "alice";
    const char *password = "password123";
    
    SRP_HashAlgorithm alg = SRP_SHA512;
    SRP_NGType ng_type = SRP_NG_3072;   // was SRP_NG_1024; // TEST_NG;
    
    // The test vectors from
    // https://tools.ietf.org/html/rfc5054#appendix-B
    
    
    static const uint8_t srp_3072_salt[] = {
        0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E, 0xB5, 0xA7, 0x27, 0x67, 0x3A, 0x24,
        0x41, 0xEE
    };
    
    static const uint8_t srp_3072_a[] = {
        0x60, 0x97, 0x55, 0x27, 0x03, 0x5C, 0xF2, 0xAD, 0x19, 0x89, 0x80, 0x6F, 0x04, 0x07,
        0x21, 0x0B, 0xC8, 0x1E, 0xDC, 0x04, 0xE2, 0x76, 0x2A, 0x56, 0xAF, 0xD5, 0x29, 0xDD,
        0xDA, 0x2D, 0x43, 0x93
    };
    
    static const uint8_t srp_3072_b[] = {
        0xE4, 0x87, 0xCB, 0x59, 0xD3, 0x1A, 0xC5, 0x50, 0x47, 0x1E, 0x81, 0xF0, 0x0F, 0x69,
        0x28, 0xE0, 0x1D, 0xDA, 0x08, 0xE9, 0x74, 0xA0, 0x04, 0xF4, 0x9E, 0x61, 0xF5, 0xD1,
        0x05, 0x28, 0x4D, 0x20
    };

    
    
    unsigned char *bytes_s = 0;
    bytes_s = (unsigned char *)malloc(sizeof(srp_3072_salt));
    memcpy(bytes_s, srp_3072_salt, sizeof(srp_3072_salt));
    
    unsigned char *bytes_v = clientKeyProof;
    
    unsigned char *bytes_A = 0;
    unsigned char *bytes_B = 0;
    
    usr = srp_user_new(alg, ng_type, username, username, (const unsigned char *)password, strlen(password), NULL, NULL);
    
    srp_user_start_authentication(usr, NULL, (unsigned char *)srp_3072_a, 32, &bytes_A, &len_A);
    
    
    static const uint8_t srp_3072_A[] = {
        0xFA, 0xB6, 0xF5, 0xD2, 0x61, 0x5D, 0x1E, 0x32, 0x35, 0x12, 0xE7, 0x99, 0x1C, 0xC3,
        0x74, 0x43, 0xF4, 0x87, 0xDA, 0x60, 0x4C, 0xA8, 0xC9, 0x23, 0x0F, 0xCB, 0x04, 0xE5,
        0x41, 0xDC, 0xE6, 0x28, 0x0B, 0x27, 0xCA, 0x46, 0x80, 0xB0, 0x37, 0x4F, 0x17, 0x9D,
        0xC3, 0xBD, 0xC7, 0x55, 0x3F, 0xE6, 0x24, 0x59, 0x79, 0x8C, 0x70, 0x1A, 0xD8, 0x64,
        0xA9, 0x13, 0x90, 0xA2, 0x8C, 0x93, 0xB6, 0x44, 0xAD, 0xBF, 0x9C, 0x00, 0x74, 0x5B,
        0x94, 0x2B, 0x79, 0xF9, 0x01, 0x2A, 0x21, 0xB9, 0xB7, 0x87, 0x82, 0x31, 0x9D, 0x83,
        0xA1, 0xF8, 0x36, 0x28, 0x66, 0xFB, 0xD6, 0xF4, 0x6B, 0xFC, 0x0D, 0xDB, 0x2E, 0x1A,
        0xB6, 0xE4, 0xB4, 0x5A, 0x99, 0x06, 0xB8, 0x2E, 0x37, 0xF0, 0x5D, 0x6F, 0x97, 0xF6,
        0xA3, 0xEB, 0x6E, 0x18, 0x20, 0x79, 0x75, 0x9C, 0x4F, 0x68, 0x47, 0x83, 0x7B, 0x62,
        0x32, 0x1A, 0xC1, 0xB4, 0xFA, 0x68, 0x64, 0x1F, 0xCB, 0x4B, 0xB9, 0x8D, 0xD6, 0x97,
        0xA0, 0xC7, 0x36, 0x41, 0x38, 0x5F, 0x4B, 0xAB, 0x25, 0xB7, 0x93, 0x58, 0x4C, 0xC3,
        0x9F, 0xC8, 0xD4, 0x8D, 0x4B, 0xD8, 0x67, 0xA9, 0xA3, 0xC1, 0x0F, 0x8E, 0xA1, 0x21,
        0x70, 0x26, 0x8E, 0x34, 0xFE, 0x3B, 0xBE, 0x6F, 0xF8, 0x99, 0x98, 0xD6, 0x0D, 0xA2,
        0xF3, 0xE4, 0x28, 0x3C, 0xBE, 0xC1, 0x39, 0x3D, 0x52, 0xAF, 0x72, 0x4A, 0x57, 0x23,
        0x0C, 0x60, 0x4E, 0x9F, 0xBC, 0xE5, 0x83, 0xD7, 0x61, 0x3E, 0x6B, 0xFF, 0xD6, 0x75,
        0x96, 0xAD, 0x12, 0x1A, 0x87, 0x07, 0xEE, 0xC4, 0x69, 0x44, 0x95, 0x70, 0x33, 0x68,
        0x6A, 0x15, 0x5F, 0x64, 0x4D, 0x5C, 0x58, 0x63, 0xB4, 0x8F, 0x61, 0xBD, 0xBF, 0x19,
        0xA5, 0x3E, 0xAB, 0x6D, 0xAD, 0x0A, 0x18, 0x6B, 0x8C, 0x15, 0x2E, 0x5F, 0x5D, 0x8C,
        0xAD, 0x4B, 0x0E, 0xF8, 0xAA, 0x4E, 0xA5, 0x00, 0x88, 0x34, 0xC3, 0xCD, 0x34, 0x2E,
        0x5E, 0x0F, 0x16, 0x7A, 0xD0, 0x45, 0x92, 0xCD, 0x8B, 0xD2, 0x79, 0x63, 0x93, 0x98,
        0xEF, 0x9E, 0x11, 0x4D, 0xFA, 0xAA, 0xB9, 0x19, 0xE1, 0x4E, 0x85, 0x09, 0x89, 0x22,
        0x4D, 0xDD, 0x98, 0x57, 0x6D, 0x79, 0x38, 0x5D, 0x22, 0x10, 0x90, 0x2E, 0x9F, 0x9B,
        0x1F, 0x2D, 0x86, 0xCF, 0xA4, 0x7E, 0xE2, 0x44, 0x63, 0x54, 0x65, 0xF7, 0x10, 0x58,
        0x42, 0x1A, 0x01, 0x84, 0xBE, 0x51, 0xDD, 0x10, 0xCC, 0x9D, 0x07, 0x9E, 0x6F, 0x16,
        0x04, 0xE7, 0xAA, 0x9B, 0x7C, 0xF7, 0x88, 0x3C, 0x7D, 0x4C, 0xE1, 0x2B, 0x06, 0xEB,
        0xE1, 0x60, 0x81, 0xE2, 0x3F, 0x27, 0xA2, 0x31, 0xD1, 0x84, 0x32, 0xD7, 0xD1, 0xBB,
        0x55, 0xC2, 0x8A, 0xE2, 0x1F, 0xFC, 0xF0, 0x05, 0xF5, 0x75, 0x28, 0xD1, 0x5A, 0x88,
        0x88, 0x1B, 0xB3, 0xBB, 0xB7, 0xFE
    };
    
    if (memcmp(&srp_3072_A, bytes_A, len_A) != 0) {
        return -1;
    }
    
    ver = srp_verifier_new(alg,
                           ng_type,
                           username,
                           (unsigned char *)srp_3072_salt,
                           len_s,
                           bytes_v,
                           len_v,
                           bytes_A,
                           len_A,
                           (unsigned char *)srp_3072_b,
                           sizeof(srp_3072_b),
                           &bytes_B,
                           &len_B,
                           NULL,
                           NULL);
    
    
    *serverKeyProof = bytes_B;
    *proof_len = len_B;
    
    return 0;
    
}

void SRPClient::createSaltedVerificationKey(uint8_t * salt, uint8_t * verificationKey )
{
//    let salt = salt ?? Data(bytes: try! Random.generate(byteCount: 16))
    salt = (uint8_t *)malloc(16 + 20);
    uint8_t defaultSalt[16] = {0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E, 0xB5, 0xA7, 0x27, 0x67, 0x3A, 0x24, 0x41, 0xEE};
    memcpy(salt, defaultSalt, 16);
    
 
    uint8_t * key = (uint8_t *)malloc(20);
    static char namePass[18] = "alice:password123";
    crypto_hash_sha1((uint8_t *)key, (uint8_t *)namePass, 17);
    
    memcpy(salt+16, key, 16);
    crypto_hash_sha1((uint8_t *)key, (uint8_t *)salt, 16 + 20);
    
    
//    let x = calculate_x(algorithm: algorithm, salt: salt, username: username, password: password)
    
//    let v = calculate_v(group: group, x: x)

}



// SHA512...
// https://github.com/jedisct1/libsodium/src/libsodium/crypto_hash/sha512/cp/hash_sha512_cp.c

/*-
 * Copyright 2005,2007,2009 Colin Percival
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#define crypto_hash_sha512_BYTES 64
//#define NATIVE_BIG_ENDIAN 1
typedef struct crypto_hash_sha512_state {
    uint64_t state[8];
    uint64_t count[2];
    uint8_t  buf[128];
} crypto_hash_sha512_state_t;

#define ROTR64(X, B) rotr64((X), (B))
static inline uint64_t
rotr64(const uint64_t x, const int b)
{
    return (x >> b) | (x << (64 - b));
}

#define STORE64_BE(DST, W) store64_be((DST), (W))
static inline void
store64_be(uint8_t dst[8], uint64_t w)
{
#ifdef NATIVE_BIG_ENDIAN
    memcpy(dst, &w, sizeof w);
#else
    dst[7] = (uint8_t) w; w >>= 8;
    dst[6] = (uint8_t) w; w >>= 8;
    dst[5] = (uint8_t) w; w >>= 8;
    dst[4] = (uint8_t) w; w >>= 8;
    dst[3] = (uint8_t) w; w >>= 8;
    dst[2] = (uint8_t) w; w >>= 8;
    dst[1] = (uint8_t) w; w >>= 8;
    dst[0] = (uint8_t) w;
#endif
}

#define LOAD64_BE(SRC) load64_be(SRC)
static inline uint64_t
load64_be(const uint8_t src[8])
{
#ifdef NATIVE_BIG_ENDIAN
    uint64_t w;
    memcpy(&w, src, sizeof w);
    return w;
#else
    uint64_t w = (uint64_t) src[7];
    w |= (uint64_t) src[6] <<  8;
    w |= (uint64_t) src[5] << 16;
    w |= (uint64_t) src[4] << 24;
    w |= (uint64_t) src[3] << 32;
    w |= (uint64_t) src[2] << 40;
    w |= (uint64_t) src[1] << 48;
    w |= (uint64_t) src[0] << 56;
    return w;
#endif
}

void
sodium_memzero(void *const pnt, const size_t len)
{
    volatile unsigned char *volatile pnt_ =
    (volatile unsigned char *volatile) pnt;
    size_t i = (size_t) 0U;
    
    while (i < len) {
        pnt_[i++] = 0U;
    }
}

static void
be64enc_vect(unsigned char *dst, const uint64_t *src, size_t len)
{
    size_t i;
    
    for (i = 0; i < len / 8; i++) {
        STORE64_BE(dst + i * 8, src[i]);
    }
}

static void
be64dec_vect(uint64_t *dst, const unsigned char *src, size_t len)
{
    size_t i;
    uint64_t w;

    for (i = 0; i < len / 8; i++) {
        dst[i] = LOAD64_BE(src + i * 8);
    }
}

static const uint64_t Krnd[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

#define Ch(x, y, z) ((x & (y ^ z)) ^ z)
#define Maj(x, y, z) ((x & (y | z)) | (y & z))
#define SHR(x, n) (x >> n)
#define ROTR(x, n) ROTR64(x, n)
#define S0(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define S1(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define s0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7))
#define s1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6))

#define RND(a, b, c, d, e, f, g, h, k) \
h += S1(e) + Ch(e, f, g) + k;      \
d += h;                            \
h += S0(a) + Maj(a, b, c);

#define RNDr(S, W, i, ii)                                                   \
RND(S[(80 - i) % 8], S[(81 - i) % 8], S[(82 - i) % 8], S[(83 - i) % 8], \
S[(84 - i) % 8], S[(85 - i) % 8], S[(86 - i) % 8], S[(87 - i) % 8], \
W[i + ii] + Krnd[i + ii])

#define MSCH(W, ii, i) \
W[i + ii + 16] =   \
s1(W[i + ii + 14]) + W[i + ii + 9] + s0(W[i + ii + 1]) + W[i + ii]

static void
SHA512_Transform(uint64_t *state, const uint8_t block[128], uint64_t W[80],
                 uint64_t S[8])
{
    int i;
    
    be64dec_vect(W, block, 128);
    memcpy(S, state, 64);
    for (i = 0; i < 80; i += 16) {
        RNDr(S, W, 0, i);
        RNDr(S, W, 1, i);
        RNDr(S, W, 2, i);
        RNDr(S, W, 3, i);
        RNDr(S, W, 4, i);
        RNDr(S, W, 5, i);
        RNDr(S, W, 6, i);
        RNDr(S, W, 7, i);
        RNDr(S, W, 8, i);
        RNDr(S, W, 9, i);
        RNDr(S, W, 10, i);
        RNDr(S, W, 11, i);
        RNDr(S, W, 12, i);
        RNDr(S, W, 13, i);
        RNDr(S, W, 14, i);
        RNDr(S, W, 15, i);
        if (i == 64) {
            break;
        }
        MSCH(W, 0, i);
        MSCH(W, 1, i);
        MSCH(W, 2, i);
        MSCH(W, 3, i);
        MSCH(W, 4, i);
        MSCH(W, 5, i);
        MSCH(W, 6, i);
        MSCH(W, 7, i);
        MSCH(W, 8, i);
        MSCH(W, 9, i);
        MSCH(W, 10, i);
        MSCH(W, 11, i);
        MSCH(W, 12, i);
        MSCH(W, 13, i);
        MSCH(W, 14, i);
        MSCH(W, 15, i);
    }
    for (i = 0; i < 8; i++) {
        state[i] += S[i];
    }
}

static const uint8_t PAD[128] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void
SHA512_Pad(crypto_hash_sha512_state *state, uint64_t tmp64[80 + 8])
{
    uint64_t r;
    uint64_t i;
    
    r = (state->count[1] >> 3) & 0x7f;
    if (r < 112) {
        for (i = 0; i < 112 - r; i++) {
            state->buf[r + i] = PAD[i];
        }
    } else {
        for (i = 0; i < 128 - r; i++) {
            state->buf[r + i] = PAD[i];
        }
        SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
        memset(&state->buf[0], 0, 112);
    }
    be64enc_vect(&state->buf[112], state->count, 16);
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
}

int
crypto_hash_sha512_init(crypto_hash_sha512_state *state)
{
    static const uint64_t sha512_initial_state[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
        0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };
    
    state->count[0] = state->count[1] = (uint64_t) 0U;
    memcpy(state->state, sha512_initial_state, sizeof sha512_initial_state);
    
    return 0;
}


int
crypto_hash_sha512_update(crypto_hash_sha512_state *state,
                          const unsigned char *in, unsigned long long inlen)
{
    uint64_t           tmp64[80 + 8];
    uint64_t           bitlen[2];
    unsigned long long i;
    unsigned long long r;
    
    if (inlen <= 0U) {
        return 0;
    }
    r = (unsigned long long) ((state->count[1] >> 3) & 0x7f);
    
    bitlen[1] = ((uint64_t) inlen) << 3;
    bitlen[0] = ((uint64_t) inlen) >> 61;
    if ((state->count[1] += bitlen[1]) < bitlen[1]) {
        state->count[0]++;
    }
    state->count[0] += bitlen[0];
    if (inlen < 128 - r) {
        for (i = 0; i < inlen; i++) {
            state->buf[r + i] = in[i];
        }
        return 0;
    }
    for (i = 0; i < 128 - r; i++) {
        state->buf[r + i] = in[i];
    }
    SHA512_Transform(state->state, state->buf, &tmp64[0], &tmp64[80]);
    in += 128 - r;
    inlen -= 128 - r;
    
    while (inlen >= 128) {
        SHA512_Transform(state->state, in, &tmp64[0], &tmp64[80]);
        in += 128;
        inlen -= 128;
    }
    inlen &= 127;
    for (i = 0; i < inlen; i++) {
        state->buf[i] = in[i];
    }
    sodium_memzero((void *) tmp64, sizeof tmp64);
    
    return 0;
}

int
crypto_hash_sha512_final(crypto_hash_sha512_state *state, unsigned char *out)
{
    uint64_t tmp64[80 + 8];
    
    SHA512_Pad(state, tmp64);
    be64enc_vect(out, state->state, 64);
    sodium_memzero((void *) tmp64, sizeof tmp64);
    sodium_memzero((void *) state, sizeof *state);
    
    return 0;
}

int SRPClient::crypto_hash_sha512(unsigned char *out, const unsigned char *in, unsigned long long inlen)
{
    
    crypto_hash_sha512_state state;
    
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, in, inlen);
    crypto_hash_sha512_final(&state, out);
    
    return 0;
    
}

// SHA1

/*
 * @file sha1.c
 * @date 20.05.2015
 * @author Steve Reid <steve@edmweb.com>
 *
 * from: http://www.virtualbox.org/svn/vbox/trunk/src/recompiler/tests/sha1.c
 */

/* from valgrind tests */

/* ================ sha1.c ================ */

/* ================ sha1.c ================ */
/*
 SHA-1 in C
 By Steve Reid <steve@edmweb.com>
 100% Public Domain
 Test Vectors (from FIPS PUB 180-1)
 "abc"
 A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 A million repetitions of "a"
 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
 */

#define SHA1HANDSOFF

/* #define LITTLE_ENDIAN * This should be #define'd already, if true. */

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
|(rol(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#error "Endianness not defined!"
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);


/* Hash a single 512-bit block. This is the core of the algorithm. */

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;


void SHA1Transform(uint32_t state[5], uint8_t buffer[64])
{
    uint32_t a, b, c, d, e;
    typedef union {
        unsigned char c[64];
        uint32_t l[16];
    } CHAR64LONG16;
#ifdef SHA1HANDSOFF
    CHAR64LONG16 block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, 64);
#else
    /* The following had better never be used because it causes the
     * pointer-to-const buffer to be cast into a pointer to non-const.
     * And the result is written through.  I threw a "const" in, hoping
     * this will cause a diagnostic.
     */
    CHAR64LONG16* block = (const CHAR64LONG16*)buffer;
#endif
    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
    memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(SHA1_CTX* context)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

void SHA1Update(SHA1_CTX* context, uint8_t* data, uint32_t len)
{
    uint32_t i;
    uint32_t j;
    
    j = context->count[0];
    if ((context->count[0] += len << 3) < j)
        context->count[1]++;
    context->count[1] += (len>>29);
    j = (j >> 3) & 63;
    if ((j + len) > 63) {
        memcpy(&context->buffer[j], data, (i = 64-j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < len; i += 64) {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
    unsigned i;
    unsigned char finalcount[8];
    unsigned char c;
    
#if 0   /* untested "improvement" by DHR */
    /* Convert context->count to a sequence of bytes
     * in finalcount.  Second element first, but
     * big-endian order within element.
     * But we do it all backwards.
     */
    unsigned char *fcp = &finalcount[8];
    for (i = 0; i < 2; i++)
    {
        uint32_t t = context->count[i];
        int j;
        for (j = 0; j < 4; t >>= 8, j++)
            *--fcp = (unsigned char) t;
    }
#else
    for (i = 0; i < 8; i++) {
        finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                         >> ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
    }
#endif
    c = 0200;
    SHA1Update(context, &c, 1);
    while ((context->count[0] & 504) != 448) {
        c = 0000;
        SHA1Update(context, &c, 1);
    }
    SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++) {
        digest[i] = (unsigned char)
        ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    /* Wipe variables */
    memset(context, '\0', sizeof(*context));
    memset(&finalcount, '\0', sizeof(finalcount));
}
/* ================ end of sha1.c ================ */

int SRPClient::crypto_hash_sha1(unsigned char *out, const unsigned char *in, unsigned long long inlen)
{
    // Fixed 20 byte out size
    
    SHA1_CTX ctx;

    SHA1Init(&ctx);
    SHA1Update(&ctx, (uint8_t *)in, (uint32_t)inlen);
    SHA1Final(out, &ctx);

    return 0;
}
