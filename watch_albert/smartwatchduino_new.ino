/*
Name:		smartwatchduino.ino
Created:	9/27/2015 12:23:55 AM
Author:	Albert
*/

#include <Vcc.h>
const float VccCorrection = 3.8 / 3.87;  // Measured Vcc by multimeter divided by reported Vcc
Vcc vcc(VccCorrection);

#include <MemoryFree.h>//library for RAM usage information

#include <avr/interrupt.h>//library for using interrupts (used to wake up from sleep mode)
#include <avr/power.h>//Power Reduction Management library
#include <avr/sleep.h>//library for enabling the sleep mode

#include <virtuabotixRTC.h>//DS1302 RTC library 
virtuabotixRTC RTC(3, 4, 7);// Creation of the Real Time Clock Object:  CLK, DAT, RST


#include <toneAC.h>//Replacement to the standard Arduino tone library with twice the volume, higher quality and higher frequency
uint8_t globalVolume = 10;

#include <SPI.h>
#include <Adafruit_GFX.h>//adafruit graphics library (for the graphic lcd)
#include <Adafruit_PCD8544.h>//PCD8544 (controller in nokia 3310/nokia 5110 lcds) plugin for the adafruit gfx library
//using softare SPI for LCD:
#define LCDSCLK	13 // pin 13 - Serial clock out (SCLK)
#define	LCDDIN 11 // pin 11 - Serial data out (DIN)
#define	LCDDC 14 // pin 5 - Data/Command select (D/C)
#define LCDCS 15 // pin 4 - LCD chip select (CS)
#define LCDRST 16 // pin 6 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(LCDSCLK, LCDDIN, LCDDC, LCDCS, LCDRST);//creation of graphic lcd object (SCLK, DIN, D/C, CS, RST)
uint8_t StartupContrast = 50;


	   
//pin definitions:
#define BLpin 5 //LCD backlight control pin (must be PWM)
#define flashlightPin 6//pin used for controlling the white flashlight LED (must be PWM)

#define BTN1 2 //button 1 (must be an external interrupt pin)
#define BTN2 19 //button 2
#define BTN3 17 //button 3
#define BTN4 8 //button 4

#define LCDPowerPin 20//pin used for controlling power supply of LCD


bool BLstate, flashlightState;


bool checkButtonState(int BTN) {
	if (digitalRead(BTN))
		return 0;
	else
		return 1;
}

void LEDfadeOUT(int numPin, int x) {
	for (int i = 0; i <= 255; i++) {
		analogWrite(numPin, i);
		delay(x);
	}
	digitalWrite(numPin, HIGH);
}

void LEDfadeIN(int numPin, int x) {
	for (int i = 255; i > 0; i--) {
		analogWrite(numPin, i);
		delay(x);
	}
	digitalWrite(numPin, LOW);
}

void setup() {
	//make_time(&time, 014, 6, 27, 18, 45, 00);
	//ds1337_write_time(&time);

	Serial.begin(9600);

	// seconds, minutes, hours, day of the week, day of the month, month, year
	//RTC.setDS1302Time(00, 30, 3, 4, 30, 6, 2016);

	////pin initialisation:
	//pinMode(LEDR, OUTPUT); digitalWrite(LEDR, HIGH);
	//pinMode(LEDG, OUTPUT); digitalWrite(LEDG, HIGH);
	//pinMode(LEDB, OUTPUT); digitalWrite(LEDB, HIGH);

	pinMode(BTN1, INPUT_PULLUP);
	pinMode(BTN2, INPUT_PULLUP);
	pinMode(BTN3, INPUT_PULLUP);
	pinMode(BTN4, INPUT_PULLUP);


	pinMode(LCDPowerPin, OUTPUT); digitalWrite(LCDPowerPin, HIGH);
	pinMode(BLpin, OUTPUT); digitalWrite(BLpin, LOW);
	pinMode(flashlightPin, OUTPUT); digitalWrite(flashlightPin, LOW);
	flashlightState = 0;

	display.begin(); //display initialisation
	display.setContrast(StartupContrast); //set display contrast
	display.setTextSize(1);
	display.clearDisplay();
	display.display();

	toneAC(100, globalVolume, 100, true);
	delay(100);
	toneAC(200, globalVolume, 100, true);
	delay(100);
	toneAC(300, globalVolume, 100, true);
	delay(100);

	LEDfadeIN(BLpin, 1);
	//analogWrite(BLpin, 150);
	BLstate = true;
	display.display();

	Serial.println(F("initialised"));

}

unsigned long timer1Millis = millis();
bool checkTimer1(uint16_t time) {
	if (millis() - timer1Millis >= time) {
		resetTimer1();
		return 1;
	}
	else {
		return 0;
	}
}
void resetTimer1() {
	timer1Millis = millis();
}
unsigned long timer2Millis = millis();
bool checkTimer2(uint16_t time) {
	if (millis() - timer2Millis >= time) {
		resetTimer2();
		return 1;
	}
	else {
		return 0;
	}
}
void resetTimer2() {
	timer1Millis = millis();
}

#define numScreens 2 //number of screens in the menu
uint8_t curScreen = 1;

void loop() {


	if (checkButtonState(BTN1) == 1) {
		toneAC(200, globalVolume, 50, true);
		delay(50);
		toneAC(400, globalVolume, 50, true);
		while (checkButtonState(BTN1) == 1) {}
		delay(20);
		sleepNow();
	}
	if (checkButtonState(BTN2) == 1) {
		toneAC(200, globalVolume, 50, true);
		delay(50);
		toneAC(400, globalVolume, 50, true);
		while (checkButtonState(BTN2) == 1) {}
		delay(20);
		curScreen++;
		if (curScreen == numScreens + 1)
			curScreen = 1;
	}
	if (checkButtonState(BTN3) == 1) {
		toneAC(300, globalVolume, 50, true);
		delay(50);
		toneAC(500, globalVolume, 50, true);
		while (checkButtonState(BTN3) == 1) {}
		delay(20);
		curScreen--;
		if (curScreen == 0)
			curScreen = numScreens;
	}
	if (checkButtonState(BTN4) == 1) {
		delay(50);
		resetTimer1();
		while (1) {
			if (checkButtonState(BTN4) == 0) {
				toneAC(800, globalVolume, 50, true);
				delay(50);
				toneAC(1000, globalVolume, 50, true);
				delay(50);
				toneAC(1200, globalVolume, 50, true);
				if (BLstate == true)
					LEDfadeOUT(BLpin, 1);
				else
					LEDfadeIN(BLpin, 1);
				BLstate = !BLstate;
				break;
			}
			if (checkTimer1(1000)) {
				LEDfadeIN(flashlightPin, 1);
				if (BLstate == true)
					LEDfadeOUT(flashlightPin, 1);
				else
					LEDfadeIN(flashlightPin, 1);
				flashlightState = !flashlightState;
				//setTimeMenu();
				delay(50);
				while (checkButtonState(BTN4) == 1) {}
				break;
			}
		}
	}

	switch (curScreen) {
	case 1:
		if (checkTimer1(100)) {
			display.clearDisplay();
			RTC.updateTime();

			display.setTextSize(2);
			display.setCursor(2, 6);
			display.print(RTC.hours);
			display.print(F(":"));
			display.print(RTC.minutes);
			display.setTextSize(1);
			display.print(F(":"));
			display.print(RTC.seconds);
			display.setTextSize(2);
			display.setCursor(2, 24);
			//display.print("\n");
			display.print(RTC.dayofmonth);
			display.print(F("/"));
			display.print(RTC.month);
			display.setTextSize(1);
			display.print(F("/"));
			display.println(RTC.year);

			display.setTextSize(1);
			display.print(F("Vcc="));
			display.print(vcc.Read_Volts());
			display.print(F("V"));

			display.display();
		}
		break;

	case 2:
		display.clearDisplay();
		display.setTextSize(1);
		display.print("settings\n\nhold button 3\n for editing");
		display.display();
		break;
	}
}

void sleepNow(void)//fuction for enabling sleep mode
{
	attachInterrupt(0, pinInterrupt, LOW);// Set pin 2 as interrupt and attach handler
	display.clearDisplay();
	display.print(F("sleep mode\nactivated"));
	display.display();
	LEDfadeOUT(BLpin, 1);
	digitalWrite(LCDPowerPin, LOW);//turn peripheral devices off when going to sleep
	
	digitalWrite(LCDSCLK, LOW);
	digitalWrite(LCDDIN, LOW);
	digitalWrite(LCDRST, LOW);
	digitalWrite(LCDDC, LOW);
	digitalWrite(LCDCS, LOW);
	digitalWrite(BTN2, LOW);
	digitalWrite(BTN3, LOW);
	digitalWrite(BTN4, LOW);
	digitalWrite(0, LOW);
	digitalWrite(1, LOW);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);// Choose our preferred sleep mode	
	sleep_enable();// Set sleep enable (SE) bit
	sleep_mode();// Put the device to sleep	

	sleep_disable();//  <--- Upon waking up, sketch continues from this point.

	digitalWrite(LCDPowerPin, HIGH);//turn peripheral devices on when waking up from sleep mode
	digitalWrite(BTN2, HIGH);
	digitalWrite(BTN3, HIGH);
	digitalWrite(BTN4, HIGH);

	display.clearDisplay();
	display.begin();
	display.setContrast(StartupContrast);
	LEDfadeIN(BLpin, 1);
}
void pinInterrupt(void)//pin interrupt funcion used in attachInterrupt function (when enabling the sleep mode)
{
	detachInterrupt(0);
}
/*
Programming notes:
-string is defined as "static char stringName [size];"
-comment CTRL+k+c, uncomment CTRL+k+u
*/
