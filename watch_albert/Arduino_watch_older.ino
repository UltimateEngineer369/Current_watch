#include <MemoryFree.h>//library for RAM usage information

#include <virtuabotixRTC.h>//DS1302 RTC library 
virtuabotixRTC myRTC(14, 15, 16);// Creation of the Real Time Clock Object:  CLK, DAT, RST

#include <SPI.h>
#include <Adafruit_GFX.h>//adafruit graphics library (for the graphic lcd)
#include <Adafruit_PCD8544.h>//PCD8544 (controller in nokia 3310/nokia 5110 lcds) plugin for the adafruit gfx library
//using softare SPI for LCD:
// pin 13 - Serial clock out (SCLK)
// pin 8 - Serial data out (DIN)
// pin 7 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 5 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 8, 7, 6, 5);//creation of graphic lcd object (SCLK, DIN, D/C, CS, RST)

//pin definitions:
#define BLpin 3 //LCD backlight control pin (must be PWM)

#define SPKRpin 2 //speaker pin

#define BTN0 12 //button 0
#define BTN1 4 //button 1
#define BTN2 17 //button 1

#define LEDR 9 //red led (must be PWM)
#define LEDG 10 //green led (must be PWM)
#define LEDB 11 //blue led (must be PWM)

bool BLstate;

void setup()   {
	Serial.begin(9600);

	//pin initialisation:
	pinMode(LEDR, OUTPUT); digitalWrite(LEDR, HIGH);
	pinMode(LEDG, OUTPUT); digitalWrite(LEDG, HIGH);
	pinMode(LEDB, OUTPUT); digitalWrite(LEDB, HIGH);

	pinMode(BTN0, INPUT); digitalWrite(BTN0, HIGH);
	pinMode(BTN1, INPUT); digitalWrite(BTN1, HIGH);
	pinMode(BTN2, INPUT); digitalWrite(BTN2, HIGH);
	
	pinMode(BLpin, OUTPUT); digitalWrite(BLpin, HIGH);
	pinMode(SPKRpin, OUTPUT);

	display.begin(); //display initialisation
	display.setContrast(55); //set display contrast
	display.setTextSize(1);

	tone(SPKRpin, 100, 100);
	delay(150);
	tone(SPKRpin, 200, 100);
	delay(150);
	tone(SPKRpin, 300, 100);
	delay(150);

	LEDfadeIN(BLpin, 1);
	BLstate = true;
	// seconds, minutes, hours, day of the week, day of the month, month, year
	//myRTC.setDS1302Time(00, 15, 00, 5, 22, 5, 2015);
}


#define numScreens 12 //number of screens in the menu
int i = 1;
int x, y;
int hours;
unsigned long previousMillis1 = 0;


//converter variables:
int number = 15;
bool ConverterEditing = false;
unsigned long previousMillis3 = 0;
int cursor = 1;
bool cursorState = 0;
int ConverterMode = 0;
char DECbuffer[7];
int numLen = 0;


void loop() {
	
	if (digitalRead(BTN0) == LOW){
		delay(50);
		unsigned long previousMillis2 = millis();

		while (1){
			if (digitalRead(BTN0) == HIGH){
				tone(SPKRpin, 800, 50);
				delay(50);
				tone(SPKRpin, 1000, 50);
				delay(50);
				tone(SPKRpin, 1200, 50);
				delay(20);

				BLstate = !BLstate;
				if (BLstate == true)
					LEDfadeIN(BLpin, 1);
				else
					LEDfadeOUT(BLpin, 1);
				break;
			}
			if (millis() - previousMillis2 >= 1000){
				if (i == 12){
					ConverterEditing = !ConverterEditing;
					tone(SPKRpin, 100, 50);
					//delay(50);
					//tone(SPKRpin, 400, 50);
					delay(50);
					tone(SPKRpin, 150, 50);
					ConverterMode = 0;
					break;
				}
			}
		}
		
		//
		//
		//tone(SPKRpin, 800, 50);
		//delay(50);
		//tone(SPKRpin, 1000, 50);
		//delay(50);
		//tone(SPKRpin, 1200, 50);
		//delay(20);
		//
		//if (i == 12){
		//	unsigned long previousMillis2 = millis();
		//	bool skipper = false;
		//	while (digitalRead(BTN0) == LOW){
		//		if (millis() - previousMillis2 >= 1000){
		//			ConverterEditing = !ConverterEditing;
		//			tone(SPKRpin, 200, 50);
		//			delay(50);
		//			tone(SPKRpin, 400, 50);
		//			delay(50);
		//			tone(SPKRpin, 600, 50);
		//			skipper = true;
		//			break;
		//		}
		//	}
		//	if (skipper == false){
		//		delay(20);
		//		BLstate = !BLstate;
		//		if (BLstate == true)
		//			LEDfadeIN(BLpin, 1);
		//		else
		//			LEDfadeOUT(BLpin, 1);
		//	}
		//}
		//else{
		//	while (digitalRead(BTN0) == LOW){}
		//	delay(20);
		//	BLstate = !BLstate;
		//	if (BLstate == true)
		//		LEDfadeIN(BLpin, 1);
		//	else
		//		LEDfadeOUT(BLpin, 1);
		//}
	}
	
	if (digitalRead(BTN1) == LOW){
		tone(SPKRpin, 200, 50);
		delay(50);
		tone(SPKRpin, 400, 50);
		delay(20);
		while (digitalRead(BTN1) == LOW){}
		delay(20);
		i++;
		if (i == numScreens + 1)
			i = 1;
	}
	
	if (digitalRead(BTN2) == LOW){
		tone(SPKRpin, 400, 50);
		delay(50);
		tone(SPKRpin, 200, 50);
		delay(20);
		while (digitalRead(BTN2) == LOW){}
		delay(20);
		i--;
		if (i == 0)
			i = numScreens;
	}

	switch (i){
		
	case 1:

		if (millis() - previousMillis1 >= 100) {
			previousMillis1 = millis();

			display.clearDisplay();
			myRTC.updateTime();

			display.setTextWrap(false);
			display.setTextSize(1);

			display.print(myRTC.hours, DEC);                                                                                                         
			display.print("/");
			display.println(myRTC.minutes, DEC);

			display.print(myRTC.hours, BIN);                                                                                                       
			display.print("/:");
			display.println(myRTC.minutes, BIN);


			display.print(myRTC.hours, HEX);
			display.print("/:");
			display.println(myRTC.minutes, HEX);

			display.print(myRTC.hours, OCT);
			display.print("/:");
			display.println(myRTC.minutes, OCT);

			display.print(DECtoROMAN(myRTC.hours));
			display.print("/:");
			display.println(DECtoROMAN(myRTC.minutes));





			display.setTextWrap(true);

			display.display();
		}
		break;

	case 12:	//converter screen
		
		display.clearDisplay();
		display.setTextSize(2);
		display.println("Converter");
		display.setTextSize(1);
		display.println("/hold BTN0 for editing");
		display.display();




		//if converterEditing==true, enter the editing mode
		while (ConverterEditing == true){
			int i;
			switch (ConverterMode){
				case 0:	
					display.clearDisplay();
					display.setTextSize(1);
					display.println("BINARY:");
					i = 15;
					while (i > cursor && bitRead(number, i) == 0)
						i--;
					while (i >= 0){
						if (i == cursor){
							if (cursorState == 0)
								display.print("_");

							else
								display.print(bitRead(number, i));
						}
						else
							display.print(bitRead(number, i));

						i--;
					}

					display.print("\nDEC:");
					display.print(number, DEC);
					display.print("\nHEX:");
					display.print(number, HEX);
					display.print("\nOCT:");
					display.print(number, OCT);
					display.display();

					if (digitalRead(BTN0) == LOW){
						tone(SPKRpin, 500, 20);
						delay(20);
						tone(SPKRpin, 600, 50);
						while (digitalRead(BTN0) == LOW){}
						delay(20);

						cursor--;
						if (cursor == -1)
							cursor = 15;
					}

					if (digitalRead(BTN1) == LOW){
						unsigned long previousMillis = millis();
						tone(SPKRpin, 600, 20);
						delay(20);
						tone(SPKRpin, 500, 50);
						
						while (digitalRead(BTN1) == LOW){
							if (millis() - previousMillis >= 1000){
								tone(SPKRpin, 100, 50);
								delay(20);
								ConverterMode++;
								if (ConverterMode == 4)
									ConverterMode = 0;
								cursor = 1;
								break;
							}
						}
							delay(20);
							cursor++;
							if (cursor == 16)
								cursor = 0;
						
					}

					if (digitalRead(BTN2) == LOW){
						tone(SPKRpin, 300, 20);
						delay(20);
						tone(SPKRpin, 400, 50);
						while (digitalRead(BTN2) == LOW){}
						delay(20);

						bitWrite(number, cursor, !bitRead(number, cursor));
					}
					break;

				case 1:
					display.clearDisplay();
					display.setTextSize(1);
					display.println("BINARY:");
					display.print(number,BIN);
					display.print("\nDEC:");
					
					dtostrf(number, 6, 0, DECbuffer);
					for (int i = 0; DECbuffer[i] == ' ' && i < 7; i++)
						DECbuffer[i] = '0';
					
					i = 0;
					while (i < cursor && DECbuffer[i] == '0')
						i++;
					//while (i < 6 && DECbuffer[i] == ' '){
					//	DECbuffer[i] == '0';
					//	i++;
					//}

					while (i <= 5){
						if (i == cursor){
							if (cursorState == 0)
								display.print("_");

							else
								display.print(DECbuffer[i]);
						}
						else
							display.print(DECbuffer[i]);

						i++;
					}



					if (digitalRead(BTN0) == LOW){
						tone(SPKRpin, 500, 20);
						delay(20);
						tone(SPKRpin, 600, 50);
						while (digitalRead(BTN0) == LOW){}
						delay(20);

						cursor++;
						if (cursor == 6)
							cursor = 0;
					}

					if (digitalRead(BTN1) == LOW){
						unsigned long previousMillis = millis();
						tone(SPKRpin, 600, 20);
						delay(20);
						tone(SPKRpin, 500, 50);

						while (digitalRead(BTN1) == LOW){
							if (millis() - previousMillis >= 1000){
								tone(SPKRpin, 100, 50);
								delay(20);
								ConverterMode++;
								if (ConverterMode == 4)
									ConverterMode = 0;
								cursor = 1;
								break;
							}
						}
						delay(20);
						cursor--;
						if (cursor == -1)
							cursor = 5;

					}
					
					if (digitalRead(BTN2) == LOW){
						tone(SPKRpin, 300, 20);
						delay(20);
						tone(SPKRpin, 400, 50);
						while (digitalRead(BTN2) == LOW){}
						delay(20);

						DECbuffer[cursor] = DECbuffer[cursor]++;
						if (DECbuffer[cursor]=='9'+1)
							DECbuffer[cursor] = '0';
						
						number = atoi(DECbuffer);
						//number = 0;
						//for (int i = 0; i < 7; i++){
						//	number += (DECbuffer[i]-'0') * pow(10, -1 * (i - 6));
						//}
						
					}

					//display.print(DECbuffer);
					display.print("\nHEX:");
					display.print(number, HEX);
					display.print("\nOCT:");
					display.print(number, OCT);
					display.display();
					break;

				case 2:
					display.clearDisplay();
					display.setTextSize(1);
					display.println("BINARY:");
					display.print(number, BIN);
					display.print("\nDEC:");
					display.print(number, DEC);
					display.print("\nHEX:");
					
					
					char HEXbuffer[7];
					sprintf(HEXbuffer, "%X", number);
					numLen = strlen(HEXbuffer);
					//for (int i = 0; i < cursor - numLen - 1; i++)
					//	HEXbuffer[i] = '0';


					sprintf(HEXbuffer + (7-numLen-1), "%X", number);
					for (int i = 0; i < 7 - numLen - 1; i++)
						HEXbuffer[i] = '0';

					//display.print(HEXbuffer);
					//shiftRight(HEXbuffer);
					display.print(strlen(HEXbuffer));
					

					
					i = 0;
					for (i = 0; HEXbuffer[i] == '0' && i< cursor;i++);
					
					if (cursorState == 0)
						HEXbuffer[cursor] = '_';
					
					display.print(&HEXbuffer[i]);


					//for (int i = 0; DECbuffer[i] == ' ' && i < 7; i++)
					//	DECbuffer[i] = '0';

					//i = 0;
					//while (i < cursor && DECbuffer[i] == '0')
					//	i++;
					////while (i < 6 && DECbuffer[i] == ' '){
					////	DECbuffer[i] == '0';
					////	i++;
					////}

					//while (i <= 5){
					//	if (i == cursor){
					//		if (cursorState == 0)
					//			display.print("_");

					//		else
					//			display.print(DECbuffer[i]);
					//	}
					//	else
					//		display.print(DECbuffer[i]);

					//	i++;
					//}


					//display.print(DECbuffer);

					display.print("\nOCT:");
					display.print(number, OCT);
					display.display();

					if (digitalRead(BTN0) == LOW){
						tone(SPKRpin, 500, 20);
						delay(20);
						tone(SPKRpin, 600, 50);
						while (digitalRead(BTN0) == LOW){}
						delay(20);

						cursor++;
						if (cursor == 6)
							cursor = 0;
					}

					if (digitalRead(BTN1) == LOW){
						unsigned long previousMillis = millis();
						tone(SPKRpin, 600, 20);
						delay(20);
						tone(SPKRpin, 500, 50);

						while (digitalRead(BTN1) == LOW){
							if (millis() - previousMillis >= 1000){
								tone(SPKRpin, 100, 50);
								delay(20);
								ConverterMode++;
								if (ConverterMode == 4)
									ConverterMode = 0;
								cursor = 1;
								break;
							}
						}
						delay(20);
						cursor--;
						if (cursor == -1)
							cursor = 5;

					}

					if (digitalRead(BTN2) == LOW){
						tone(SPKRpin, 300, 20);
						delay(20);
						tone(SPKRpin, 400, 50);
						while (digitalRead(BTN2) == LOW){}
						delay(20);

						DECbuffer[cursor] = DECbuffer[cursor]++;
						if (DECbuffer[cursor] == '9' + 1)
							DECbuffer[cursor] = '0';

						number = atoi(DECbuffer);
						//number = 0;
						//for (int i = 0; i < 7; i++){
						//	number += (DECbuffer[i]-'0') * pow(10, -1 * (i - 6));
						//}

					}

					break;

				case 3:
					ConverterEditing = false;
					break;
					


			}

			//update cursorState
			if (millis() - previousMillis1 >= 500){
				cursorState = !cursorState;
				previousMillis1 = millis();
			}

			//while ()
			//for (i = 15;i>cursor && bitRead(number,i)!=0; i--){
			//}
			//if (millis() - previousMillis1 >= 500){

			//}

			//display.clearDisplay();
			//display.setTextSize(1);
			//display.println("BINARY:");
			//display.println(number);

			//delay(20);
		}
	
			





			//display.clearDisplay();
			//myRTC.updateTime();

			//display.setTextWrap(false);

			//display.setTextSize(1);
			//display.print("H");
			//display.setTextSize(2);
			//display.println(myRTC.hours, BIN);                                                                                                         
			//display.setTextSize(1);
			//display.print("M");
			//display.setTextSize(2);
			//display.println(myRTC.minutes, BIN);
			//display.setTextSize(1);
			//display.print("S");
			//display.setTextSize(2);
			//display.println(myRTC.seconds, BIN);

			//display.setCursor(66, 0);
			//display.setTextSize(1);
			//display.print("BIN");

			//display.setTextWrap(true);

			//display.display();
		break;

		case 11:

			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();
				
				display.setTextWrap(false);
				
				display.setTextSize(1);
				display.print("H");
				display.setTextSize(2);
				display.println(myRTC.hours, BIN);                                                                                                         
				display.setTextSize(1);
				display.print("M");
				display.setTextSize(2);
				display.println(myRTC.minutes, BIN);
				display.setTextSize(1);
				display.print("S");
				display.setTextSize(2);
				display.println(myRTC.seconds, BIN);
				
				display.setCursor(66, 0);
				display.setTextSize(1);
				display.print("BIN");

				display.setTextWrap(true);

				display.display();
			}
			break;
		
		case 7:

			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextWrap(false);

				display.setTextSize(1);
				display.print("Hours:");
				display.setTextSize(2);
				display.println(myRTC.hours, HEX);                                                                                                        
				display.setTextSize(1);
				display.print("Minutes:");
				display.setTextSize(2);
				display.println(myRTC.minutes, HEX);
				display.setTextSize(1);
				display.print("Seconds:");
				display.setTextSize(2);
				display.println(myRTC.seconds, HEX);

				display.setCursor(66, 0);
				display.setTextSize(1);
				display.print("HEX");

				display.setTextWrap(true);

				display.display();
			}
			break;

		case 8:

			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextWrap(false);

				display.setTextSize(1);
				display.print("Hours:");
				display.setTextSize(2);
				display.println(myRTC.hours, OCT);                                                                                                         
				display.setTextSize(1);
				display.print("Minutes:");
				display.setTextSize(2);
				display.println(myRTC.minutes, OCT);
				display.setTextSize(1);
				display.print("Seconds:");
				display.setTextSize(2);
				display.println(myRTC.seconds, OCT);

				display.setCursor(66, 0);
				display.setTextSize(1);
				display.print("OCT");

				display.setTextWrap(true);

				display.display();
			}
			break;

		case 9:

			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextWrap(false);

				display.setTextSize(1);
				display.print("Hours:");
				display.setTextSize(2);
				display.println(myRTC.hours, DEC);                                                                                                         
				display.setTextSize(1);
				display.print("Minutes:");
				display.setTextSize(2);
				display.println(myRTC.minutes, DEC);
				display.setTextSize(1);
				display.print("Seconds:");
				display.setTextSize(2);
				display.println(myRTC.seconds, DEC);

				display.setCursor(66, 0);
				display.setTextSize(1);
				display.print("DEC");

				display.setTextWrap(true);

				display.display();
			}
			break;

		case 10:

			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextWrap(false);
				
				//display.setTextSize(1);
				//display.print("H:");
				display.setTextSize(2);
				display.println(DECtoROMAN(myRTC.hours));                                                                                                         
				//display.setTextSize(1);
				//display.print("M:");
				display.setTextSize(2);
				display.println(DECtoROMAN(myRTC.minutes));
				display.setTextSize(2);
				display.println(DECtoROMAN(myRTC.seconds));

				display.setCursor(66, 0);
				display.setTextSize(1);
				display.print("ROM");

				display.setTextWrap(true);

				display.display();
			}
			break;

		case 2:			
			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextSize(1);

				display.print(" ");
				display.print(myRTC.hours);                                                                                                        
				display.print(":");
				display.print(myRTC.minutes);
				display.print(":");
				display.println(myRTC.seconds);
				display.print(myRTC.dayofmonth);
				display.print("/");
				display.print(myRTC.month);
				display.print("/");
				display.println(myRTC.year);

				display.print("BINARY CLOCK:\nH:");
				display.println(myRTC.hours, BIN);                                                                                                         
				display.print("M:");
				display.println(myRTC.minutes, BIN);
				display.print("S:");
				display.println(myRTC.seconds, BIN);

				//display.drawRect(0, 0, myRTC.seconds, myRTC.seconds, BLACK);
				//display.print("\nthermistor reading= "); display.print(readThermistor()); display.print("*C");
				display.display();
			}
			break;

		case 3:
			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextSize(1);

				display.print(" ");
				display.print(myRTC.hours);                                                                                                         
				display.print(":");
				display.print(myRTC.minutes);
				display.print(":");
				display.println(myRTC.seconds);
				display.print(myRTC.dayofmonth);
				display.print("/");
				display.print(myRTC.month);
				display.print("/");
				display.print(myRTC.year);


				display.drawCircle(42, 24, 23, BLACK);
				x = 42 + 23 * sin(myRTC.seconds * 360 / 60 * DEG_TO_RAD); y = 24 - 23 * cos(myRTC.seconds * 360 / 60 * DEG_TO_RAD);
				display.drawLine(42, 24, x, y, BLACK);
				x = 42 + 23 * sin(myRTC.minutes * 360 / 60 * DEG_TO_RAD); y = 24 - 23 * cos(myRTC.minutes * 360 / 60 * DEG_TO_RAD);
				display.drawLine(42, 24, x, y, BLACK);
				hours = myRTC.hours;
				if (hours > 12)hours -= 12;
				x = 42 + 18 * sin(hours * 360 / 12 * DEG_TO_RAD); y = 24 - 18 * cos(hours * 360 / 60 * DEG_TO_RAD);
				display.drawLine(42, 24, x, y, BLACK);

				//x=42+23*(sin(heading)* 180/M_PI); y =24-23*(cos(heading)* 180/M_PI);
				display.display();
			}
			break;

		case 4:
			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();

				display.setTextSize(2);

				display.print(myRTC.hours);                                                                                                           
				display.print(":");
				display.print(myRTC.minutes);
				display.setTextSize(1);
				display.print(":");
				display.print(myRTC.seconds);
				display.setTextSize(2);
				display.print("\n");
				display.print(myRTC.dayofmonth);
				display.print("/");
				display.print(myRTC.month);
				display.setTextSize(1);
				display.print("/");
				display.println(myRTC.year);

				//display.drawRect(0, 0, myRTC.seconds, myRTC.seconds, BLACK);
				//display.print("\nthermistor reading= "); display.print(readThermistor()); display.print("*C");
				display.display();
			}
			break;

		case 5:
			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();

				display.clearDisplay();
				myRTC.updateTime();
				//display.drawRect
				display.setTextSize(1);
				display.println("SQUARE ROOT\nCLOCK:");

				display.setTextSize(1);
				display.setCursor(15, 20);
				display.drawLine(15 - 5, 20 + 2, 15 - 2, 20 + 8, BLACK);
				display.drawLine(15 - 2, 20 + 8, 15 - 2, 20 - 3, BLACK);
				display.drawLine(15 - 2, 20 - 3, 65, 20 - 3, BLACK);
				//display.setCursor(5, 35);
				display.print(myRTC.hours*myRTC.hours);                                                                                                           
				display.print(":");
				display.print(myRTC.minutes*myRTC.minutes);

				display.display();
			}
			break;

		case 6:
			if (millis() - previousMillis1 >= 100) {
				previousMillis1 = millis();
				display.clearDisplay();
				display.setTextSize(1);
				display.println("free RAM:");
				display.print(freeMemory());
				display.println(" bytes");
				display.display();
				delay(100);
			}
			break;
	}
}


void LEDfadeOUT(int numPin, int x){
	for (int i = 0; i <= 255; i++){
		analogWrite(numPin, i);
		delay(x);
	}
	digitalWrite(numPin, HIGH);
}



void LEDfadeIN(int numPin, int x){
	for (int i = 255; i > 0; i--){
		analogWrite(numPin, i);
		delay(x);
	}
	digitalWrite(numPin, LOW);
}

char* DECtoROMAN(int x){
	static char string[10];
	int cursor;

	for (cursor = 0; x > 0; cursor++){
		if (x / 50 > 0){
			string[cursor] = 'L';
			x -= 50;
		}
		else if (x / 40 > 0){
			string[cursor] = 'X';
			cursor++;
			string[cursor] = 'L';
			x -= 40;
		}
		else if (x / 10 > 0){
			string[cursor] = 'X';
			x -= 10;
		}
		else if (x / 9 > 0){
			string[cursor] = 'I';
			cursor++;
			string[cursor] = 'X';
			x -= 9;
		}
		else if (x / 5 > 0){
			string[cursor] = 'V';
			x -= 5;
		}
		else if (x / 4 > 0){
			string[cursor] = 'I';
			cursor++;
			string[cursor] = 'V';
			x -= 4;
		}
		else if (x > 0){
			string[cursor] = 'I';
			x -= 1;
		}
	}
	string[cursor] = '\0';
	//display.print(string); display.display();
	return string;
}

//void shiftLeft1(char *SRbuffer){
//	int nCursor = 0;
//	int j=0;
//	for (nCursor; SRbuffer[nCursor - 1] != ' ' && SRbuffer[nCursor - 1] != ',' && SRbuffer[nCursor - 1] != ';'; nCursor++);
//	for (nCursor; nCursor < strlen(SRbuffer); nCursor++){
//		SRbuffer[j++] = SRbuffer[nCursor];
//	}
//}

void shiftRight(char *SRbuffer){
	while (SRbuffer[strlen(SRbuffer) - 1] == ' ')
		for (int i = strlen(SRbuffer) - 1; i >= 0; i--){
			SRbuffer[i + 1] = SRbuffer[i];
			SRbuffer[0] = '0';
		}
}
