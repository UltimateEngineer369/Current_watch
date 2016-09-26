#include <SoftwareSerial.h>

SoftwareSerial bt(2, 3);                         //RX AND THEN TX;

String readS;
char endChar = "!";

void setup() 
{
        Serial.begin(9600);
        bt.begin(9600);        
}

void loop() 
{
        if (bt.available()) 
        {
                readS = bt.readStringUntil(endChar);
                
                Serial.println(readS);
                Serial.println("recived");
        }
        if (Serial.available()) 
        {
                readS = Serial.readStringUntil(endChar);
                
                bt.println(readS);
                Serial.println("sent");        
        }

        delay(50);
}
