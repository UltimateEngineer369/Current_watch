#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

uint8_t tick = 0; 

uint8_t playerX = 44; 
const uint8_t playerY = 54;

uint8_t bullet[5][3] = {};
uint8_t bullet_n = 0;

void setup() 
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);	

	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);

	display.display();
	delay(1000);

	display.clearDisplay();
}

void loop() 
{
	display.clearDisplay();

	if(tick == 2)
	{
		for(int i = 0; i<bullet_n; i++)
		{
			bullet[i][1] -= 8;	
		}

		tick = 0;
	}

	tick++;

	if(button(0) == 0)
	{
		if(playerX > 0)
		{
			playerX-=2;
		}
	}
	else if(button(1) == 0)
	{
		if(playerX < 108)
		{
			playerX+=2;
		}
	}
	else if(button(2) == 0)
	{
		bullet_n++;

		bullet[bullet_n-1][0] = playerX + 4;
		bullet[bullet_n-1][1] = playerY;
	}

	drawPlayer(playerX, playerY);
	drawBullet();

	display.display();

	delay(50);	
}

void drawPlayer(uint8_t x, uint8_t y)
{
	display.fillRect(x, y + 5, 20, 5, WHITE);
	display.fillRect(x + 8, y, 4, 5, WHITE);
}

bool button(uint8_t but)
{
	if(but == 0)
	{
		return digitalRead(3);
	}

	if(but == 1)
	{
		return digitalRead(2);
	}
	if(but == 2)
	{
		return digitalRead(4);
	}
}

void drawBullet()
{
	display.drawPixel(bullet[0][0], bullet[0][1], WHITE);
}





























