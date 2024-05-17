#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define class 13
#define Voter 1000
#define Votes 109
// �ʿ��� ����ü �� �Լ� ����
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
    while (n < 150 || n >250)   //150�� 250������ �ĺ��� �� �Է¹ޱ�
    {
        printf("���ǿ� �ĺ��� ���� �Է� : ");
        scanf("%d", &n);
        while (getchar() != '\n');
    }
    printf("��� :");
    vote = (int*)calloc(n, sizeof(int));
    blmnum = (int*)calloc(n, sizeof(int));
    candi = (int*)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        Elected[i] = false;
    }
   
    for (i = 0; i < n; i++) // �� ��ȣ�� �ݹ���
    {
        blmnum[i] = rand() % class + 1;
        candi[i] = i + 1;
    }

    for (i = 0; i < n; i++)
    {
        printf("%d�� - %d�� ", candi[i], blmnum[i]);
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    //1000���� ���� 109���� ��ǥ
    
    
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
        printf("\n��ɾ� : ");
        scanf(" %c",&command);

        switch (command) {
        case 'p':   // ���ǿ� �� �� �ݿ� ����
            for (i = 0; i < class; i++)
            {    
                printf("%d�� - %d��, ", (i + 1), classblm[i]);
            }
            break;
        case 'a':   // �ݺ��� �ĺ��ڵ��� ��ǥ���� �������� ���� �� ���
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
            for (t = 0; t < class; t++) // �ݺ��� �ĺ��ڿ� �ĺ����� ��ǥ�� ���
            {
                printf("\n%d�� : ", t + 1);
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1))
                    {
                        printf("%d�� - %dǥ, ", candi[i], vote[i]);
                    }
                }
            }
            break;
        case 'd':   // �������� ���� �� ���
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
            for (t = 0; t < class; t++) // �ݺ��� �ĺ��ڿ� �ĺ����� ��ǥ�� ���
            {
                printf("\n%d�� : ", t + 1);
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1))
                    {
                        printf("%d�� - %dǥ, ", candi[i], vote[i]);
                    }
                }
            }
            break; 
        case 'm':   // �� �� �缱 ���ǿ� ��ȣ ���
            
            // ��ǥ ��� �������� ����
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

            // �ݺ��� �ĺ��ڿ� �ĺ����� ��ǥ�� ���
            for (t = 0; t < class; t++) 
            { 
                printf("\n%d�� : ", t + 1);// ���⼭���� �ٽ��ؾ߉� , count2�� ��ü ����� ���ǿ��� , count �� �ݺ��� ����� ���ǿ���,  Elected[i]�� ����Ǹ� ��ȯ���� 0(true)��
                // �ݺ��� �ִ��ο� �ȳѰ� ��±����� ��  ���⼭ �ݺ��� �����ϴٸ� ������ �߰��� ���ִ� ���ǹ� �۾��� �ϴ�����
                for (i = 0; i < n; i++)
                {
                    if (blmnum[i] == (t + 1)&&count[t]<classblm[t]) // �� ��ȣ�� ��ġ && �ݺ��� ����� �ο��� ����������
                    {
                        printf("%d�� - %dǥ, ", candi[i], vote[i]);
                        count[t]++;                //�ݺ��� ����� �ο��� ī��Ʈ�ϱ�
                    }  
                }
            }

           



            break;
        case 'n':   // ��ü �ٵ�ǥ �� �缱 ���ǿ� ��ȣ ���
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (vote[i] < vote[j]) { // ��ǥ���� ���� �������� ����
                        
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
            printf("\n��ü �ٵ�ǥ������ ����� ���ǿ���:\n");
            for (i = 0; i < Votes; i++) {
                printf("%d�� - %dǥ %d��, ", candi[i], vote[i],blmnum[i]);
            }
            break;
        case'r':  // ���α׷� �����
            shuffle(classblm, class);

            while (n2 < 150 || n2 >250)   //150�� 250������ �ĺ��� �� �Է¹ޱ�
            {
                printf("���ǿ� �ĺ��� ���� �Է� : ");
                scanf("%d", &n2);
                while (getchar() != '\n');
            }
            printf("��� :");
            vote = (int*)calloc(n2, sizeof(int));
            blmnum = (int*)calloc(n2, sizeof(int));
            candi = (int*)calloc(n2, sizeof(int));

            for (i = 0; i < n2; i++) // �� ��ȣ�� �ݹ���
            {
                blmnum[i] = rand() % class + 1;
                candi[i] = i + 1;
            }

            for (i = 0; i < n2; i++)
            {
                printf("%d�� - %d�� ", candi[i], blmnum[i]);
                if ((i + 1) % 10 == 0)
                    printf("\n");
            }
            //1000���� ���� 109���� ��ǥ


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