#ifndef cio_h
#define cio_h

struct Player_info {
	int pcol; // LEFT_CORNER;
	int prow; // LEFT_CORNER;
	int dirCol;//  DIR_RIGHT;
	int dirRow; // DIR_DOWN;
	int nStage; //��������
	int gold;
	int mp;
	int mpitem;
	char myWeapon[100];
};

struct Enemy_info {
	int ecol;
	int erow; 
	int dirCol;// = DIR_RIGHT;
	int dirRow;// = DIR_DOWN;
	int tick;// = 0;
	bool Boss;
};

int moveP();
void moveE();

void rePrint();
int get_key();
void turn(int key); // ��������� �ٲ��ִ� �Լ�
void playerSkill(int);

#endif