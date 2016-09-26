#include <EEPROM.h>

int addrH = 0;
int addrM = 1;

void setup() 
{
        Serial.begin(9600);
}   

void loop() 
{
        EEPROM.write(addrH, 22);
        EEPROM.write(addrM, 41);
        
        delay(100);
}
