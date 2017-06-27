// Basic c++ test bench for testing Particle C code
/*
   g++ test.cpp tlv8.cpp -Wall -ggdb -std=c++0x -I. -O3 -fpermissive
   gcc2minix < a.out >test
   chmod +x test   # may not be necessary depending on your umask
   ./test

   Use online HEXDump tools to debug output: https://hexed.it

*/

#include "src/homekit.h"

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


