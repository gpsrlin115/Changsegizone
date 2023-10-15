#include "common.h"
#include "cio.h"

//몹을 세번 잡으면 다음 단계로(O), 2스테이지는 두마리 , 3스테이지는 세마리 다잡으면 보스출현 그후 엔딩
//몹을 잡으면 골드를 2씩 떨군다 
//MP를 채워줄 포션 구현

//왜 mp는 10에서 스킬을 쓰면 90으로 변할까
//왜 포션은 빨면 +10을 하는데 정상적으로 10만 올라갈까.
//왜 상점에서 나가면 벽들이 제대로 표기가 안되는걸까.
//보스를 잡으면 왜 스테이지가 호로로로록 올라가서 오버플로우가 생길까.

struct Player_info player;
struct Enemy_info enemy[4];

int enemyRoute = 0; //랜덤으로 생성된 루트를 저장하는 변수


enum DIRECTION
{
    UP, DOWN, LEFT, RIGHT
};

char bBuf[FILED_HEIGHT][FIELD_WIDTH]; //맵 뒷버퍼
char fBuf[FILED_HEIGHT][FIELD_WIDTH]; //앞버퍼
//int nSpeed = 1000; //스피드




int get_key() //키입력
{
    int key = K_NONE;

    if (_kbhit()) { key = _getch(); }
    if (key == K_ARROW) 
    {
        key = _getch();
        switch (key)
        {
        case K_LEFT:
            player.dirCol = DIR_LEFT;
            break;
        case K_RIGHT:
            player.dirCol = DIR_RIGHT;
            break;
        case K_UP:
            player.dirRow = DIR_UP;
            break;
        case K_DOWN:
            player.dirRow = DIR_DOWN;
            break;
                               
                
        }
    }
    if (key == drinkMP && player.mpitem != 0) {
        player.mpitem--;
        player.mp += 10;
    }
    if (key == SHOP_KEY) {
        system("cls");
        int potionAmount;
        printf("구입하실 마나포션의 개수를 입력하세요 \n개당 1골드입니다.");
        scanf_s("%d", &potionAmount);
        if (player.gold <= 0) {
            printf("잔액이 부족합니다.");
            Sleep(1000);
            system("cls");
            rePrint();
        }
        else {
            player.mpitem += potionAmount;
            player.gold -= potionAmount;
            system("cls");
            rePrint();
        }
        
    }
    /*맵안에움직이는공간 bBuf[1~16][1~20]
			
            1. z 키를 누른다.
            2. 십자 모양으로 별이 생긴다
            3.1. 적이 별에 닿았을 경우
                 별에 닿은 적이 사라지고
				 별이 사라진다.
                 사라진 만큼 새로운 적이 생긴다
                 score++ (1점씩 업)
                
            3.2. 별에 닿은 적이 없는 경우
                 생겼던 별이 없어진다

    }*/

    return key;
}

void turn(int key) { // 진행방향 바꾸는 함수

    if (key == K_LEFT) {
        player.dirCol = DIR_LEFT;
    }
    else if (key == K_RIGHT) {
        player.dirCol = DIR_RIGHT;
    }

    if (key == K_UP) {
        player.dirRow = DIR_UP;
    }
    else if (key == K_DOWN) {
        player.dirRow = DIR_DOWN;
    }


    ////방향전환
    //if (dirCol == DIR_RIGHT)
    //{
    //    if (pcol >= RIGHT_CORNER)
    //        dirCol = DIR_LEFT;
    //}

    //else if (dirCol == DIR_LEFT)
    //{
    //    if (pcol <= LEFT_CORNER)
    //        dirCol = DIR_RIGHT;
    //}

    //if (dirRow == DIR_DOWN)
    //{
    //    if (erow >= FILED_HEIGHT - 2)
    //        dirRow = DIR_UP;
    //}

    //else if (dirRow == DIR_UP)
    //{
    //    if (erow <= FIRST_ENEMY_PLACE)
    //        dirRow = DIR_DOWN;
    //}
}

int moveP() { //플레이어 움직임
	
	if (player.dirRow == DIR_UP && player.prow > 0 && bBuf[player.prow - 1][player.pcol] != '#' && bBuf[player.prow - 1][player.pcol] != bossSymbol) {
        bBuf[player.prow][player.pcol] = MAP_SPACE;
        player.prow += player.dirRow;
        for (int i = 0; i < player.nStage+1; i++) {
            if (player.prow == enemy[i].erow && player.pcol == enemy[i].ecol) { // 플레이어가 적을 잡았을 때
                /*bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
                enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
                enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;*/
                makeEnemy();
                enemyKill++;
                score++;
                player.gold += 2;
            }
        }
        
        bBuf[player.prow][player.pcol] = MAP_PLAYER;
        player.dirRow = 0;
        
    }//위
    
	if (player.dirRow == DIR_DOWN && player.prow < 17 && bBuf[player.prow + 1][player.pcol] != '#' && bBuf[player.prow - 1][player.pcol] != bossSymbol) {
        bBuf[player.prow][player.pcol] = MAP_SPACE;

        player.prow += player.dirRow;
        for (int i = 0; i < player.nStage+1; i++) {
            if (player.prow == enemy[i].erow && player.pcol == enemy[i].ecol) {
                /*bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
                enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
                enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;*/
                makeEnemy();
                enemyKill++;
                score++;
                player.gold += 2;
            }
        }
        bBuf[player.prow][player.pcol] = MAP_PLAYER;
        player.dirRow = 0;
      
    }//아래
	if (player.dirCol == DIR_LEFT && player.pcol > 0 && bBuf[player.prow][player.pcol - 1] != '#' && bBuf[player.prow - 1][player.pcol] != bossSymbol) {
        bBuf[player.prow][player.pcol] = MAP_SPACE;

        player.pcol += player.dirCol;
        for (int i = 0; i < player.nStage+1; i++) {
            if (player.prow == enemy[i].erow && player.pcol == enemy[i].ecol) {
                /*bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
                enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
                enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;*/
                makeEnemy();
                enemyKill++;
                score++;
                player.gold += 2;
            }
        }
        
        bBuf[player.prow][player.pcol] = MAP_PLAYER;
        
        player.dirCol = 0;
     

    }//왼쪽
	if (player.dirCol == DIR_RIGHT && player.pcol < 20 && bBuf[player.prow][player.pcol + 1] != '#' && bBuf[player.prow - 1][player.pcol] != bossSymbol) {
        bBuf[player.prow][player.pcol] = MAP_SPACE;
        player.pcol += player.dirCol;
        for (int i = 0; i < player.nStage+1; i++) {
            if (player.prow == enemy[i].erow && player.pcol == enemy[i].ecol) {
                /*bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;
                enemy[i].ecol = rand() % FIRST_ENEMY_PLACE_COL + 1;
                enemy[i].erow = rand() % FIRST_ENEMY_PLACE_ROW + 1;
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;*/
                makeEnemy();
                enemyKill++;
                score++;
                player.gold += 2;
            }
        }
        
        bBuf[player.prow][player.pcol] = MAP_PLAYER;
        player.dirCol = 0;
        
    } // 오른쪽
    
    return enemyKill;

}

void moveE() { //적 움직임
 
        enemyRoute = (rand() % 4); //0~3까지 랜덤으로 생성

        for (int i = 0; i < player.nStage+1; i++) { //몹 갯수만큼
            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == UP && bBuf[enemy[i].erow - 1][enemy[i].ecol] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirRow = DIR_UP;
                enemy[i].erow += enemy[i].dirRow;
                if(enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;


            }//위

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == DOWN && bBuf[enemy[i].erow + 1][enemy[i].ecol] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirRow = DIR_DOWN;
                enemy[i].erow += enemy[i].dirRow;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//아래

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == LEFT && bBuf[enemy[i].erow][enemy[i].ecol - 1] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirCol = DIR_LEFT;
                enemy[i].ecol += enemy[i].dirCol;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//왼쪽

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == RIGHT && bBuf[enemy[i].erow][enemy[i].ecol + 1] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirCol = DIR_RIGHT;
                enemy[i].ecol += enemy[i].dirCol;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//오른쪽
            if (enemy[i].tick == 100) enemy[i].tick = 0;
        }
        
        
		
 }

void playerSkill(int key) {
    
    if (key == zskill && player.mp > 0) {
        player.mp = player.mp -1;
        enemyKill = zSkill();
        player.gold += 2;
        display();
        Sleep(1500);
        zSkillNormalize();
        makeEnemy();
        system("cls");
        rePrint();
    }
    if (key == xskill && player.mp > 0) {
        player.mp = player.mp - 1;
        enemyKill = xSkill();
        player.gold += 2;
        display();
        Sleep(1500);
        xSkillNormalize();
        makeEnemy();
        system("cls");
        rePrint();
    }
}



//1. 스킬로 적을 몇마리나 잡았는지 세는 카운트를 만든다
//2. zSkill 함수 돌 때 적 한마리 만날 때마다 카운트 하나씩 늘려준다
//3. 적을 생성하는 함수를 따로 정의해준다
//4. sleep 끝나고 normalize 한 다음에 적 잡은 수만큼 적을 만들어준다
//Boss 는 무조건 스킬로 잡아야한다.
int zSkill() {
        for (int i = 1; i < 6; i++) {
            if (player.prow + i < FILED_HEIGHT - 1 && bBuf[player.prow + i][player.pcol] != MAP_WALL) {
                if (bBuf[player.prow + i][player.pcol] == MAP_ENEMY) {
                    score++;
                    enemyKill++;
                }
                if (bBuf[player.prow + i][player.pcol] == bossSymbol) {
                    enemy[3].Boss = false;
                }
                bBuf[player.prow + i][player.pcol] = skillEfect;
            }
            if (player.pcol + i < FIELD_WIDTH - 1 && bBuf[player.prow][player.pcol + i] != MAP_WALL) {
                if (bBuf[player.prow][player.pcol + i] == MAP_ENEMY) {
                    score++;
                    enemyKill++;
                }
                if (bBuf[player.prow][player.pcol+i] == bossSymbol) {
                    enemy[3].Boss = false;
                }
                bBuf[player.prow][player.pcol + i] = skillEfect;
            }
            if (player.pcol - i > 0 && bBuf[player.prow][player.pcol - i] != MAP_WALL) {
                if (bBuf[player.prow][player.pcol - i] == MAP_ENEMY) {
                    score++;
                    enemyKill++;
                }
                if (bBuf[player.prow][player.pcol-i] == bossSymbol) {
                    enemy[3].Boss = false;
                }
                bBuf[player.prow][player.pcol - i] = skillEfect;
            }
            if (player.prow - i > 0 && bBuf[player.prow - i][player.pcol] != MAP_WALL) {
                if (bBuf[player.prow - i][player.pcol] == MAP_ENEMY) {
                    score ++;
                    enemyKill++;
                }
                if (bBuf[player.prow - i][player.pcol] == bossSymbol) {
                    enemy[3].Boss = false;
                }
                bBuf[player.prow - i][player.pcol] = skillEfect;
            }
        }
        return enemyKill;
 }

void zSkillNormalize() {
    for (int i = 1; i < 6; i++) {
        if (player.prow + i < FILED_HEIGHT - 1 && bBuf[player.prow + i][player.pcol] != MAP_WALL) {
            bBuf[player.prow + i][player.pcol] = MAP_SPACE;
        }
        if (player.pcol + i < FIELD_WIDTH - 1 && bBuf[player.prow][player.pcol + i] != MAP_WALL) {
            bBuf[player.prow][player.pcol + i] = MAP_SPACE;
        }
        if (player.pcol - i > 0 && bBuf[player.prow][player.pcol - i] != MAP_WALL) {
           
            bBuf[player.prow][player.pcol - i] = MAP_SPACE;
        }
        if (player.prow - i > 0 && bBuf[player.prow - i][player.pcol] != MAP_WALL) {
            bBuf[player.prow - i][player.pcol] = MAP_SPACE;
        }
    }
}



int xSkill() {
    int enemyKill = 0;
    for (int i = 1; i < 6; i++) {
        if (player.prow + i < FILED_HEIGHT - 1 && player.pcol + i < FIELD_WIDTH - 1 && bBuf[player.prow + i][player.pcol + i] != MAP_WALL) {
            if (bBuf[player.prow + i][player.pcol + i] == MAP_ENEMY) { // 아래 오른쪽
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow + i][player.pcol + i] == bossSymbol) {
                enemy[3].Boss = false;
            }
            bBuf[player.prow + i][player.pcol + i] = skillEfect;
        }
        if (player.prow + i > 0 && player.pcol - i < FIELD_WIDTH - 1 && bBuf[player.prow - i][player.pcol + i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol + i] == MAP_ENEMY) { // 위 오른쪽
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow -i ][player.pcol+i] == bossSymbol) {
                enemy[3].Boss = false;
            }
            bBuf[player.prow - i][player.pcol + i] = skillEfect;
        }
        if (player.prow - i > 0 && player.pcol - i > 0 && bBuf[player.prow - i][player.pcol - i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol - i] == MAP_ENEMY) { // 위 왼쪽
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow-i][player.pcol - i] == bossSymbol){
                enemy[3].Boss = false;
            }
            bBuf[player.prow - i][player.pcol - i] = skillEfect;
        }
        if (player.prow + i < FILED_HEIGHT -1 && player.pcol - i > 0 && bBuf[player.prow + i][player.pcol - i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol] == MAP_ENEMY) { // 아래 왼쪽
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow + i][player.pcol - i] == bossSymbol) {
                enemy[3].Boss = false;
            }
            bBuf[player.prow + i][player.pcol - i] = skillEfect;
        }
    }
    return enemyKill;
}

void xSkillNormalize() {
    for (int i = 1; i < 6; i++) {
        if (player.prow + i < FILED_HEIGHT - 1 && player.pcol + i < FIELD_WIDTH - 1 && bBuf[player.prow + i][player.pcol + i] != MAP_WALL) {
            bBuf[player.prow + i][player.pcol + i] = MAP_SPACE;
        }
        if (player.prow + i > 0 && player.pcol - i < FIELD_WIDTH - 1 && bBuf[player.prow - i][player.pcol + i] != MAP_WALL) {
            bBuf[player.prow - i][player.pcol + i] = MAP_SPACE;
        }
        if (player.prow - i > 0 && player.pcol - i > 0 && bBuf[player.prow - i][player.pcol - i] != MAP_WALL) {

            bBuf[player.prow - i][player.pcol - i] = MAP_SPACE;
        }
        if (player.prow + i < FILED_HEIGHT - 1 && player.pcol - i > 0 && bBuf[player.prow + i][player.pcol - i] != MAP_WALL) {
            bBuf[player.prow + i][player.pcol - i] = MAP_SPACE;
        }
    }
}
