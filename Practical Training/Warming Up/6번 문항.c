#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void calculate1(const int arr[], const char op[], int* result) {
    int tempArr[5];
    int a;
    for (int i = 0; i < 5; i++) {
        tempArr[i] = arr[i];
    }
    for (int i = 0; i < 4; i++)
    {
        if (i > 2) {
            if (op[i] == '*')
            {
                tempArr[i] = tempArr[i] * tempArr[i + 1];
            }
            else if (op[i] == '/')
            {
                tempArr[i] = tempArr[i] / tempArr[i + 1];
            }
        }
        else
        {
            if (op[i] == '*')
            {
                tempArr[i] = tempArr[i] * tempArr[i + 1];
                for (int j = i; j < 4; j++)
                {
                    tempArr[i + 1] = tempArr[i + 2];
                }     
            }
            else if (op[i] == '/')
            {
                tempArr[i] = tempArr[i] / tempArr[i + 1];
                for (int j = i; j < 4; j++)
                {
                    tempArr[i + 1] = tempArr[i + 2];
                }
            }
        }
        a++;
    }
    for (int i = 0; i < 4-a; i++) {
        switch (op[i]) {
        case '+':
            tempArr[i + 1] = tempArr[i] + tempArr[i + 1];
            break;
        case '-':
            tempArr[i + 1] = tempArr[i] - tempArr[i + 1];
            break;
        default:
            break;
        }
    }
    *result = tempArr[4-a];
}


void calculate2(const int arr[], const char op[], int* result) {
    int tempArr[5];
    for (int i = 0; i < 5; i++) {
        tempArr[i] = arr[i];
    }
        for (int i = 0; i < 4; i++) {
            switch (op[i]) {
            case '+':
                tempArr[i + 1] = tempArr[i] + tempArr[i + 1];
                break;
            case '-':
                tempArr[i + 1] = tempArr[i] - tempArr[i + 1];
                break;
            case '*':
                tempArr[i + 1] = tempArr[i] * tempArr[i + 1];
                break;
            case '/':
                if (tempArr[i + 1] != 0) {
                    tempArr[i + 1] = tempArr[i] / tempArr[i + 1];
                }
                else {
                    printf("Error: Division by zero.\n");
                    *result = 0; // 오류 처리
                    return;
                }
                break;
            default:
                break;
            }
        }
        *result = tempArr[4];
    }
void calculate3(const int arr[], const char op[], int* result) {
    int tempArr[5];
    int temparr2[4];
    for (int i = 0; i < 5; i++) {
        tempArr[4-i] = arr[i];
    }
    for (int i = 0; i < 4; i++) {
        temparr2[3 - i] = op[i];
    }
    for (int i = 0; i < 4; i++) {
        switch (temparr2[i]) {
        case '+':
            tempArr[i + 1] = tempArr[i] + tempArr[i + 1];
            break;
        case '-':
            tempArr[i + 1] = tempArr[i] - tempArr[i + 1];
            break;
        case '*':
            tempArr[i + 1] = tempArr[i] * tempArr[i + 1];
            break;
        case '/':
            if (tempArr[i + 1] != 0) {
                tempArr[i + 1] = tempArr[i] / tempArr[i + 1];
            }
            else {
                printf("Error: Division by zero.\n");
                *result = 0;
                return;
            }
            break;
        default:
            break;
        }
    }
    *result = tempArr[4];
}
int main() {
    int number[5];
    char operator[4];
    char command = 0;
    int i,j,t,temp, result=0;
    srand(time(NULL));
    for ( i = 0; i < 5; i++) // 숫자 입력
    {
        do
        {
            printf("%d번째 숫자 : ",i+1);
            scanf(" %d",&number[i]);
        } while (number[i] <= -100 || number[i] >= 100);
    }
    for (i = 0; i < 4; i++)  // 연산자 입력
    {
        do
        {
            printf("%d번째 연산자 : ", i + 1);
            scanf(" %c", &operator[i]);
        } while (operator[i] !='+' && operator[i] != '-'&& operator[i] != '*'&& operator[i] != '/');
    }
    for (i = 0; i < 100; i++)   //숫자 순서를 랜덤하게 설정
    {
        j = rand() % 5;
        t = rand() % 5;
        temp = number[j];
        number[j] = number[t];
        number[t] = temp;
    }
    for (i = 0; i < 100; i++)   //연산자 순서를 랜덤하게 설정
    {
        j = rand() % 4;
        t = rand() % 4;
        temp = operator[j];
        operator[j] = operator[t];
        operator[t] = temp;
    }
    printf("Number to caculating order %d %c %d %c %d %c %d %c %d",number[0],operator[0], number[1], operator[1],number[2], operator[2],number[3], operator[3],number[4]);
    do
    {
        printf("\n명령어 : ");
        scanf(" %c",&command);
        switch (command)
        {
        case'1' : 
            calculate11(number, operator,&result);
            
            printf("%d", &result);
            break;
        case'2':
            calculate2(number, operator, &result);
            printf("계산 : ((((%d %c %d) %c %d) %c %d %c %d) = %d", number[0], operator[0], number[1], operator[1], number[2], operator[2], number[3], operator[3], number[4], result);
            break;
        case'3':
            calculate3(number, operator, &result);
            printf("계산 : ((((%d %c %d) %c %d) %c %d) %c %d) = %d", number[4], operator[3], number[3], operator[2], number[2], operator[1], number[1], operator[0], number[0], result);
            break;
        case'4':
            srand(time(NULL));
            for (i = 0; i < 5; i++) // 숫자 입력
            {
                do
                {
                    printf("%d번째 숫자 : ", i + 1);
                    scanf(" %d", &number[i]);
                } while (number[i] <= -100 || number[i] >= 100);
            }
            for (i = 0; i < 4; i++)  // 연산자 입력
            {
                do
                {
                    printf("%d번째 연산자 : ", i + 1);
                    scanf(" %c", &operator[i]);
                } while (operator[i] != '+' && operator[i] != '-' && operator[i] != '*' && operator[i] != '/');
            }
            for (i = 0; i < 100; i++)   //숫자 순서를 랜덤하게 설정
            {
                j = rand() % 5;
                t = rand() % 5;
                temp = number[j];
                number[j] = number[t];
                number[t] = temp;
            }
            for (i = 0; i < 100; i++)   //연산자 순서를 랜덤하게 설정
            {
                j = rand() % 4;
                t = rand() % 4;
                temp = operator[j];
                operator[j] = operator[t];
                operator[t] = temp;
            }
            printf("Number to caculating order %d %c %d %c %d %c %d %c %d", number[0], operator[0], number[1], operator[1], number[2], operator[2], number[3], operator[3], number[4]);
            break;
        case'r':
            srand(time(NULL));
            for (i = 0; i < 5; i++) // 숫자 입력
            {
                do
                {
                    printf("%d번째 숫자 : ", i + 1);
                    scanf(" %d", &number[i]);
                } while (number[i] <= -100 || number[i] >= 100);
            }
            for (i = 0; i < 4; i++)  // 연산자 입력
            {
                do
                {
                    printf("%d번째 연산자 : ", i + 1);
                    scanf(" %c", &operator[i]);
                } while (operator[i] != '+' && operator[i] != '-' && operator[i] != '*' && operator[i] != '/');
            }
            for (i = 0; i < 100; i++)   //숫자 순서를 랜덤하게 설정
            {
                j = rand() % 5;
                t = rand() % 5;
                temp = number[j];
                number[j] = number[t];
                number[t] = temp;
            }
            for (i = 0; i < 100; i++)   //연산자 순서를 랜덤하게 설정
            {
                j = rand() % 4;
                t = rand() % 4;
                temp = operator[j];
                operator[j] = operator[t];
                operator[t] = temp;
            }
            printf("Number to caculating order %d %c %d %c %d %c %d %c %d", number[0], operator[0], number[1], operator[1], number[2], operator[2], number[3], operator[3], number[4]);
            break;
        default:
            break;
        }
    } while (command != 'q');
    
    return 0;
}