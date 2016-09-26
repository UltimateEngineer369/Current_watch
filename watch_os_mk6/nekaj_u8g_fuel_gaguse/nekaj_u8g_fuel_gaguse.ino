#include "MAX17043.h"
#include "Wire.h"
#include "U8glib.h"

MAX17043 lipo;
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);        // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(4, 5, 6, 7);        // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new white HalTec OLED)
//U8GLIB_SSD1306_128X64 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);      // I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);     // Fast I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);        // Display which does not send AC
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(13, 11, 10, 9);    // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(10, 9);            // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

int led = 13;

int b1Pin = 2, b2Pin = 3, b3Pin = 7, b4Pin = 8;
int b1, b2, b3, b4;
int code = 2000;

float cellVoltage;
float stateOfCharge;

int timeB, timeN;	 //later add just delay and remove this 2 but for now thy stay
int timeDelay = 500;

int n, m;
int mode;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lipo.reset();
  lipo.quickStart();

  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);
  pinMode(b3Pin, INPUT_PULLUP);
  pinMode(b4Pin, INPUT_PULLUP);

  Serial.println("Hello this is start");

  // flip screen, if required
  // u8g.setRot180();
  // u8g.setRot90();

  u8g.setColorIndex(1);         //color index for b/w display
}

void loop()
{
  //lipo.reset();
  //lipo.quickStart();

  readButtons();

  //cellVoltage = lipo.getVCell();
  //stateOfCharge = lipo.getSoC();

  //Serial.print(stateOfCharge);
  //Serial.print("  ");
  //Serial.println(cellVoltage);

	u8g.firstPage();
	do
	{
		if(mode == 0)
		{
			draw();	
		}
		else if(mode == 1)
		{

			draw_time();
		}
		else if(mode == 2)
		{
			showButtons();
		}
	}
	while(u8g.nextPage());

	timer(500);

	if(mode != 2)
	{
		mode = not(mode);
	}
}

void showButtons()
{
  u8g.setFont(u8g_font_fur14);
  u8g.setPrintPos(0, 36);

	u8g.print(code);

	readButtons();

	if(code == 2011)
	{
		mode = 1;
		waitFR();
	} 
}

void draw()
{
  // graphic commands to redraw the complete screen should be placed here
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_fur14);
  u8g.setFont(u8g_font_courR12r);
  u8g.drawStr( 0, 22, "Hello Wor!d!");

  u8g.drawStr(0, 40, "Pero Slavko");

  u8g.setPrintPos(0, 63);
  u8g.print("hahahahhah");
}

void draw_time()
{
  u8g.setFont(u8g_font_courB24);
  n = u8g.getStrWidth("22:22");
  m = n;
  //Serial.print(n);
  n = (128 - n) / 2;
  //Serial.print("  ");
  //Serial.println(n);
  u8g.drawBox(0, 45, n, n);
  u8g.setPrintPos(n, 45);
  u8g.drawBox(m + n, 45, n, n);
  u8g.print("22:22");
}

void readButtons()
{
  b1 = digitalRead(b1Pin);
  b2 = digitalRead(b2Pin);
  b3 = digitalRead(b3Pin);
  b4 = digitalRead(b4Pin);

  Serial.print(b1);
  Serial.print("  ");
  Serial.print(b2);
  Serial.print("  ");
  Serial.print(b3);
  Serial.print("  ");
  Serial.println(b4);

  if (b1 == 0)
  {
    if (b2 == 0)
    {
      if (b3 == 0)
      {
        if (b4 == 0)
        {
          code = 1111;
        }
        else
        {
          code = 1110;
        }
      }
      else
      {
        if (b4 == 0)
        {
          code = 1101;
        }
        else
        {
          code = 1100;
        }
      }
    }
    else
    {
      if (b3 == 0)
      {
        if (b4 == 0)
	{ 
		code = 1011; 
	} 
	else 
	{ 
		code = 1010; 
	}
      }
      else 
      { 
      	if (b4 == 0) 
	{ 
		code = 1001; 
	}	
	else 
	{
		code = 1000;
        }
      }
    }
  }
  else
  {
    if (b2 == 0)
    {
      if (b3 == 0)
      {
        if (b4 == 0)
        {
          code = 2111;
        }
        else
        {
          code = 2110;
        }
      }
      else
      {
        if (b4 == 0)
        {
          code = 2101;
        }
        else
        {
          code = 2100;
        }
      }
    }
    else
    {
      if (b3 == 0)
      {
        if (b4 == 0)
        {
          code = 2011;
        }
        else
        {
          code = 2010;
        }
      }
      else
      {
        if (b4 == 0)
        {
          code = 2001;
        }
        else
        {
          code = 2000;
        }
      }
    }
  }

  Serial.println(code);
}

void timer(int timeU)
{
  timeB = millis();
  timeN = timeB;

  while (timeN <= timeB + timeU)
  {
    timeN = millis();
    readButtons();
    if (code == 2001)
    {
      	mode = 2;
	waitFR();
	break;
    }
    if(mode == 2)
    {
    	break;
    }
  }
}

void waitFR()
{
	while(code != 2000)
	{
		readButtons();
	}
}
