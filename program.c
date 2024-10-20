#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#ifdef _WIN32
#include "conio.h"
#include "windows.h"

void moveCursor()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0,0};
	SetConsoleCursorPosition(hOut, coord);
}

#define CLEAR system("cls")
void checkForKey(char *key)
{
	if (_kbhit())
	{
		*key = _getch();
	}
}

void slowDown(int time)
{
	Sleep(time);
}
#else
#include "termios.h"
#include "unistd.h"
#define CLEAR system("clear")
void disableBuffer()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&t);
}

void restoreBuffer(struct termios *original)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void checkForKey(char *key)
{
	struct termios original;
	tcgetattr(STDIN_FILENO, &original);
	
	disableBuffer();
	
	*key = getchar();

	restoreBuffer(&original);
}

void slowDown(int time)
{
	time = time * 1000; // Convert microsecond to milliseconds
	usleep(time);
}
#endif

void mm(); // declaration of main menu function

int playGame(int bsize);

void drawBoard(int bsize, int plyrX, int plyrY, int targX, int targY);

int main(){
	srand(time(NULL));
	mm();

	return 0;
}

void mm(){
	char c = 'n'; 

	while(1)
	{
		printf("Welcome to the Chasing game!, would you like to play?\n[y/n]?\n");
		scanf(" %c", &c);

		if(!(c=='y'))
		{
			printf("Thanks for playing!");
			return;
		}
		
		printf("Great!\nYou will control the character \"X\", trying to reach the character \"O\"!\n");
		int bsize;
		printf("It's time to enter the dimensions of the board!\nEnter a number greater than 1\nbut less than 51: ");
		scanf(" %d", &bsize);

		if (bsize < 2)
		{
			bsize = 2;
		}
		else if (bsize > 50)
		{
			bsize = 50;
		}
		printf("Creating %d sized board...\n\n",bsize);
		
		playGame(bsize);
	}
}

int playGame(int bsize){
	int plyrX = rand() % bsize;
	int plyrY = rand() % bsize;
	
	int targX;
	int targY;

	int steps = 0;	
	time_t startTime;
	time_t endTime;

	startTime = time(NULL);
	CLEAR;
	do
	{
		targX = rand() % bsize;
		targY = rand() % bsize;
	}
	while(targX == plyrX);
	
	while(!(plyrX == targX && plyrY == targY))
	{
		slowDown(10);
		
		#ifdef _WIN32
		moveCursor();
		#else
		CLEAR;
		#endif
		drawBoard(bsize, plyrX, plyrY, targX, targY);
		
		char choice = ' ';
		//scanf(" %c", &choice);
		checkForKey(&choice);

		if(choice == 'a')
		{
			plyrX = (plyrX == 0) ? 0 : plyrX - 1;
		}
		else if(choice == 'w')
		{
			plyrY = (plyrY == 0) ? 0 : plyrY - 1;
		}
		else if(choice == 'd')
		{
			plyrX = (plyrX == (bsize - 1)) ? (bsize - 1) : plyrX + 1;
		}
		else if(choice == 's')
		{
			plyrY = (plyrY == (bsize - 1)) ? (bsize - 1) : plyrY + 1;
		}
		else // In this scenario the user has not inputed a correct character
		{
			steps--;
		}
		steps++;
	}
	CLEAR;
	endTime = time(NULL);
	printf("Congratulations! You won!\nIt took you %d steps!\nCompleted in %d seconds\n", steps, (endTime-startTime));
}

void drawBoard(int bsize, int plyrX, int plyrY, int targX, int targY){
	
	char drawPiece = '-';
	for(int i = 0; i < bsize; i++)
	{
		for(int x = 0; x < bsize; x++)
		{
			drawPiece = '-';

			if(x == plyrX)
			{
				if(i == plyrY)
				{
					drawPiece = 'X';
				}		
			}

			if(x == targX)
			{
				if(i == targY)
				{
					drawPiece = 'O';
				}
			}

			printf("%c", drawPiece);
		}
		printf("\n");
	}
	printf("You are the X, try to reach O!\n");
	printf("Use keys, \"wasd\" to move!\n");
}
