// A Particle library for TLV encoding, for use with Apple's HomeKit.
// Copyright (c) 2017, mobile flow llc.
// https://github.com/moflo/tlv8-particle

#ifndef _INCL_TLV8
#define _INCL_TLV8

#if defined(PARTICLE)
#   include "application.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


// using namespace std;


#define  TLV8_MAP_SIZE       8

typedef struct tlv
{
    int8_t type;    // type
    uint8_t * data; // pointer to data
    int16_t size;   // size of data

    tlv() {type = 0; size = 0;};
    tlv(int8_t t, int8_t c) {
        type = t;
        size = 1;
        data = (uint8_t *)malloc(1); data = (uint8_t *)memcpy(data,&c,1);
    };
    tlv(int8_t t, uint8_t *d, int16_t s) {
        type = t;
        size = s;
        data = (uint8_t *)malloc(s); data = (uint8_t *)memcpy(data,d,s);
    };

    
} tlv_t;

typedef struct tlv_map
{
    struct tlv object[ TLV8_MAP_SIZE ];
    uint8_t count; // keep track of tlv elements used

} tlv_map_t;

typedef enum tlv_result {

    TLV_SUCESS = 0,
    TLV_ERROR_NULL,
    TLV_ERROR_MAX_LIMIT

} tlv_result_t;

class TLV8Class
{
private:
   uint8_t*     _buffer;

public:
   TLV8Class();
   ~TLV8Class();
   
   tlv_result_t encode(tlv_map_t * map, uint8_t ** stream_ptr, uint16_t * length);
   tlv_result_t decode(uint8_t * stream, uint16_t length, tlv_map_t * map);
   tlv_t        TLVFromBuffer(uint8_t * stream, int16_t type, int16_t size);
   tlv_t        TLVAppendBuffers(uint8_t * stream1, uint8_t * stream2, int16_t type, int16_t size1, int16_t size2);
   tlv_result_t insert(tlv_map_t * map, tlv_t tlv);
   uint8_t      getCount(tlv_map_t map);
   tlv_t        getTLVAtIndex(tlv_map_t map, uint8_t index);
    tlv_result_t TLVFree(tlv_map_t * map);

};

#endif
