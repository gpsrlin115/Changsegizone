/*
* �������� ���� ��� ��ƾ� �������� ��ȭ �ؾ���
* ���� �߰� �ʿ�
* 
* 1-1) ���� I (O)
* 1-2) ���� II (O)
* 1-3) intro(), outro() (O)
* 1-4) 2���� �迭 (O)
* 1-5) ���� ���۸� (O)
* 1-6) ����ȭ (Player) (O)
* 1-7) ����ȭ (Enemy) (O)
* 2-1) ���� ������ (O)
* 2-2) �� ����� (O)
* 2-3,4) ������Ʈ ����� (O)
* 2-5) �浹 �̺�Ʈ (O)
* 2-6) ��ų ����Ʈ (O)
* 2-7) ���� ǥ�� (O)
* 2-8) �������� (O)
* 2-9) �߰���� ����� : ���� ���� ��ȣ�ۿ�, ����, ����ȸ��������, ���� ������ ��ȭ (��ų�� ����ؼ� ��ƾ� ���ӳ�)
*/


#include "common.h"
#include "cio.h"

struct Player_info player;
struct Enemy_info enemy[4];

int score = 0;
int enemyKill = 0;
int enemyAmount = 1;

int main(void) {
    intro();
    initialize();

    for (int i = 0; i < player.nStage+1; i++) {
        enemy[i].tick = 0;
    }
    
    
    
    while (true) {
        
        display();
        Sleep(10);
        int key = get_key();
        if (key == K_QUIT) { //q������ ��
            break;
        }
        
        playerSkill(key);

        if (key == K_LEFT || key == K_RIGHT || key == K_UP || key == K_DOWN) {
            turn(key);
        }
        
        
        enemyKill = moveP();
        moveE();
		
        for (int i = 0; i < player.nStage+1; i++) {
            enemy[i].tick++;
        }

        if (player.nStage < 3 && enemyKill == 3) { // ���� ����������
            player.nStage++;
            enemyAmount++;
            enemyKill = 0;
        }
        if (enemy[3].Boss == false) { // Ŭ���� ����
            break;
        }
		
    }
    
    outro();
    return 0;
}

// (��, ��)�� �Է� �޾Ƽ� (��, ��) ��ġ�� Ŀ���� �Ű��ִ� �Լ�
void gotoxy(int row, int column)
{
    COORD pos = { column, row };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void intro()
{
    printf("Welcome!");

    if (_getch() == ENTER)
        system("cls");
}

void outro()
{
    system("cls");
    char dino[256];
    FILE* fp;
    int error = fopen_s(&fp, "dino.tmg", "rt");

    
    system("mode con cols=100");
    if (fp == NULL) {
        printf("�ҷ����� ����\n");
    }
    while (fgets(dino, 255, fp) != NULL) {
        printf(dino);
    }
    puts("");
    fclose(fp);

    printf("COOL! BYE!\n");
}

void rePrint() {

    int i, j;
    for (i = 0; i < FILED_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            gotoxy(i, j);
            printf("%c", fBuf[i][j]);
        }
    }
    gotoxy(19, 0);
    printf("SCORE : %d\n", score);
    strcpy_s(player.myWeapon, 50, "����������");
    printf("���� ��� : %s\n", player.myWeapon);
    printf("MP : %d\n", player.mp);
    printf("���� ��� : %d", player.gold);
    gotoxy(5, 24);
    int displayStage = player.nStage + 1;
    printf("���� �������� : %d\n", displayStage);
}

void initialize() {

    srand((int)time(NULL));
    //�� �ʱ�ȭ
    player.nStage = 0;
    player.pcol = LEFT_CORNER;
    player.prow = LEFT_CORNER;
    for (int i = 0; i < 4; i++) {
        enemy[i].ecol = FIRST_ENEMY_PLACE_COL;
        enemy[i].erow = FIRST_ENEMY_PLACE_ROW;
    }
    enemy[3].Boss = true;
    player.mp = 10;



    for (int i = 0; i < FILED_HEIGHT; i++) { 
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (i == player.prow && j == player.pcol) {
                bBuf[i][j] = MAP_PLAYER;
            }
            else if (player.nStage < 3 && j == enemy[player.nStage].erow && i == enemy[player.nStage].ecol && enemy[i].Boss != true) {
                bBuf[i][j] = MAP_ENEMY;
            }
            else if (j == enemy[3].erow && i == enemy[3].ecol && enemy[3].Boss == true) {
                bBuf[i][j] = bossSymbol;
            }
            else if (i == 0 || i == FILED_HEIGHT - 1 || j == 0 || j == FIELD_WIDTH - 1) {
                bBuf[i][j] = MAP_WALL;
            }
            else if (i == 1 && j == 1) {
                bBuf[i][j] = MAP_SPACE;
            }
            else {
                bBuf[i][j] = MAP_SPACE;
            }
        }
    }

    for (int i = 0; i < 36; i++) { //�������� �ʾȿ� �� ����
        int rx = rand() % (FIELD_WIDTH - 2) + 1;
        int ry = rand() % (FILED_HEIGHT - 2) + 1;
        if (rx == 1 && ry == 1) continue;
        bBuf[ry][rx] = MAP_WALL;

    }

}

void display(void) {
    int i, j;
    for (i = 0; i < FILED_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            if (bBuf[i][j] != fBuf[i][j]) { //���� ���۸� �˻�
                fBuf[i][j] = bBuf[i][j];
                gotoxy(i, j);

                printf("%c", fBuf[i][j]);
            }
        }
    }
    gotoxy(19, 0);
    printf("SCORE : %d\n", score);
    strcpy_s(player.myWeapon, 50, "����������");
    printf("���� ��� : %s\n", player.myWeapon);
    printf("MP : %d\n", player.mp);
    printf("���� ��� : %d\n", player.gold);
    printf("���� ���� : %d", player.mpitem);
    gotoxy(5, 24);
    int displayStage = player.nStage + 1;
    printf("���� �������� : %d\n", displayStage);
}

void makeEnemy() { //��Ҵٸ� ���ʹ̸� �ٽ� ����

    for (int i = 0; i < enemyAmount; i++) {
        if (enemyAmount) {
            bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
            enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
            enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
            bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;
        }
    }


}