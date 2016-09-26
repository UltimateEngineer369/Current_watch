#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

uint8_t tick = 0;

uint8_t snake[25][2];

uint8_t snakeLen = 1;
uint8_t snakeDir = 0;

uint8_t dir_U = 0, dir_D = 2, dir_R = 1, dir_L = 3;

uint8_t x = 0, X = 128;
uint8_t y = 14, Y = 64;

uint16_t score;

uint8_t L = 3, D = 4, U = 2, R = 5;  

void setup() 
{
	pinMode(L, INPUT_PULLUP);
	pinMode(D, INPUT_PULLUP);
	pinMode(U, INPUT_PULLUP);
	pinMode(R, INPUT_PULLUP);
	
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);	

	display.setTextColor(WHITE);
	display.setTextSize(1);

	snake[0][0] = 64;
	snake[0][1] = 32;
}

void loop()
{
	changeDir();

	if(button(1) == 0)
	{
		ate();
	}

	if(tick >= 2)
	{
		tick = 0;

		moveFirst();
		drawScreen();
	}

	tick++;

	display.display();
	delay(25);
}

void drawSnake()
{
	for(int i = 0; i<snakeLen; i++)
	{
		display.drawPixel(snake[i][0], snake[i][1], WHITE);
	}
}

void drawScreen()
{
	display.clearDisplay();

	display.setCursor(1, 1);
	display.print("Score: ");
	display.print(score);

	display.drawRect(x, y, X, Y-y, WHITE);

	drawSnake();
}

void changeDir()
{
	if(snakeDir == dir_U)
	{
		if(button(0) == 0)
		{
			snakeDir = dir_L;
		}
		else if(button(3) == 0)
		{
			snakeDir = dir_R;
		}
	}
	else if(snakeDir == dir_R)
	{
		if(button(0) == 0)
		{
			snakeDir = dir_U;
		}
		else if(button(3) == 0)
		{
			snakeDir = dir_D;
		}
	}
	else if(snakeDir == dir_D)
	{
		if(button(0) == 0)
		{
			snakeDir = dir_R;
		}
		else if(button(3) == 0)
		{
			snakeDir = dir_L;
		}
	}
	else if(snakeDir == dir_L)
	{
		if(button(0) == 0)
		{
			snakeDir = dir_D;
		}
		else if(button(3) == 0)
		{
			snakeDir = dir_U;
		}
	}
}

bool button(uint8_t b)
{
	if(b == 0)
	{
		return digitalRead(L);
	}
	else if(b == 1)
	{
		return digitalRead(D);	
	}
	else if(b == 2)
	{
		return digitalRead(U);
	}
	else 
	{
		return digitalRead(R);
	}
}

void moveFirst()
{
	if(snakeDir == dir_U)
	{
		snake[0][1]--;
	}
	else if(snakeDir == dir_D)
	{
		snake[0][1]++;
	}
	else if(snakeDir == dir_R)
	{
		snake[0][0]++;	
	}
	else if(snakeDir == dir_L)
	{
		snake[0][0]--;
	}
}
