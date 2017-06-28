// A Particle library for TLV encoding, for use with Apple's HomeKit.
// Copyright (c) 2017, mobile flow llc.
// https://github.com/moflo/tlv8-particle

#include "TLV8.h"

#define MAX_ITEM_SIZE 0xff

TLV8Class::TLV8Class()
{
    this->_buffer = NULL;
}

TLV8Class::~TLV8Class()
{
}

tlv_result_t TLV8Class::encode(tlv_map_t * map, uint8_t ** stream_ptr, uint16_t * length)
{

    uint8_t * stream = *stream_ptr;
    struct tlv TLV8;
    uint8_t offset = 0;
    uint8_t previous_type = 0xff;   // Should be an unused type code, assume 0xFF
    uint16_t remaining_bytes = 0;

    for (int i = 0; i < map->count; i++) {

        TLV8 = map->object[i];
        uint16_t size = TLV8.size;
        uint8_t type = TLV8.type;
        uint8_t * data = TLV8.data;
        
        // Split encoded object into two or more consecutive segments
        previous_type = type;
        remaining_bytes = size;
        
        while (remaining_bytes > 0) {

            // Initialize or reallocate the stream buffer as needed
            uint16_t data_size = ( remaining_bytes >= MAX_ITEM_SIZE ) ? MAX_ITEM_SIZE : remaining_bytes;
            
            if ( i == 0 )
            {
                uint8_t *mem = (uint8_t *)malloc(data_size + 2);
                if (NULL == mem)
                {
                    return TLV_ERROR_NULL;
                }
                else
                {
                    stream = mem;
                }
            }
            else
            {
                uint8_t *mem = (uint8_t *)realloc(stream, data_size + 2);
                if (NULL == mem)
                {
                    return TLV_ERROR_NULL;
                }
                else
                {
                    stream = mem;
                }
            }
            
            stream[offset] = type;
            stream[offset+1] = data_size;
            memcpy(stream + offset + 2, data, data_size);
            
            offset += data_size + 2;
            
            remaining_bytes = remaining_bytes - data_size;
        }
    }
    
    *length = offset;
    *stream_ptr = stream;
    
   return TLV_SUCESS;

}

tlv_result_t TLV8Class::decode(uint8_t * stream, uint16_t length, tlv_map_t * map){

    uint8_t previous_type = 0xff;   // Should be an unused type code, assume 0xFF
    uint16_t previous_size = 0;
    
    for (int i = 0; i < length; ) {
        struct tlv new_tlv;
        
        uint8_t type = stream[i];
        uint8_t size = stream[i+1];
        uint8_t * data = stream + i + 2;
        
        
        // Check whether the data should be appended
        if ( type == previous_type && previous_size == MAX_ITEM_SIZE ) {
            uint8_t index = map->count - 1;
            uint8_t * old_data = map->object[ index ].data;
            
            new_tlv = TLVAppendBuffers(old_data, data, type, previous_size, size);
            map->object[ index ] = new_tlv;

        }
        else {
            new_tlv = TLVFromBuffer(data, type, size);
            
            uint8_t index = map->count;
            map->object[ index ] = new_tlv;
            map->count++;
        }
        
        // Save previous values
        previous_type = type;
        previous_size = size;
        
        i += size + 2;
        
    }
    
   return TLV_SUCESS;

}


tlv_t TLV8Class::TLVFromBuffer(uint8_t * stream, int16_t type, int16_t size)
{

    struct tlv TLV8;
    memset(&TLV8, 0, sizeof(tlv));
    
    TLV8.type = type;
    TLV8.size = size;
    TLV8.data = (uint8_t *)malloc(size);
    
    memcpy(TLV8.data, stream, size);
    

    return TLV8;

}

tlv_t TLV8Class::TLVAppendBuffers(uint8_t * stream1, uint8_t * stream2, int16_t type, int16_t size1, int16_t size2)
{
    struct tlv TLV8;
    memset(&TLV8, 0, sizeof(tlv));
    
    TLV8.type = type;
    TLV8.size = size1 + size2;
    TLV8.data = (uint8_t *)malloc(size1 + size2);
    
    memcpy(TLV8.data, stream1, size1);
    memcpy(TLV8.data + size1 + 1, stream2, size2);
    
    return TLV8;
}

tlv_result_t TLV8Class::insert(tlv_map_t * map, tlv_t tlv)
{
    uint8_t index = map->count;
    
    if (index == TLV8_MAP_SIZE)
    {
        return TLV_ERROR_MAX_LIMIT;
    }
    
    map->object[ index ] = tlv;
    map->count = index  + 1;
    
   return TLV_SUCESS;

}

uint8_t TLV8Class::getCount(tlv_map_t map)
{
    return map.count;

}

tlv_t TLV8Class::getTLVAtIndex(tlv_map_t map, uint8_t index)
{

    struct tlv TLV8;
    memset(&TLV8, 0, sizeof(tlv));

    if (index <= TLV8_MAP_SIZE)
    {
        TLV8 = map.object[ index ];
    }
    
    return TLV8;

}

tlv_result_t TLV8Class::TLVFree(tlv_map_t * map)
{
    if (map == NULL)
        return TLV_ERROR_NULL;
    
    for(int i = 0; i < map->count; i++)
    {
        free(map->object[i].data);
        
        map->object[i].data = NULL;
    }
    
    return TLV_SUCESS;
}
