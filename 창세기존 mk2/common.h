#ifndef common_h
#define common_h

#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define FIRST_ENEMY_PLACE_COL 20
#define FIRST_ENEMY_PLACE_ROW 16
#define FILED_HEIGHT 18
#define FIELD_WIDTH 22
#define LEFT_CORNER 1
#define RIGHT_CORNER (FIELD_WIDTH - 2) // 벽 앞 (오른쪽), 배열의 마지막은 [21]
#define MAP_WALL '#'
#define MAP_PLAYER '@'
#define MAP_SPACE ' '
#define K_QUIT 'q'
#define MAP_ENEMY '$'
#define MAP_BOSS 'B'
#define TOP_CORNER 0
#define zskill 'z'
#define xskill 'x'
#define skillEfect '*'
#define bossName 'Shturman'
#define bossSymbol '!'

#define SHOP_KEY 's'
#define drinkMP 'j'

#define DIR_LEFT -1
#define DIR_RIGHT 1
#define DIR_DOWN 1
#define DIR_UP -1

#define ENTER 13
#define K_ARROW 224
#define K_LEFT 75
#define K_RIGHT 77
#define K_UP 72
#define K_DOWN 80
#define K_NONE 0 // NO Key input or invalid key input
#define enemyMoveCount 20

void gotoxy(int, int);
//int get_key();
void intro();
void outro();


void initialize();
void display(void);
int zSkill();
void zSkillNormalize();
void makeEnemy();
int xSkill();
void xSkillNormalize();


extern int enemyAmount;
extern int score;
extern int enemyKill;
extern char bBuf[FILED_HEIGHT][FIELD_WIDTH]; //맵 뒷버퍼
extern char fBuf[FILED_HEIGHT][FIELD_WIDTH]; // 앞버퍼

#endif