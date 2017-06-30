//
//  SRPClient.hpp
//  Homekit
//
//  Created by d. nye on 6/28/17.
//  Copyright © 2017 Mobile Flow LLC. All rights reserved.
//

#ifndef SRPClient_H
#define SRPClient_H

#if defined(PARTICLE)
#   include "application.h"
#endif

#include "srp.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

extern "C" SRP_Result srp_create_salted_verification_key(SRP_HashAlgorithm alg,
                                                         SRP_NGType ng_type, const char *username_for_verifier,
                                                         const unsigned char *password, size_t len_password,
                                                         unsigned char **bytes_s,  size_t *len_s,
                                                         unsigned char **bytes_v, size_t *len_v,
                                                         const char *n_hex, const char *g_hex);


extern "C" struct SRPVerifier* srp_verifier_new(SRP_HashAlgorithm alg, SRP_NGType ng_type,
                                                const char *username,
                                                const unsigned char *bytes_s, size_t len_s,
                                                const unsigned char *bytes_v, size_t len_v,
                                                const unsigned char *bytes_A, size_t len_A,
                                                const unsigned char *bytes_b, size_t len_b,
                                                unsigned char** bytes_B, size_t *len_B,
                                                const char* n_hex, const char* g_hex);

extern "C" struct SRPUser *srp_user_new(SRP_HashAlgorithm alg, SRP_NGType ng_type,
                                        const char *username, const char *username_for_verifier,
                                        const unsigned char *bytes_password, size_t len_password, const char *n_hex,
                                        const char *g_hex);

extern "C" SRP_Result srp_user_start_authentication(struct SRPUser* usr, char **username,
                                                    const unsigned char *bytes_a, size_t len_a,
                                                    unsigned char **bytes_A, size_t* len_A);



class SRPClient
{
public:
    SRPClient();
    ~SRPClient();
    
    int getChallenge(uint8_t **salt,uint16_t *salt_len, uint8_t **key, uint16_t *key_len);
    
    int verifySession(uint8_t **serverKeyProof, uint16_t *proof_len, uint8_t *clientPublicKey, uint16_t client_key_len, uint8_t *clientKeyProof, uint16_t client_proof_len);

    void createSaltedVerificationKey(uint8_t * salt, uint8_t * verificationKey );
    int crypto_hash_sha512(unsigned char *out, const unsigned char *in, unsigned long long inlen);
    int crypto_hash_sha1(unsigned char *out, const unsigned char *in, unsigned long long inlen);
    

private:
    uint8_t * _salt;
    uint8_t _username[6] = "alice";
    uint8_t _password[12] = "password123";
    
};
#endif /* SRPClient_H */
