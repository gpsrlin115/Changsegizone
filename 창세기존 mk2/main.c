/*
* 스테이지 적들 모두 잡아야 스테이지 변화 해야함
* 무기 추가 필요
* 
* 1-1) 정리 I (O)
* 1-2) 정리 II (O)
* 1-3) intro(), outro() (O)
* 1-4) 2차원 배열 (O)
* 1-5) 더블 버퍼링 (O)
* 1-6) 동기화 (Player) (O)
* 1-7) 동기화 (Enemy) (O)
* 2-1) 분할 컴파일 (O)
* 2-2) 맵 만들기 (O)
* 2-3,4) 오브젝트 만들기 (O)
* 2-5) 충돌 이벤트 (O)
* 2-6) 스킬 이펙트 (O)
* 2-7) 상태 표시 (O)
* 2-8) 스테이지 (O)
* 2-9) 추가기능 만들기 : 골드와 상점 상호작용, 마나, 마나회복아이템, 보스 공략법 변화 (스킬을 사용해서 잡아야 게임끝)
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
        if (key == K_QUIT) { //q누르면 끝
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

        if (player.nStage < 3 && enemyKill == 3) { // 다음 스테이지로
            player.nStage++;
            enemyAmount++;
            enemyKill = 0;
        }
        if (enemy[3].Boss == false) { // 클리어 조건
            break;
        }
		
    }
    
    outro();
    return 0;
}

// (행, 열)로 입력 받아서 (열, 행) 위치로 커서를 옮겨주는 함수
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
        printf("불러오기 실패\n");
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
    strcpy_s(player.myWeapon, 50, "나무지팡이");
    printf("현재 장비 : %s\n", player.myWeapon);
    printf("MP : %d\n", player.mp);
    printf("소지 골드 : %d", player.gold);
    gotoxy(5, 24);
    int displayStage = player.nStage + 1;
    printf("현재 스테이지 : %d\n", displayStage);
}

void initialize() {

    srand((int)time(NULL));
    //맵 초기화
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

    for (int i = 0; i < 36; i++) { //랜덤으로 맵안에 벽 생성
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
            if (bBuf[i][j] != fBuf[i][j]) { //더블 버퍼링 검사
                fBuf[i][j] = bBuf[i][j];
                gotoxy(i, j);

                printf("%c", fBuf[i][j]);
            }
        }
    }
    gotoxy(19, 0);
    printf("SCORE : %d\n", score);
    strcpy_s(player.myWeapon, 50, "나무지팡이");
    printf("현재 장비 : %s\n", player.myWeapon);
    printf("MP : %d\n", player.mp);
    printf("소지 골드 : %d\n", player.gold);
    printf("마나 포션 : %d", player.mpitem);
    gotoxy(5, 24);
    int displayStage = player.nStage + 1;
    printf("현재 스테이지 : %d\n", displayStage);
}

void makeEnemy() { //잡았다면 에너미를 다시 생성

    for (int i = 0; i < enemyAmount; i++) {
        if (enemyAmount) {
            bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
            enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
            enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
            bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;
        }
    }


}