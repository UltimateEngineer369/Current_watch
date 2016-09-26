#include "U8glib.h"
#include "Wire.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); 	//try others maybe they are faster

int F5 = 1;			//to refresh or not to refresh

const int len = 12;		
int menuTop = 0;		//first one to load
int selected = 2;		//starring form 0; + menuTop
char *menu[len] = {"Pero", "Slavko", "Hello world", "Last", "No ur not", "pls work", "rope", "hello", "hahahah", "I", "dont", "know" "or neznam"};
int maxMenuLen;			//max amount of stuff that can be displayed at the same time

int b1, b2, b3, b4;		//var for valuse
int button1 = 2, button2 = 3, button3 = 7, button4 = 8;
int code = 2000;		//buttons code if 0 0 0 0 => 2000, 1 0 1 0 => 1010, 0 1 1 0 => 2110

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
	if(F5 == 1)			//to refresh or not to refresh
	{
		u8g.firstPage();
		do
		{
			drawMenu();	//menu 	
		}while(u8g.nextPage());
	}

	//read buttons and setting F5 + movement
	uiMovement();
}

void drawMenu()
{
	//u8g.setFont(u8g_font_courR12r);			//constant width font
	u8g.setFont(u8g_font_fur11r);			//just personally beatutifu font
	
	int fH = u8g.getFontAscent()-u8g.getFontDescent();//font height

	maxMenuLen = 64/fH;
	
	Serial.print(fH);
	Serial.print("	");
	Serial.println(maxMenuLen);

	for(int i = 0; i<maxMenuLen; i++)		//actually draw display
	{
		u8g.setDefaultForegroundColor();	// white

		if(i == selected)			//if curent is selected
		{
			u8g.drawBox(0, i*fH+1, 128, fH);//draw box w
			u8g.setDefaultBackgroundColor();//set color b
		}
		
		u8g.setPrintPos(1, i*fH + fH);		//print text

		u8g.print(menu[i+menuTop]);		
	}
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

void uiMovement()
{
	readButtons();

	if(code == 2010)				//down
	{
		if(selected == 3)			//end of screen but not list
		{
			if(menuTop == len-maxMenuLen)	//end of list
			{
				menuTop = 0;
				selected = 0;
			}				
			else				//not end of list
			{
				menuTop++;
			}
		}
		else 					//just down
		{
			selected++;
		}

		F5 = 1;					//refresh
	}
	else if(code == 2100)				//up
	{
		if(selected == 0)			//top one
		{
			if(menuTop == 0)		//is top of list
			{
				selected = len-maxMenuLen-1;		//last one
				menuTop = len-maxMenuLen;				
			}
			else
			{
				menuTop--;		//move display up
			}
		}
		else
		{
			selected--;
		}
		
		F5 = 1;					//REFRESh
	}
	else if(code == 2001)		//THIS IS FOR LIKE WHEN IT IS SELECTED PRESS OK
	{
		F5 = 0;	
	}
	else
	{
		F5 = 0;
	}
}
