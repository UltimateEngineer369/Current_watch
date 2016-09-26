#include "U8glib.h"
#include "Wire.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);        //try others maybe they are faster

int F5 = 1;                     //to refresh or not to refresh
int b1, b2, b3, b4;             //var for valuse
int button1 = 2, button2 = 3, button3 = 7, button4 = 8;
int code = 2000;                //buttons code if 0 0 0 0 => 2000, 1 0 1 0 => 1010, 0 1 1 0 => 2110

int h, m, s;
int timeN, timeB;

void setup()
{
	Wire.begin();
	Serial.begin(9600);

	pinMode(button1, INPUT_PULLUP);
	pinMode(button2, INPUT_PULLUP);
	pinMode(button3, INPUT_PULLUP);
	pinMode(button4, INPUT_PULLUP);

	Serial.println("Hello world Starrt :)");
	
	u8g.setColorIndex(1);		//mode for b/w displays
}

void loop()
{
	uiUpdate();			//set refresh, time, and if buttons has been pressed;

	Serial.print("f5 is ");
	Serial.print(F5);

	Serial.print("	timeN ");
	Serial.print(timeN);
	Serial.print("	timeB ");
	Serial.println(timeB);

	if(F5 == 1)			//to refresh or not to refresh
	{
		u8g.firstPage();
		do
		{
			drawHome();	//home 	
		}while(u8g.nextPage());
	}

	if(m == 59)
	{
		if(h == 23)
		{
			m = 0;
			h = 0;
		}
		else
		{
			m = 0;
			h++;
		}
	}
	else
	{
		m++;
	}

	delay(1000);	
}

void drawHome()
{
	u8g.setPrintPos(0, 11);
	u8g.setFont(u8g_font_6x10);
	u8g.print("jebi se");

	//battery indicator
	u8g.drawFrame(100, 0, 20, 8);
	u8g.drawBox(120, 2, 2, 4);

	//wether widget

	u8g.setFont(u8g_font_fur25);
	int n = (128-u8g.getStrWidth("22:22"))/2;
	u8g.setPrintPos(n, 44);
	
	if(h < 10)
		u8g.print(0);
	u8g.print(h);
	u8g.print(":");
	if(m < 10)
		u8g.print(0);
	u8g.print(m);

	u8g.setFont(u8g_font_fur11);
	u8g.setPrintPos(0, 64);
	u8g.print("Menu");

	n = 128-u8g.getStrWidth("some shit");
	u8g.setPrintPos(n, 64);
	u8g.print("some shit");
}

void uiUpdate()
{
	readButtons();
	timeN = m+h;

	if(timeN != timeB)
	{
		F5 = 1;

	}
	else if(code != 2000)
	{
		F5 = 1;	
	}
	else
	{
		F5 = 0;
	}

	timeB = timeN;
}

void readButtons()
{
	b1 = digitalRead(button1);
	b2 = digitalRead(button2);
	b3 = digitalRead(button3);
	b4 = digitalRead(button4);
	
	if(b1 == 0)
	{
		if(b2 == 0)	
		{
			if(b3 == 0)
			{
				if(b4 == 0)
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
				if(b4 == 0)
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
			if(b3 == 0)
			{
				if(b4 == 0)
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
				if(b4 == 0)
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
		if(b2 == 0)
		{
			if(b3 == 0)
			{
				if(b4 == 0)
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
				if(b4 == 0)
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
			if(b3 == 0)
			{
				if(b4 == 0)
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
				if(b4 == 0)
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
	
}
