#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#define SNAKE_MAX_LENGTH 20
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define SNAKE_IMAGE 'O'

typedef struct Snake_{
	int x;
	int y;
} SnakeBody;

SnakeBody snake[SNAKE_MAX_LENGTH];
int row, col;
int direction;
int snakeLength = 1;


void initGame(void)
{
	for(int i = 0; i < SNAKE_MAX_LENGTH; i++) {
		snake[i].x = -1;
		snake[i].y = -1;
	}

	initscr();			//curses & terminal setting init and save
	noecho();	
	cbreak();			//interupt & 흐름 제어 키를 제외한 모든 키들의 작동을 무시
	noecho();
	curs_set(0);		//커서 안보이게
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);	//???
	attron(A_BOLD);		//==standout
	getmaxyx(stdscr, row, col);	//curses.h 매크로, row&col값을 저장
	clear();
	refresh();

	
}


void showWinner()
{
	clear();
	mvprintw(row/2, col/2-strlen("HELLO")/2, "HELLO");
	refresh();
}


void initSnake()
{
	snake[1].x = 2;
	snake[1].y = row/2;
	snake[0].x = 3;
	snake[0].y = row/2;
	snakeLength = 2;
	direction = RIGHT;
}


void drawSnake()
{
	int i = 0;
	while(snake[i].x != -1 && snake[i].y != -1) {
		move(snake[i].y, snake[i].x);	//cursur 이동
		addch(SNAKE_IMAGE);				//그리기
		i++;
	}
}


int main()
{
	initGame();
	showWinner();		//HELLO  화면에 출력
	getch();
	clear();
	initSnake();
	drawSnake();

	refresh();

	sleep(5);
	endwin();
}
