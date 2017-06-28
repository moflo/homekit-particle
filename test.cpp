// Basic c++ test bench for testing Particle C code
/*
   g++ test.cpp tlv8.cpp -Wall -ggdb -std=c++0x -I. -O3 -fpermissive
   gcc2minix < a.out >test
   chmod +x test   # may not be necessary depending on your umask
   ./test

   Use online HEXDump tools to debug output: https://hexed.it

*/

#include "src/homekit.h"
#include "src/WebClient.h"
#include "src/TLV8.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

using namespace std;


void print_hex_memory(void *mem, int count) {
  int i;
  unsigned char *p = (unsigned char *)mem;
  for (i=0; i < count; i++) {
    printf("0x%02x ", p[i]);
    if ((i%16==0) && i)
      printf("\n");
  }
  printf("\n");
}

void assertNotEqual(uint8_t mem1, uint8_t mem2, const char * description, int * error_count ) {
    
    if (mem1 != mem2) {
        cout << "Error - " << description << ", got : " << (unsigned)mem1 << endl;
        (*error_count)++;
    }
}

int main()
{
    cout << "Starting Test.cpp ...\n\n" << endl;
    
    try {

        int error_count = 0;
        
        {
            cout << "Initialize - Homekit object default test ..." << endl;
            
            
            Homekit homekit = Homekit();
            
            assertNotEqual(lightbulbType, homekit.getType(), "type not equal", &error_count);
            
            uint8_t len = strlen(homekit.getName());
            
            assertNotEqual(len, 8, "name not equal", &error_count);
            
        }
        {
            cout << "Initialize - Homekit type object test ..." << endl;
            

            Homekit homekit = Homekit( fanType );
            
            assertNotEqual(fanType, homekit.getType(), "type not equal", &error_count);
            
            uint8_t len = strlen(homekit.getName());
            
            assertNotEqual(len, 8, "name not equal", &error_count);
            
        }
        {
            cout << "Initialize - Homekit type & name object test ..." << endl;
            
            
            Homekit homekit = Homekit( fanType, "Test" );
            
            assertNotEqual(fanType, homekit.getType(), "type not equal", &error_count);
            
            uint8_t len = strlen(homekit.getName());
            
            assertNotEqual(len, 4, "name not equal", &error_count);
            
        }
        {
            cout << "Initialize - Homekit accessory test ..." << endl;
            
            
            Homekit homekit = Homekit( lightbulbType );
            
            HomekitAccessory lightBulb = homekit.newAccessory( lightbulbType );
            
            assertNotEqual(lightbulbType, lightBulb.type, "type not equal", &error_count);
            
            Characterist *light = homekit.newCharacteristic( onSetting );
            
            assertNotEqual(onSetting, light->type, "type not equal", &error_count);
            
            assertNotEqual(homekit.getCharacteristicCount(), 1, "count not equal", &error_count);
            
            
        }
        {
            cout << "Initialize - Homekit process test ..." << endl;
            
            
            Homekit homekit = Homekit( fanType );
            
            Characterist *light = homekit.newCharacteristic( onSetting );

            TCPClient client;
            
            homekit.process( client );
            
            assertNotEqual(light->intValue, 99, "intValue not equal", &error_count);

            
        }
        {
            cout << "WebClient - initialization test ..." << endl;
            
            
            TCPClient tcpStream;
            const char data[140] = "POST /pair-setup HTTP/1.1\nHost: emulator._hap._tcp.local\nContent-Length: 3\nContent-Type: application/pairing+tlv8\n\r\x01\x01\x01\n\r";
            tcpStream.stream = (char *)data;
            WebClient client = WebClient( tcpStream );
            
            int method = 0;
            char url[80];
            int contentLen = 0;
            
            int result = client.readHTTPReqHeader(&method, url, &contentLen);
            
            assertNotEqual(result, 1, "result not equal", &error_count);
            
            assertNotEqual(method, HTTP_METHOD_POST, "method not equal", &error_count);
            
            assertNotEqual(url[0], '/', "url not equal", &error_count);
            assertNotEqual(url[1], 'p', "url not equal", &error_count);
            assertNotEqual(url[2], 'a', "url not equal", &error_count);
            
            assertNotEqual(contentLen, 3, "contentLen not equal", &error_count);
            
            
        }
        {
            cout << "WebClient - parsing test ..." << endl;
            
            
            TCPClient tcpStream;
            const char data[140] = "GET /pair-setup HTTP/1.1\nHost: emulator._hap._tcp.local\nContent-Length: 4\nContent-Type: application/pairing+tlv8\n\r\x02\x02\x01\x02\n\r";
            tcpStream.stream = (char *)data;
            WebClient client = WebClient( tcpStream );
            
            int method = 0;
            char url[80];
            int contentLen = 0;
            
            int result = client.readHTTPReqHeader(&method, url, &contentLen);
            
            assertNotEqual(result, 1, "result not equal", &error_count);
            
            assertNotEqual(method, HTTP_METHOD_GET, "method not equal", &error_count);
            
            assertNotEqual(url[0], '/', "url not equal", &error_count);
            assertNotEqual(url[1], 'p', "url not equal", &error_count);
            assertNotEqual(url[2], 'a', "url not equal", &error_count);
            
            assertNotEqual(contentLen, 4, "contentLen not equal", &error_count);
            
            
        }
        {
            cout << "WebClient - parsing error test ..." << endl;
            
            
            TCPClient tcpStream;
            const char data[140] = "XXX /pair-setup HTTP/1.1\nHost: emulator._hap._tcp.local\nContent-Length: 4\nContent-Type: application/pairing+tlv8\n\r\x02\x02\x01\x02\n\r";
            tcpStream.stream = (char *)data;
            WebClient client = WebClient( tcpStream );
            
            int method = 0;
            char url[80];
            int contentLen = 0;
            
            int result = client.readHTTPReqHeader(&method, url, &contentLen);
            
            assertNotEqual(result, 0, "result not equal", &error_count);
            
            assertNotEqual(method, HTTP_METHOD_INVALID, "method not equal", &error_count);
            
            assertNotEqual(url[0], 0, "url not equal", &error_count);
            
            assertNotEqual(contentLen, 0, "contentLen not equal", &error_count);
            
            
        }
        {
            cout << "TLV8 - tlv creation test ..." << endl;
            
            tlv_t tlvTest = tlv();
            
            assertNotEqual(tlvTest.type, 0x00, "type not equal", &error_count);
            
        }
        {
            cout << "TLV8 - tlv data test ..." << endl;
            
            tlv_t tlvTest = tlv(kTLVType_State, kTLVType_State_M2);
            
            assertNotEqual(tlvTest.type, kTLVType_State, "type not equal", &error_count);
            assertNotEqual(tlvTest.size, 0x01, "size not equal", &error_count);
            assertNotEqual(tlvTest.data[0], kTLVType_State_M2, "data not equal", &error_count);
            
        }
        {
            cout << "TLV8 - tlv alloc test ..." << endl;
            
            const char string[9] = "DEADBEEF";
            
            tlv_t tlvTest = tlv(kTLVType_Salt, (uint8_t *)string, 8);
            
            assertNotEqual(tlvTest.type, kTLVType_Salt, "type not equal", &error_count);
            assertNotEqual(tlvTest.size, 0x08, "size not equal", &error_count);
            assertNotEqual(tlvTest.data[0], 'D', "data not equal", &error_count);
            assertNotEqual(tlvTest.data[1], 'E', "data not equal", &error_count);
            assertNotEqual(tlvTest.data[2], 'A', "data not equal", &error_count);
            assertNotEqual(tlvTest.data[3], 'D', "data not equal", &error_count);
            
        }
        {
            cout << "TLV8 - tlv_map creation test" << endl;
            
            tlv_map_t response;
            uint8_t key[17] = "DEADBEEFDEADBEEF";
            uint8_t salt[17] = "DEADBEEFDEADBEEF";
            
            response.count = 2;
            response.object[0] = tlv(kTLVType_State, kTLVType_State_M2);
            response.object[1] = tlv(kTLVType_PublicKey_Accessory, key, 16);
            response.object[2] = tlv(kTLVType_Salt, salt, 16);

            assertNotEqual(response.count, 0x03, "count not equal", &error_count);

            assertNotEqual(response.object[0].type, kTLVType_State, "type not equal", &error_count);
            assertNotEqual(response.object[0].size, 0x01, "size not equal", &error_count);
            assertNotEqual(response.object[0].data[0], kTLVType_State_M2, "data not equal", &error_count);
            
            assertNotEqual(response.object[1].type, kTLVType_PublicKey_Accessory, "type not equal", &error_count);
            assertNotEqual(response.object[1].size, 0x10, "size not equal", &error_count);
            assertNotEqual(response.object[1].data[0], 'D', "data not equal", &error_count);
            
            assertNotEqual(response.object[2].type, kTLVType_Salt, "type not equal", &error_count);
            assertNotEqual(response.object[2].size, 0x10, "size not equal", &error_count);
            assertNotEqual(response.object[2].data[0], 'D', "data not equal", &error_count);
            
        }
        cout << "\n\nError count == " << error_count << endl;
        
        return 1;

    }

    catch (...)
	{
		cout << "unknown exception occured" << endl;
		cout << "!!! TEST VECTORS FAILURE !!!" << endl;
	}

	cout << "end!!! TEST VECTORS FAILURE !!!" << endl;
	return -1;

}


