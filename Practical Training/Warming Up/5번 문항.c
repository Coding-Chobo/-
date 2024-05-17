#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define class 13
#define Voter 1000
#define Votes 109
// 필요한 구조체 및 함수 선언
void shuffle(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
int main() {
    int n = 0, n2 = 0, i, j, t, temp, count[class], count2 = 0;
    char command = 0;
    int classblm[class] = { 9 , 12 , 9 , 13 , 10 , 6 , 10 , 6 , 9 , 5 , 6 , 7 , 7 };
    int *blmnum,*candi,*vote;
    bool* Elected = (bool*)malloc(n * sizeof(bool));
    
    srand(time(NULL));

    shuffle(classblm, class);
    for (i = 0; i < class; i++)
    {
        count[i] = 0;
    }
    while (n < 150 || n >250)   //150과 250사이의 후보자 수 입력받기
    {
        printf("대의원 후보자 숫자 입력 : ");
        scanf("%d", &n);
        while (getchar() != '\n');
    }
    printf("출력 :");
    vote = (int*)calloc(n, sizeof(int));
    blmnum = (int*)calloc(n, sizeof(int));
    candi = (int*)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        Elected[i] = false;
    }
   
    for (i = 0; i < n; i++) // 각 번호당 반배정
    {
        blmnum[i] = rand() % class + 1;
        candi[i] = i + 1;
    }

    for (i = 0; i < n; i++)
    {
        printf("%d번 - %d반 ", candi[i], blmnum[i]);
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    //1000명이 각자 109명을 투표
    
    
    for (i = 0; i < n; i++) {
        vote[i] = 0;
    }
    for (int i = 0; i < Voter; i++) {
        for (int j = 0; j < Votes; j++) {
            int voting = rand() % n;
            vote[voting]++;
        }
    }
    while (command != 'q') {
        printf("\n명령어 : ");
        scanf(" %c",&command);

        switch (command) {
        case 'p':   // 대의원 수 각 반에 배정
            for (i = 0; i < class; i++)
            {    
                printf("%d반 - %d명, ", (i + 1), classblm[i]);
            }
            break;
        case 'a':   // 반별로 후보자들의 투표수를 오름차순 정렬 및 출력
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    if (vote[i] > vote[j]) {
                        
                        temp = vote[i];
                        vote[i] = vote[j];
                        vote[j] = temp;

                        
                        temp = candi[i];
                        candi[i] = candi[j];
                        candi[j] = temp;

                        
                        temp = blmnum[i];
                        blmnum[i] = blmnum[j];
                        blmnum[j] = temp;
                    }
                }  
            }
            for (t = 0; t < class; t++) // 반별로 후보자와 후보자의 투표수 출력
            {
                printf("\n%d반 : ", t + 1);
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1))
                    {
                        printf("%d번 - %d표, ", candi[i], vote[i]);
                    }
                }
            }
            break;
        case 'd':   // 내림차순 정렬 및 출력
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    if (vote[i] < vote[j]) {
                        
                        temp = vote[i];
                        vote[i] = vote[j];
                        vote[j] = temp;

                       
                        temp = candi[i];
                        candi[i] = candi[j];
                        candi[j] = temp;

                       
                        temp = blmnum[i];
                        blmnum[i] = blmnum[j];
                        blmnum[j] = temp;
                    }
                }
            }
            for (t = 0; t < class; t++) // 반별로 후보자와 후보자의 투표수 출력
            {
                printf("\n%d반 : ", t + 1);
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1))
                    {
                        printf("%d번 - %d표, ", candi[i], vote[i]);
                    }
                }
            }
            break; 
        case 'm':   // 반 별 당선 대의원 번호 출력
            
            // 투표 결과 내림차순 정렬
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    if (vote[i] < vote[j]) {
                        
                        temp = vote[i];
                        vote[i] = vote[j];
                        vote[j] = temp;

                        temp = candi[i];
                        candi[i] = candi[j];
                        candi[j] = temp;

                        temp = blmnum[i];
                        blmnum[i] = blmnum[j];
                        blmnum[j] = temp;
                    }
                }
            } 

            // 반별로 후보자와 후보자의 투표수 출력
            for (t = 0; t < class; t++) 
            { 
                printf("\n%d반 : ", t + 1);// 여기서부터 다시해야됌 , count2는 전체 선출된 대의원수 , count 는 반별로 선출된 대의원수,  Elected[i]는 선출되면 반환값이 0(true)임
                // 반별로 최대인원 안넘게 출력까지는 함  여기서 반별로 부족하다면 선출을 추가로 해주는 조건문 작업을 하는중임
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1)&&count[t]<classblm[t]) // 반 번호가 일치 && 반별로 선출된 인원이 차지않을때
                    {
                        printf("%d번 - %d표, ", candi[i], vote[i]);
                        count[t]++;                //반별로 선출된 인원수 카운트하기
                    }  
                }
            }

           



            break;
        case 'n':   // 전체 다득표 순 당선 대의원 번호 출력
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (vote[i] < vote[j]) { // 투표수에 따라 내림차순 정렬
                        
                        temp = vote[i];
                        vote[i] = vote[j];
                        vote[j] = temp;

                       
                        temp = candi[i];
                        candi[i] = candi[j];
                        candi[j] = temp;

                        temp = blmnum[i];
                        blmnum[i] = blmnum[j];
                        blmnum[j] = temp;
                    }
                }
            }             
            printf("\n전체 다득표순으로 선출된 대의원들:\n");
            for (i = 0; i < Votes; i++) {
                printf("%d번 - %d표 %d반, ", candi[i], vote[i],blmnum[i]);
            }
            break;
        case'r':  // 프로그램 재시작
            shuffle(classblm, class);

            while (n2 < 150 || n2 >250)   //150과 250사이의 후보자 수 입력받기
            {
                printf("대의원 후보자 숫자 입력 : ");
                scanf("%d", &n2);
                while (getchar() != '\n');
            }
            printf("출력 :");
            vote = (int*)calloc(n2, sizeof(int));
            blmnum = (int*)calloc(n2, sizeof(int));
            candi = (int*)calloc(n2, sizeof(int));

            for (i = 0; i < n2; i++) // 각 번호당 반배정
            {
                blmnum[i] = rand() % class + 1;
                candi[i] = i + 1;
            }

            for (i = 0; i < n2; i++)
            {
                printf("%d번 - %d반 ", candi[i], blmnum[i]);
                if ((i + 1) % 10 == 0)
                    printf("\n");
            }
            //1000명이 각자 109명을 투표


            for (i = 0; i < n2; i++) {
                vote[i] = 0;
            }
            for (int i = 0; i < Voter; i++) {
                for (int j = 0; j < Votes; j++) {
                    int voting = rand() % n2;
                    vote[voting]++;
                }
            }
            n = n2;
            break;
        case 'q':
            break;
        default:
            break;
        }
    }

    return 0;
}