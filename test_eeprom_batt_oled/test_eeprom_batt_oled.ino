#include <Wire.h>
#include <EEPROM.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS3231 rtc;

int addrH = 0;
int addrM = 1;

void setup()   
{                
        Serial.begin(9600);
        pinMode(13, OUTPUT);
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
        
         if (rtc.lostPower()) 
         {
                digitalWrite(13, HIGH);
                delay(100);
                digitalWrite(13, LOW);
                delay(100);
                
                rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                // This line sets the RTC with an explicit date & time, for example to set
                // January 21, 2014 at 3am you would call:
                // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        }
      
        display.display();
        delay(2000);

        display.clearDisplay();
}

void loop() 
{
        DateTime now = rtc.now();

        display.setTextSize(4);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        
        if(now.hour() < 10)
        {
                display.print("0");
        }

        display.print(now.hour());

        display.print(":");

        if(now.minute() < 10)
        {
                display.print("0");
        }

        display.print(now.minute());
        
        display.setTextSize(1);
        display.setCursor(0, 50);
        display.print(now.day());
        display.print(" ");
        display.print(now.month());

        display.setCursor(64, 50);
        display.print(now.year());
        
        display.display();
        
        delay(2000);

        EEPROM.write(addrH, now.hour());
        EEPROM.write(addrM, now.minute());
        
        display.clearDisplay();        
}
