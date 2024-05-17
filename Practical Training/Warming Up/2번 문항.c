#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define count 50
int main()
{
    int rnd[count], op, op2 = 0, num, num1[count], num2, num3, i, j, temp, d3 = 0, d5 = 0, d15 = 0, nc = 0, rnd1[count], rnd2[count], rnd3[count];
    char command;
    
    srand(time(NULL));
	printf("Input number : ");
	scanf("%d", &op);
    for (i = 0; i < count; i++) {
        
        do {
            num = (rand()%op) + 1; 
            for (j = 0; j < i; j++) {
                if (rnd[j] == num) break;
            }
        } while (j != i);

        rnd[i] = num;
    }
    printf("Generted Numbers :\n");
    for (i = 0; i < count; i++)
    { 
        printf("%d ", rnd[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
   
    do
    {
        printf("\nCommand: ");
        scanf(" %c", &command);
        switch (command)
        {
        case'a':    //버블정렬로 오름차순 만들기
            for (i = 0; i < count - 1; i++)
            {
                for (j = i + 1; j < count; j++)
                {
                    if (rnd[i] > rnd[j])
                    {
                        temp = rnd[i];
                        rnd[i] = rnd[j];
                        rnd[j] = temp;
                    }
                }
            }
            for (i = 0; i < count; i++)
            {
                printf("%d ", rnd[i]);
                if ((i + 1) % 10 == 0) printf("\n");
            }
            break;
        case 'd':   //버블정렬로 내림차순 만들기
            for (i = 0; i < count - 1; i++)
            {
                for (j = i + 1; j < count; j++)
                {
                    if (rnd[i] < rnd[j])
                    {
                        temp = rnd[i];
                        rnd[i] = rnd[j];
                        rnd[j] = temp;
                    }
                }
            }
            for (i = 0; i < count; i++)
            {
                printf("%d ", rnd[i]);
                if ((i + 1) % 10 == 0) printf("\n");
            }
            break;
        case 'e':   //3의 배수 출력후 합 구하기
            for (i = 0; i < count - 1; i++)
            {
                if (rnd[i] % 3 == 0)
                {
                    printf("%d ", rnd[i]);
                    d3 += rnd[i];
                }
            }
            printf("--> %d\n", d3);
            break;
        case 'f':   // 5의 배수 출력후 합구하기
            for (i = 0; i < count - 1; i++)
            {
                if (rnd[i] % 5 == 0)
                {
                    printf("%d ", rnd[i]);
                    d5 += rnd[i];
                }
            }
            printf("--> %d\n", d5);
            break;
        case 'm':   // 3 또는 5 의 배수 출력후 1을 더한 값 출력하기
            for (i = 0; i < count; i++)
            {
                if (rnd[i] % 3 == 0 || rnd[i] % 5 == 0)
                {
                    printf("%d ", rnd[i]);
                    nc++;
                    rnd1[nc - 1] = rnd[i] + 1;
                }

            }
            printf("--> ");
            for (i = 0; i < nc; i++)
            {
                printf("%d ", rnd1[i]);
            }
            break;
        case 'n':   //15의 배수 구한 뒤 1을 빼고 출력하기
            for (i = 0; i < count; i++)
            {
                if (rnd[i] % 3 == 0 && rnd[i] % 5 == 0)
                {
                    printf("%d ", rnd[i]);
                    nc++;
                    rnd2[nc - 1] = rnd[i] - 1;
                }

            }
            printf("--> ");
            for (i = 0; i < nc; i++)
            {
                printf("%d ", rnd2[i]);
            }
            break;
        case 'r':   // 50개의 난수 랜덤하게 재배열하기
            for (i = 0; i < count; i++) {

                do {
                    num2 = (rand() % count);
                    for (j = 0; j < i; j++) {
                        if (num1[j] == num2) break;
                    }
                } while (j != i);
                num1[i] = num2;
            }
        for (i = 0; i < count; i++)
        {
            rnd1[i] = rnd[num1[i]];
            printf("%d ",rnd1[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }
            break;
        case 'q':   // 프로그램 종료
            printf("프로그램 종료");
            break;
        case 'p':
            printf("Input number : ");
            scanf("%d", &op2);
            for (i = 0; i < count; i++) {

                do {
                    
                    num3 = rand() % + op2+1;
                    for (j = 0; j < i; j++) {
                        if (rnd3[j] == num3) break;
                    }
                } while (j != i);
                
                rnd3[i] = num3;
            }
            printf("Generted Numbers :\n");
            for (i = 0; i < count; i++)
            {
                printf("%d ", rnd3[i]);
                if ((i + 1) % 10 == 0) printf("\n");
            }
            
            break;
        default:
            break;
        }
    } while (command != 'q');
    
    return 0;   
} 