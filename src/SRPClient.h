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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

class SRPClient
{
public:
    SRPClient();
    ~SRPClient();
    
    void createSaltedVerificationKey(uint8_t * salt, uint8_t * verificationKey );
    int crypto_hash_sha512(unsigned char *out, const unsigned char *in, unsigned long long inlen);

private:
    uint8_t * _salt;
    uint8_t _username[6] = "alice";
    uint8_t _password[12] = "password123";
    
};
#endif /* SRPClient_H */
