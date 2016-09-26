#include <EEPROM.h>

int addrH = 0;
int addrM = 1;

void setup() 
{
        Serial.begin(9600);
}

void loop() 
{
        Serial.print(EEPROM.read(addrH));
        Serial.print("  ");
        Serial.println(EEPROM.read(addrM));
        
        delay(100);
}
