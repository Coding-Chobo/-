#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void shuffle(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
#define ROWS 4
#define COLS 5
#define MAX 50
#define TOTAL (ROWS*COLS)
int main(void)
{   
    int sql[ROWS][COLS];
    int odd[ROWS][COLS], even[ROWS][COLS];
    int number[TOTAL];
    int number2[TOTAL];
    int reset[TOTAL], reset1[TOTAL];
    char command = 0;
    int rnd[MAX];
    int mlt, sum, max, min, num;
    int i = 0, j = 0, t = 0, temp = 0, count = 0, p = 0;
    int i2 = 0, j2 = 0, t2 = 0, temp2 = 0, count2 = 0;
    srand(time(NULL));
    for (i = 0; i < MAX; i++)
    {
        rnd[i] = i + 1;

    }
    shuffle(rnd, MAX); 

    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            sql[i][j] = rnd[i * COLS + j];
        }
    }

    
    printf("��İ� : ");   // ��İ� ���
    for (i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (sql[i][j] < 10)
            {
                printf(" %d ", sql[i][j]);
            }
            else
                printf("%d ", sql[i][j]);
            if ((j + 1) % 5 == 0) {
                printf("\n         ");
            }

        }
    }
    do
    {
        printf("\n��ɾ� �Է� : ");  // ��ɾ� �Է�
        scanf(" %c", &command);
        printf("         ");
        switch (command)
        {
        case 'a':     
            for (i = 0; i < TOTAL - 1; i++) { 
                for (j = 0; j < TOTAL - i - 1; j++) {
                    if (rnd[j] > rnd[j+1]) {
                        int temp = rnd[j];
                        rnd[j] = rnd[j + 1];
                        rnd[j + 1] = temp;
                    }
                }
            }
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    sql[i][j] = rnd[i * COLS + j];
                }
            }

            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;
        case 'd':

            for (i = 0; i < TOTAL - 1; i++) {
                for (j = 0; j < TOTAL - i - 1; j++) {
                    if (rnd[j] > rnd[j + 1]) {
                        int temp = rnd[j];
                        rnd[j] = rnd[j + 1];
                        rnd[j + 1] = temp;
                    }
                }
            }
            for (j = 0; j < COLS; j++) {
                for (int i = ROWS - 1; i > -1; i--) {
                    sql[i][j] = rnd[j * ROWS + ROWS-1 - i];
                }
            }

            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;
        case 'e':
            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] % 2 == 1) {
                        odd[i][j] = 0;
                    }
                    else
                    {
                        odd[i][j] = sql[i][j];
                    }
                }
            }
            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (odd[i][j] < 10)
                    {
                        printf(" %d ", odd[i][j]);
                    }
                    else
                        printf("%d ", odd[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;
        case 'o':
            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] % 2 == 0) {
                        even[i][j] = 0;
                    }
                    else
                    {
                        even[i][j] = sql[i][j];
                    }
                }
            }
            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (even[i][j] < 10)
                    {
                        printf(" %d ", even[i][j]);
                    }
                    else
                        printf("%d ", even[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;
        case 'm':    // �ִ밪�� ã�� ��� ���� ���� ���� ���
            max = sql[0][0];
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] > max) {
                        max = sql[i][j];
                    }
                }
            }
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    sql[i][j] += max;
                }
            }

            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }

            break;

        case 'n':    // �ּҰ��� ã�� ��� ������ �� ���� ���
            min = sql[0][0];
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] <  min) {
                        min = sql[i][j];
                    }
                }
            }
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    sql[i][j] -= min;
                }
            }

            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }

            break;

        case 'p':    // �� ���� ���� ��� ���� �� ���� 1���� ����
            for (int i = 0; i < ROWS; i++) {
                int sum = 0;
                for (int j = 0; j < COLS; j++) {
                    sum += sql[i][j];
                }
                sql[i][0] = sum;
            }
            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }

            break;
        case 'u':    //  �� ���� ���� ��� ���� �� ���� 5���� ����  
            for (int i = 0; i < ROWS; i++) {
                int mlt = 1;
                for (int j = 0; j < COLS; j++) {
                    mlt = mlt * sql[i][j];
                }
                sql[i][4 ] = mlt;
            }

            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;

        case 'r':    // ������ ���ڸ� �����ϰ� ������
            for (i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++)
                {
                    number2[i * COLS + j] = sql[i][j];
                }
            }
            while (count2 < 50)  // ���� (������ ���� �ȵǰ� ����.) ������� ����
            {
                int i2 = rand() % TOTAL;
                int j2 = rand() % TOTAL;
                int temp2 = number2[i2];
                number2[i2] = number2[j2];
                number2[j2] = temp2;
                count2++;
            }
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    sql[i][j] = number2[i * COLS + j];
                }
            }
            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }
            break;

        case 's':    // ���ο� ������ ���ڸ� �ٽ� �ڵ� �Է� (����)
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    sql[i][j] = rand() % 50 + 1;
                }
            }
            for (i = 0; i < ROWS; i++) {  // 4 * 5 ���� ���
                for (int j = 0; j < COLS; j++) {
                    if (sql[i][j] < 10)
                    {
                        printf(" %d ", sql[i][j]);
                    }
                    else
                        printf("%d ", sql[i][j]);
                    if ((j + 1) % 5 == 0) {
                        printf("\n         ");
                    }
                }
            }

            break;

        case 'q'://���α׷� ����
            break;
        }
    } while (command != 'q');
            return 0;
}
