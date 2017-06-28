//
//  WebClient.hpp
//  Homekit
//
//  Created by d. nye on 6/27/17.
//  Copyright © 2017 Mobile Flow LLC. All rights reserved.
//  Based on original code https://github.com/ilias-pat/arduweb
//

#ifndef WebClient_H
#define WebClient_H

#if defined(PARTICLE)
#   include "application.h"
#   include "Particle.h"
#else
#   include "test.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#define WEB_CLIENT_BUFFER_SIZE		( 0x20 )

#define HTTP_VER_1_0					( 0x10 )
#define HTTP_VER_1_1					( 0x11 )
#define HTTP_VER_INVALID				( -1 )

#define HTTP_METHOD_HEAD				( 0x00 )
#define HTTP_METHOD_GET					( 0x01 )
#define HTTP_METHOD_POST				( 0x02 )
#define HTTP_METHOD_PUT                 ( 0x03 )
#define HTTP_METHOD_INVALID				( -1 )

#define HTTP_STATUS_200_OK				( 200 )
#define HTTP_STATUS_400_BAD_REQ			( 400 )
#define HTTP_STATUS_404_NOT_FOUND		( 404 )
#define HTTP_STATUS_INVALID				( -1 )

class WebClient
{
public:
    WebClient( void );
    WebClient( const TCPClient client );
    
    uint8_t writeHTTPReqHeader( int method, const char* url, int contentLen );
    uint8_t readHTTPReqHeader( int* method, char* url, int* contentLen );
    uint8_t writeHTTPRespHeader( int status, const char* mime_type, int contentLen );
    uint8_t readHTTPRespHeader( int* status, char* mime_type, int* contentLen );
    uint8_t waitForResponse( int timeout ); // in msecs
    size_t write_pgm( const char* str );
    
private:
    int getHTTPMethodFromStr( const char* str );
    int getHTTPVersionFromStr( const char* str );
    int getHTTPStatusFromStr( const char* str );
    int decodeUrl( char *str );
    const char* getNextWordUntilNewLine( void );
    
    uint8_t mBuffer[WEB_CLIENT_BUFFER_SIZE+1];
    int mBufferIndex;
    
    TCPClient client;
};


#endif /* WebClient */
