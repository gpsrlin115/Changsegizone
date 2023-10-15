#include "common.h"
#include "cio.h"

//���� ���� ������ ���� �ܰ��(O), 2���������� �θ��� , 3���������� ������ �������� �������� ���� ����
//���� ������ ��带 2�� ������ 
//MP�� ä���� ���� ����

//�� mp�� 10���� ��ų�� ���� 90���� ���ұ�
//�� ������ ���� +10�� �ϴµ� ���������� 10�� �ö󰥱�.
//�� �������� ������ ������ ����� ǥ�Ⱑ �ȵǴ°ɱ�.
//������ ������ �� ���������� ȣ�ηηη� �ö󰡼� �����÷ο찡 �����.

struct Player_info player;
struct Enemy_info enemy[4];

int enemyRoute = 0; //�������� ������ ��Ʈ�� �����ϴ� ����


enum DIRECTION
{
    UP, DOWN, LEFT, RIGHT
};

char bBuf[FILED_HEIGHT][FIELD_WIDTH]; //�� �޹���
char fBuf[FILED_HEIGHT][FIELD_WIDTH]; //�չ���
//int nSpeed = 1000; //���ǵ�




int get_key() //Ű�Է�
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
        printf("�����Ͻ� ���������� ������ �Է��ϼ��� \n���� 1����Դϴ�.");
        scanf_s("%d", &potionAmount);
        if (player.gold <= 0) {
            printf("�ܾ��� �����մϴ�.");
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
    /*�ʾȿ������̴°��� bBuf[1~16][1~20]
			
            1. z Ű�� ������.
            2. ���� ������� ���� �����
            3.1. ���� ���� ����� ���
                 ���� ���� ���� �������
				 ���� �������.
                 ����� ��ŭ ���ο� ���� �����
                 score++ (1���� ��)
                
            3.2. ���� ���� ���� ���� ���
                 ����� ���� ��������

    }*/

    return key;
}

void turn(int key) { // ������� �ٲٴ� �Լ�

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


    ////������ȯ
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

int moveP() { //�÷��̾� ������
	
	if (player.dirRow == DIR_UP && player.prow > 0 && bBuf[player.prow - 1][player.pcol] != '#' && bBuf[player.prow - 1][player.pcol] != bossSymbol) {
        bBuf[player.prow][player.pcol] = MAP_SPACE;
        player.prow += player.dirRow;
        for (int i = 0; i < player.nStage+1; i++) {
            if (player.prow == enemy[i].erow && player.pcol == enemy[i].ecol) { // �÷��̾ ���� ����� ��
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
        
    }//��
    
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
      
    }//�Ʒ�
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
     

    }//����
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
        
    } // ������
    
    return enemyKill;

}

void moveE() { //�� ������
 
        enemyRoute = (rand() % 4); //0~3���� �������� ����

        for (int i = 0; i < player.nStage+1; i++) { //�� ������ŭ
            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == UP && bBuf[enemy[i].erow - 1][enemy[i].ecol] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirRow = DIR_UP;
                enemy[i].erow += enemy[i].dirRow;
                if(enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;


            }//��

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == DOWN && bBuf[enemy[i].erow + 1][enemy[i].ecol] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirRow = DIR_DOWN;
                enemy[i].erow += enemy[i].dirRow;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//�Ʒ�

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == LEFT && bBuf[enemy[i].erow][enemy[i].ecol - 1] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirCol = DIR_LEFT;
                enemy[i].ecol += enemy[i].dirCol;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//����

            if (enemy[i].tick % enemyMoveCount == 0 && enemyRoute == RIGHT && bBuf[enemy[i].erow][enemy[i].ecol + 1] != '#') {
                bBuf[enemy[i].erow][enemy[i].ecol] = MAP_SPACE;

                enemy[i].dirCol = DIR_RIGHT;
                enemy[i].ecol += enemy[i].dirCol;
                if (enemy[i].Boss == true)
                    bBuf[enemy[i].erow][enemy[i].ecol] = bossSymbol;
                else
                    bBuf[enemy[i].erow][enemy[i].ecol] = MAP_ENEMY;

            }//������
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



//1. ��ų�� ���� ����� ��Ҵ��� ���� ī��Ʈ�� �����
//2. zSkill �Լ� �� �� �� �Ѹ��� ���� ������ ī��Ʈ �ϳ��� �÷��ش�
//3. ���� �����ϴ� �Լ��� ���� �������ش�
//4. sleep ������ normalize �� ������ �� ���� ����ŭ ���� ������ش�
//Boss �� ������ ��ų�� ��ƾ��Ѵ�.
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
            if (bBuf[player.prow + i][player.pcol + i] == MAP_ENEMY) { // �Ʒ� ������
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow + i][player.pcol + i] == bossSymbol) {
                enemy[3].Boss = false;
            }
            bBuf[player.prow + i][player.pcol + i] = skillEfect;
        }
        if (player.prow + i > 0 && player.pcol - i < FIELD_WIDTH - 1 && bBuf[player.prow - i][player.pcol + i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol + i] == MAP_ENEMY) { // �� ������
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow -i ][player.pcol+i] == bossSymbol) {
                enemy[3].Boss = false;
            }
            bBuf[player.prow - i][player.pcol + i] = skillEfect;
        }
        if (player.prow - i > 0 && player.pcol - i > 0 && bBuf[player.prow - i][player.pcol - i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol - i] == MAP_ENEMY) { // �� ����
                score++;
                enemyKill++;
            }
            if (bBuf[player.prow-i][player.pcol - i] == bossSymbol){
                enemy[3].Boss = false;
            }
            bBuf[player.prow - i][player.pcol - i] = skillEfect;
        }
        if (player.prow + i < FILED_HEIGHT -1 && player.pcol - i > 0 && bBuf[player.prow + i][player.pcol - i] != MAP_WALL) {
            if (bBuf[player.prow - i][player.pcol] == MAP_ENEMY) { // �Ʒ� ����
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
