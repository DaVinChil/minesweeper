#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>

#define LENGTH 10
#define MINES 10
#define MINE -1
#define OPENED 1
#define CLOSED 0
#define MARKED 2
#define MARK "B"
#define FAKEOPEN 100

void setUp();
void printField(int);
void createField();
void openCell(int, int);
void turn();
void botTurn();
void deepSearch(int **, int);
void getReadyForDeepSearch();
int predictTurn();
int randTurn();
void ZUGZWANG();
void showPredict();
void menu();

int statField[LENGTH][LENGTH] = {0};
int mines[LENGTH][LENGTH] = {0};
int player[LENGTH][LENGTH] = {0};
int predict[LENGTH][LENGTH] = {0};
int **cpyField;

int botturns[LENGTH * LENGTH][2] = {0};
int head = -1;

int closed = LENGTH * LENGTH;

int main() {
    menu();
}

void menu(){
    srand(time(0));
    setlocale(LC_ALL, "Rus");
    printf("CHOOSE MODE:\n");
    printf("1. PLAYER\n");
    printf("2. BOT\n");
    int choice = 2;
    scanf("%d", &choice);
    setUp();
    createField();
    if (choice == 1) {
        while (1)
        {
            printField(CLOSED);
            turn();
        }
    } else {
        printField(CLOSED);
        int y = rand() % LENGTH, x = rand() % LENGTH;
        printf("\n%d %d ПЕРВЫЙ\n", x + 1, y + 1);
        sleep(2);
        openCell(y, x);
        printField(CLOSED);
        botTurn();
    }
}

void ZUGZWANG()
{
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            if (player[i][j] == CLOSED)
            {
                printf("%d %d \n", j + 1, i + 1);
                sleep(2);
                openCell(i, j);
                printField(CLOSED);
                return;
            }
        }
    }
}

void showPredict()
{
    for (int i = -1; i <= LENGTH; i++)
    {
        if (i == -1 || i == LENGTH)
        {
            printf("  +");
        }
        else
        {
            if (i + 1 < 10)
                printf(" ");
            printf("%d|", i + 1);
        }
        for (int j = 0; j < LENGTH; j++)
        {
            if (i == -1 || i == LENGTH)
            {
                printf(" -");
            }
            else
            {

                if (player[i][j] == OPENED)
                {
                    printf(" %d", statField[i][j]);
                }
                else if (player[i][j] == MARKED || predict[i][j] == MARKED)
                {
                    printf(" %s", MARK);
                }
                else
                {
                    printf(" #");
                }
            }
        }
        if (i == -1 || i == LENGTH)
        {
            printf(" +");
        }
        else
        {
            printf(" |");
        }
        printf("\n");
    }

    printf("   ");
    for (int i = 1; i <= LENGTH; i++)
    {
        printf(" %d", i);
    }
    printf("\n");
}

void setUp()
{
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            statField[i][j] = 0;
            mines[i][j] = 0;
            player[i][j] = 0;
            predict[i][j] = 0;
            botturns[i * j][0] = 0;
            botturns[i * j][1] = 0;
        }
    }

    closed = LENGTH * LENGTH;
    head = -1;
}

void turn()
{
    printf("CHOOSE \'#\' TO OPEN\n");
    printf("TYPE \'?\' IF U WANNA MARK THE CELL\n");
    char mark;
    int x, y;
    scanf("%c", &mark);
    if (mark == '\n')
    {
        scanf("%c", &mark);
    }

    if (mark == '?')
    {
        scanf("%d%d", &x, &y);
        x--;
        y--;
        if (player[y][x] == CLOSED)
        {
            player[y][x] = MARKED;
        }
        else if (player[y][x] == MARKED)
        {
            player[y][x] = CLOSED;
        }
    }
    else
    {
        if (mark >= '0' && mark <= '9')
        {
            x = mark - '0';
            char c;
            scanf("%c", &c);
            if (c >= '0' && c <= '9')
            {
                x = (x * 10) + (c - '0');
            }
            scanf("%d", &y);
        }
        else
        {
            scanf("%d%d", &x, &y);
        }

        openCell(--y, --x);
    }
}

void openCell(int y, int x)
{
    if (mines[y][x] == MINE)
    {
        printField(OPENED);
        printf("YOU LOSE, BETTER LUCK NEXT TIME\n");
        exit(0);
    }

    if (player[y][x] == OPENED)
    {
        return;
    }

    player[y][x] = OPENED;
    closed--;
    if (statField[y][x] == 0)
    {
        for (int i = y - 1; i <= y + 1; i++)
        {
            for (int j = x - 1; j <= x + 1; j++)
            {
                if (i >= 0 && j >= 0 && i < LENGTH && j < LENGTH)
                    openCell(i, j);
            }
        }
    }

    if (closed == MINES)
    {
        printField(OPENED);
        printf("YOU WON!!!!!\n CONGRATULATES\n");
        exit(0);
    }
}

void printField(int key)
{
    for (int i = -1; i <= LENGTH; i++)
    {
        if (i == -1 || i == LENGTH)
        {
            printf("  +");
        }
        else
        {
            if (i + 1 < 10)
                printf(" ");
            printf("%d|", i + 1);
        }
        for (int j = 0; j < LENGTH; j++)
        {
            if (i == -1 || i == LENGTH)
            {
                printf(" -");
            }
            else
            {
                if (key)
                {
                    if (mines[i][j] == MINE)
                    {
                        printf(" %s", MARK);
                    }
                    else
                    {
                        printf(" %d", statField[i][j]);
                    }
                }
                else
                {
                    if (player[i][j] == OPENED)
                    {
                        printf(" %d", statField[i][j]);
                    }
                    else if (player[i][j] == MARKED)
                    {
                        printf(" %s", MARK);
                    }
                    else
                    {
                        printf(" #");
                    }
                }
            }
        }
        if (i == -1 || i == LENGTH)
        {
            printf(" +");
        }
        else
        {
            printf(" |");
        }
        printf("\n");
    }

    printf("   ");
    for (int i = 1; i <= LENGTH; i++)
    {
        printf(" %d", i);
    }
    printf("\n");
}

void createField()
{
    srand(time(NULL));
    int count = 0;

    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            statField[i][j] = 0;
            mines[i][j] = 0;
            player[i][j] = 0;
        }
    }

    while (count < MINES)
    {
        int x = rand() % LENGTH, y = rand() % LENGTH;
        if (mines[y][x] != MINE)
        {
            mines[y][x] = MINE;
            for (int i = y - 1; i <= y + 1; i++)
            {
                for (int j = x - 1; j <= x + 1; j++)
                {
                    if (i >= 0 && j >= 0 && i < LENGTH && j < LENGTH)
                        statField[i][j]++;
                }
            }
            count++;
        }
    }
}

void botTurn()
{
    int flag = 0;
    while (head >= 0)
    {
        printf("%d %d\n", botturns[head][1] + 1, botturns[head][0] + 1);
        sleep(2);
        openCell(botturns[head][0], botturns[head][1]);
        printField(CLOSED);
        head--;
    }

    while (1)
    {
        int recFlag = 0;
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                if (!player[i][j] || player[i][j] == MARKED)
                    continue;
                if (statField[i][j] == 0)
                    continue;
                int num = statField[i][j];
                int freeCell = 0;
                int cells[8][2] = {0};
                int bombs = 0;
                for (int y = i - 1; y <= i + 1; y++)
                {
                    for (int x = j - 1; x <= j + 1; x++)
                    {
                        if (y >= 0 && x >= 0 && y < LENGTH && x < LENGTH && (y != i || x != j))
                        {
                            if (player[y][x] == MARKED)
                            {
                                bombs++;
                                continue;
                            }
                            else if (player[y][x] == CLOSED)
                            {
                                cells[freeCell][0] = y;
                                cells[freeCell][1] = x;
                                freeCell++;
                            }
                        }
                    }
                }

                if (num - bombs == freeCell)
                {
                    for (int k = 0; k < freeCell; k++)
                    {
                        player[cells[k][0]][cells[k][1]] = MARKED;
                        sleep(2);
                        printf("? %d %d\n", cells[k][1] + 1, cells[k][0] + 1);
                        sleep(2);
                        printField(CLOSED);
                        recFlag = 1;
                    }
                }
                else if (num == bombs && freeCell > 0)
                {
                    for (int k = 0; k < freeCell; k++)
                    {
                        botturns[++head][0] = cells[k][0];
                        botturns[head][1] = cells[k][1];
                    }
                    flag = 1;
                    recFlag = 1;
                }

                if (flag)
                    break;
            }
            if (flag)
                break;
        }
        if (flag)
            break;
        if (recFlag == 0)
        {
            getReadyForDeepSearch();
            // printf("ЗАШЁЛ\n");
            deepSearch(cpyField, 0);
            // printf("ВЫШЁЛ\n");
            int res = predictTurn();
            if (res == -1)
            {
                res = randTurn();
                if (res == -1)
                {
                    ZUGZWANG();
                }
            }
        }
    }

    botTurn();
}

void getReadyForDeepSearch()
{
    cpyField = (int **)calloc(LENGTH, sizeof(int *));
    for (int i = 0; i < LENGTH; i++)
    {
        cpyField[i] = (int *)calloc(LENGTH, sizeof(int));
    }

    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            cpyField[i][j] = player[i][j];
            predict[i][j] = player[i][j];
        }
    }
}

void deepSearch(int **copyField, int deep) {
    // if (deep < 20)
    // {
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (!copyField[i][j] || copyField[i][j] == MARKED || copyField[i][j] == FAKEOPEN)
                continue;
            if (statField[i][j] == 0)
                continue;

            int num = statField[i][j];
            int freeCell = 0;
            int cells[8][2] = {0};
            int bombs = 0;
            for (int y = i - 1; y <= i + 1; y++) {
                for (int x = j - 1; x <= j + 1; x++) {
                    if (y >= 0 && x >= 0 && y < LENGTH && x < LENGTH && (y != i || x != j)) {
                        if (copyField[y][x] == MARKED) {
                            bombs++;
                            continue;
                        }
                        else if (copyField[y][x] == CLOSED)
                        {
                            cells[freeCell][0] = y;
                            cells[freeCell][1] = x;
                            freeCell++;
                        }
                    }
                }
            }

            if (bombs == num && freeCell > 0)
            {
                for (int k = 0; k < freeCell; k++)
                {
                    copyField[cells[k][0]][cells[k][1]] = FAKEOPEN;
                }
            }

            if (bombs > num || num - bombs > freeCell)
            {
                return;
            }
            else if (num - bombs > 0)
            {
                for (int k = 0; k < freeCell; k++)
                {
                    int save = copyField[cells[k][0]][cells[k][1]];
                    copyField[cells[k][0]][cells[k][1]] = MARKED;
                    int **term = (int **)calloc(LENGTH, sizeof(int *));
                    for (int i = 0; i < LENGTH; i++)
                    {
                        term[i] = (int *)calloc(LENGTH, sizeof(int));
                    }
                    for (int y = 0; y < LENGTH; y++)
                    {
                        for (int x = 0; x < LENGTH; x++)
                        {
                            term[y][x] = copyField[y][x];
                        }
                    }
                    deepSearch(term, ++deep);
                    copyField[cells[k][0]][cells[k][1]] = save;
                }
                return;
            }
        }
    }
    // }

    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            if (copyField[i][j] == MARKED)
            {
                predict[i][j] = MARKED;
            }
        }
    }
}

int predictTurn()
{
    // showPredict();

    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            if (!player[i][j] || player[i][j] == MARKED)
                continue;
            if (statField[i][j] == 0)
                continue;

            for (int y = i - 1; y <= i + 1; y++)
            {
                for (int x = j - 1; x <= j + 1; x++)
                {
                    if (y >= 0 && x >= 0 && y < LENGTH && x < LENGTH && (y != i || x != j))
                    {
                        if (predict[y][x] != MARKED && player[y][x] == CLOSED)
                        {
                            printf("\n%d %d \n", x + 1, y + 1);
                            sleep(2);
                            openCell(y, x);
                            printField(CLOSED);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return -1;
}

int randTurn()
{
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            if (!player[i][j] || player[i][j] == MARKED){ continue;}
            if (statField[i][j] == 0){ continue;}

            for (int y = i - 1; y <= i + 1; y++) {
                for (int x = j - 1; x <= j + 1; x++) {
                    if (y >= 0 && x >= 0 && y < LENGTH && x < LENGTH && (y != i || x != j)) {
                        if (player[y][x] == CLOSED)
                        {
                            printf("\n%d %d \n", x + 1, y + 1);
                            sleep(2);
                            openCell(y, x);
                            printField(CLOSED);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return -1;
}