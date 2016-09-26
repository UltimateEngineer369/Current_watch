#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//normal start

uint8_t matrix[4][4] = {{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}};

/*
uint8_t matrix[4][4] = {{1, 0, 1, 1},
                        {1, 1, 0, 1},
                        {1, 0, 0, 1},
                        {1, 1, 1, 0}};


uint8_t matrix[4][4] = {{2, 0, 2, 0},
                        {2, 3, 2, 0},
                        {3, 2, 2, 1},
                        {5, 4, 1, 0}};


uint8_t matrix[4][4] = {{13, 13, 13, 13},
			{13, 13, 13, 13},
			{13, 13, 13, 13},
			{13, 13, 13, 13}};


uint8_t matrix[4][4] = {{7, 8, 9, 7},
			{8, 9, 7, 7},
			{9, 7, 7, 9},
			{7, 7, 8, 8}};


uint8_t matrix[4][4] = {{10, 2, 10, 2},
			{2, 10, 2, 10},
			{10, 2, 10, 2},
			{2, 10, 2, 10}};
*/


uint8_t empty[15][2] = {};
uint8_t empty_n = 0;
uint8_t randomN = 0;

bool normalStart = true;
bool isCombineOrNot = false;
bool invertColors = false;

//bool W, B;

uint8_t buttonPin[4] = {5, 4, 3, 2};

uint32_t score = 0;
uint8_t maxScore = 0;

void setup()
{
        Serial.begin(9600);
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

        pinMode(13, OUTPUT);

        for(int i = 0; i<4; i++)
        {
                pinMode(buttonPin[i], INPUT_PULLUP);
        }

	if(invertColors == 1)
	{
		display.setTextColor(BLACK);
		W = 0;
		B = 1;
	}
	else
        {
		display.setTextColor(WHITE);
		W = 1;
		B = 0;
        }

	display.clearDisplay();
	display.setTextSize(3);
	display.setCursor(28, 10);

        display.println(F("2048"));
        display.display();

        delay(1000);

        display.clearDisplay();

	display.setTextSize(1);

	if(normalStart == 1)
	{
		makeNormalStart();
	}
}

void loop()
{
	if(gameOver() == true)
	{
		display.clearDisplay();
		getMaxNum();

		while(1)
		{
			gameOverScreen();

			display.display();
			delay(50);
		}
	}

	if(button(1) == 0 && (canMoveDown() || canCombineDown()))
        {
                Serial.print(canMoveDown());
		Serial.print(" ");
		Serial.println(canCombineDown());

		down();

		randomFill();

                while(button(1) != 1)
                {};
        }
        else if(button(3) == 0 && (canMoveUp() || canCombineUp()))
        {
		Serial.print(canMoveUp());
		Serial.print(" ");
		Serial.println(canCombineUp());

                up();

		randomFill();

		while(button(3) != 1)
                {};
        }
	else if(button(2) == 0 && (canMoveLeft() || canCombineLeft()))
	{
		Serial.print(canMoveLeft());
		Serial.print(" ");
		Serial.println(canCombineLeft());

		left();

		randomFill();

		while(button(2) != 1)
		{};
	}
	else if(button(0) == 0 && (canMoveRight() || canCombineRight()))
	{
		Serial.print(canMoveRight());
		Serial.print(" ");
		Serial.println(canCombineRight());

		right();

		randomFill();

		while(button(0) != 1)
		{};
	}

        drawMatrix();

	//printButtons();

        display.display();

        delay(50);
}

void gameOverScreen()
{
	display.fillScreen(B);
	display.setTextColor(W);
	display.setCursor(11, 1);
	display.setTextSize(2);

	display.print(F("GAME OVER"));

	display.setCursor(1, 25);

	display.print(score);

	display.setCursor(1, 45);

	display.print(pow(2, maxScore), 0);

	if(maxScore < 10)
	{
		display.setCursor(84, 45);

		display.print(F("2^"));
		display.print(maxScore);
	}
	else
	{
		display.setCursor(73, 45);

		display.print(F("2^"));
		display.print(maxScore);
	}
}

void makeNormalStart()
{
	uint8_t n = random(2, 4);

	uint8_t x, y;

	for(int i = 0; i<n; i++)
	{
		x = random(0, 3);
		y = random(0, 3);

		matrix[y][x] = random(1, 2);
	}
}

void drawMatrix()
{
        display.clearDisplay();

	if(invertColors == 1)
	{
		display.fillRect(0, 0, 128, 64, WHITE);
	}

        for(int i = 0; i<4; i++)
        {
                for(int j = 0; j<4; j++)
                {
			display.drawLine(i*32-1, 0, i*32-1, 64, W);
                        display.drawLine(0, i*16, 128, i*16, W);
			if(matrix[j][i] != 0)
                        {
                        	if(matrix[j][i] < 4)
				{
					//2, 4, 8
					display.setCursor(i*32+13, j*16+5);
				}
				else if(matrix[j][i] < 7)
				{
					//16, 32, 64
					display.setCursor(i*32+10, j*16+5);
				}
				else if(matrix[j][i] < 10)
				{
					//128, 256, 512
					display.setCursor(i*32+7, j*16+5);
				}
				else
				{
					display.setCursor(i*32+4, j*16+5);
                                }

				display.print(pow(2, matrix[j][i]), 0);
			}
                }
        }

	display.drawLine(0, 0, 0, 64, W);
	display.drawLine(127, 0, 127, 63, W);
	display.drawLine(0, 63, 127, 63, W);
}

uint8_t button(uint8_t n)
{
        return digitalRead(buttonPin[n]);
}

void printButtons()
{
	display.setTextSize(2);
	display.clearDisplay();
	display.setCursor(0, 0);

	display.print(button(3));
	display.print(F(" "));
	display.print(button(1));
	display.print(F(" "));
	display.print(button(0));
	display.print(F(" "));
	display.print(button(2));
}

bool anyBelow(uint8_t c, uint8_t fromWhat)
{
        uint8_t zbroj = 0;

        for(int i = fromWhat; i<4; i++)
        {
                zbroj += matrix[i][c];
        }

        if(zbroj > 0)
        {
                return true;
        }
        else
        {
                return false;
        }
}

void moveUp(uint8_t c)
{
        for(int i = 0; i<3; i++)
        {
                if(matrix[i][c] == 0 && anyBelow(c, i) == true)
                {
                        for(int j = i; j<4; j++)
                        {
                                if(matrix[j][c] > 0)
                                {
                                        matrix[i][c] = matrix[j][c];
                                        matrix[j][c] = 0;
					break;
                                }
                        }
                }
        }
}

void combineUp(uint8_t c)
{
        for(int i = 0; i<3; i++)
        {
                if(matrix[i][c] == matrix[i+1][c] && matrix[i][c] != 0)
                {
                        matrix[i][c]++;
			score += pow(2, matrix[i][c]);
                        matrix[i+1][c] = 0;
                }
        }
}

void up()
{
        for(int i = 0; i<4; i++)
        {
                if(anyBelow(i, 0) == true)
                {
                        moveUp(i);
                }

                combineUp(i);

                moveUp(i);
        }
}

bool anyAbove(uint8_t c, uint8_t fromWhat)
{
        uint8_t zbroj = 0;

        for(int i = fromWhat; i>=0; i--)
        {
                zbroj += matrix[i][c];
        }

        if(zbroj > 0)
        {
                return true;
        }
        else
        {
                return false;
        }
}

void moveDown(uint8_t c)
{
        for(int i = 3; i>0; i--)
        {
                if(matrix[i][c] == 0 && anyAbove(c, i) == true)
                {
                        for(int j = i; j>=0; j--)
                        {
                                if(matrix[j][c] > 0)
                                {
                                        matrix[i][c] = matrix[j][c];
                                        matrix[j][c] = 0;

                                        break;
                                }
                        }
                }
        }
}

void combineDown(uint8_t c)
{
        for(int i = 3; i>0; i--)
        { if(matrix[i][c] == matrix[i-1][c] && matrix[i][c] != 0) {
                        matrix[i][c]++;
			score += pow(2, matrix[i][c]);
                        matrix[i-1][c] = 0;
                }
        }
}

void down()
{
        for(int i = 0; i<4; i++)
        {
                if(anyAbove(i, 3) == true)
                {
                        moveDown(i);
                }

                combineDown(i);

                moveDown(i);
        }
}

void fillEmptyList()
{
        randomN = 0;

	for(int i = 0; i<15; i++)
	{
		empty[i][0] = -1;
		empty[i][1] = -1;
	}

        for(int i = 0; i<4; i++)
        {
                for(int j = 0; j<4; j++)
                {
                        if(matrix[i][j] == 0)
                        {
                                empty[randomN][0] = i;
                                empty[randomN][1] = j;
                                randomN++;
                        }
                }
        }

        empty_n = randomN;
}

void randomFill()
{
        fillEmptyList();

        randomN = random(0, empty_n);
        matrix[empty[randomN][0]][empty[randomN][1]] = random(1, 3);
}

bool anyLeft(uint8_t r, uint8_t fromWhat)
{
	uint8_t zbroj = 0;

	for(int i = fromWhat; i>=0; i--)
	{
		zbroj += matrix[r][i];
	}

	if(zbroj > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void combineRight(uint8_t r)
{
	for(int i = 3; i>0; i--)
	{
		if(matrix[r][i] == matrix[r][i-1] && matrix[r][i] != 0)
		{
			matrix[r][i]++;
			score += pow(2, matrix[r][i]);
			matrix[r][i-1] = 0;
		}
	}
}

void moveRight(uint8_t r)
{
	for(int i = 3; i>0; i--)
	{
		if(matrix[r][i] == 0 & anyLeft(r, i) == true)
		{
			for(int j = i; j>=0; j--)
			{
				if(matrix[r][j] > 0)
				{
					matrix[r][i] = matrix[r][j];
					matrix[r][j] = 0;
					break;
				}
			}
		}
	}
}

void right()
{
	for(int i = 0; i<4; i++)
	{
		if(anyLeft(i, 3) == true)
                {
                        moveRight(i);
                }

                combineRight(i);

                moveRight(i);
	}
}

bool anyRight(uint8_t r, uint8_t fromWhat)
{
	uint8_t zbroj = 0;

	for(int i = fromWhat; i<4; i++)
	{
		zbroj += matrix[r][i];
	}

	if(zbroj > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void combineLeft(uint8_t r)
{
	for(int i = 0; i<3; i++)
	{
		if(matrix[r][i] == matrix[r][i+1] && matrix[r][i] != 0)
		{
			matrix[r][i]++;
			score += pow(2, matrix[r][i]);
			matrix[r][i+1] = 0;
		}
	}
}

void moveLeft(uint8_t r)
{
	for(int i = 0; i<4; i++)
	{
		if(matrix[r][i] == 0 && anyRight(r, i) == true)
		{
			for(int j = i; j<4; j++)
			{
				if(matrix[r][j] > 0)
				{
					matrix[r][i] = matrix[r][j];
					matrix[r][j] = 0;
					break;
				}
			}
		}
	}
}

void left()
{
	for(int i = 0; i<4; i++)
	{
		if(anyRight(i, 0) == true)
                {
                        moveLeft(i);
                }

                combineLeft(i);

                moveLeft(i);
	}
}

bool isAnyEmpty()
{
	bool any = 0;

	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			if(matrix[i][j] == 0)
			{
				return true;
			}
		}
	}

	return false;
}


bool canCombineUp()
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<3; j++)
		{
			if(matrix[j][i] == matrix[j+1][i] && matrix[j][i] != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool canCombineDown()
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 3; j>0; j--)
		{
			if(matrix[j][i] == matrix[j-1][i] && matrix[j][i] != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool canCombineLeft()
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<3; j++)
		{
			if(matrix[i][j] == matrix[i][j+1] && matrix[i][j] != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool canCombineRight()
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 3; j>0; j--)
		{
			if(matrix[i][j] == matrix[i][j-1] && matrix[i][j] != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool canMoveDown()
{
	for(int j = 0; j<4; j++)
	{
		for(int i = 3; i>=0; i--)
		{
			if(matrix[i][j] == 0 && anyAbove(j, i))
			{
				return true;
			}
		}
	}

	return false;
}

bool canMoveUp()
{
	for(int j = 0; j<4; j++)
	{
		for(int i = 0; i<4; i++)
		{
			if(matrix[i][j] == 0 && anyBelow(j, i))
			{
				return true;
			}
		}
	}

	return false;
}

bool canMoveRight()
{
	for(int j = 0; j<4; j++)
	{
		for(int i = 3; i>0; i--)
		{
			if(matrix[j][i] == 0 && anyLeft(j, i))
			{
				return true;
			}
		}
	}

	return false;
}

bool canMoveLeft()
{
	for(int j = 0; j<4; j++)
	{
		for(int i = 0; i<3; i++)
		{
			if(matrix[j][i] == 0 && anyRight(j, i))
			{
				return true;
			}
		}
	}

	return false;
}

void blinkOnce()
{
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(100);
}

bool gameOver()
{
	uint8_t gameOverMain;
	gameOverMain += canCombineRight();
	gameOverMain += canCombineUp();
	gameOverMain += canCombineDown();
	gameOverMain += canCombineLeft();
	gameOverMain += canMoveDown();
	gameOverMain += canMoveUp();
	gameOverMain += canMoveLeft();
	gameOverMain += canMoveRight();

	if(gameOverMain == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void getMaxNum()
{
	maxScore = matrix[0][0];

	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			if(matrix[i][j] > maxScore)
			{
				maxScore = matrix[i][j];
			}
		}
	}
}
