#include <stdio.h>
#include <stdlib.h>
#define planetcount 8

typedef struct
{
	char name[10];			//행성 이름
	double size;			// 행성 반지름
	int satellite;			//위성의 개수
	int revolution;			//공전 주기
	double rotating;		//자전 주기
	double inclination;		//기울기
	double distance;		//태양으로부터의 거리
}Planet;

int main() {
	int i = 0, j = 0, t = 0;
	char command = 0;
	double speed[planetcount];
	double diff[planetcount];
	Planet planet[] = {
		{"Mercury", 0.38 * 10,	0,	88,		58.65,	0.03,	0.39},
		{"Venus",	0.95 * 10,	0,	224,	243.02, 177.36, 0.72},
		{"Earth",	1.00 * 10,	1,	365,	1.00,	23.44,	1.00},
		{"Mars",	0.53 * 10,	2,	687,	1.03,	25.19,	1.52},
		{"Jupiter",	11.21 * 10, 79, 4331,	0.41,	3.13,	5.20},
		{"Saturn",	9.45 * 10,	82, 10747,	0.45,	26.73,	9.58},
		{"Uranus",	4.01 * 10,	27, 30589,	0.72,	97.77,	19.22},
		{"Neptune",	3.88 * 10,	14, 59800,	0.67,	28.32,	30.05}
	};
	do
	{
		printf("명령어 : ");
		scanf(" %c", &command);
		switch (command)
		{
		case'p':
			printf("각 행성의 정보");
			for (i = 0; i < planetcount; i++)
			{
				printf("\n%s 행성의 정보\n행성의 크기 : %.2f\n위성의 개수 : %d\n공전의 주기 : %d\n자전의 주기 : %.2f\n기울기 : %.2f\n태양으로부터의 거리 : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'a': // 공전의 주기 오름차순 출력
			for (i = 0; i < planetcount; i++)
			{
				speed[i] = planet[i].size / planet[i].revolution;
			}
			for (i = 0; i < planetcount - 1; i++) {
				for (j = 0; j < planetcount - i - 1; j++) {
					if (speed[j] > speed[j+1]) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			for (i = 0; i < planetcount; i++) {
				printf("\n%s 행성의 정보\n행성의 크기 : %.2f\n위성의 개수 : %d\n공전의 주기 : %d\n자전의 주기 : %.2f\n기울기 : %.2f\n태양으로부터의 거리 : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'b':	//행성의 크기에 따라 내림차순으로 출력
			for (i = 0; i < planetcount - 1; i++) {
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].size < planet[j + 1].size) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			printf("행성의 크기에 따라 오름차순으로 출력\n");
			for (i = 0; i < planetcount; i++) {
				printf("\n%s 행성의 정보\n행성의 크기 : %.2f\n위성의 개수 : %d\n공전의 주기 : %d\n자전의 주기 : %.2f\n기울기 : %.2f\n태양으로부터의 거리 : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'c':	// 두 행성간의 거리에 대하여 내림차순으로 출력
			for (i = 0; i < planetcount - 1; i++) { // 태양으로부터의 거리순으로 오름차순 정렬
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].distance > planet[j + 1].distance) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			for (i = 0; i < planetcount; i++) // diff배열에 거리값 저장
			{
				if (i == 0)
				{
					diff[i] = planet[i].distance;
				}
				else
				{
					diff[i] = planet[i].distance - planet[i-1].distance;
				}
			}
			for (i = 0; i < planetcount - 1; i++) {			//거리값과 구조체 배열 동시에 내림차순 정렬
				for (j = 0; j < planetcount - i - 1; j++) {
					if (diff[j] < diff[j+1]) {
						double temp = diff[j];
						diff[j] = diff[j + 1];
						diff[j + 1] = temp;
						
						Planet temp2 = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp2;
					}
				}
			}
			
			for (i = 0; i < planetcount; i++)
			{
				
				if (strcmp(planet[i].name, "Mercury") != 0) { 
					printf("%s와 이전 행성 사이의 거리 차이: %.2f\n", planet[i].name, diff[i]);
				}
				else { // Mercury의 경우
					printf("태양과 %s의 거리: %.2f\n", planet[i].name, diff[i]);
				}
			}
			break;
		case'd':
			for (i = 0; i < planetcount; i++)
			{
				printf("%s 의 자전 주기 : %f 시간\n",planet[i].name,planet[i].rotating * 24);
			}
			break;
		case'e':	//기울기를 오름차순으로 출력
			for (i = 0; i < planetcount - 1; i++) {
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].inclination > planet[j + 1].inclination) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			printf("오름차순의 기울기\n");
			for (i = 0; i < planetcount; i++)
			{
				printf("%s 의 기울기 %.2f\n",planet[i].name,planet[i].inclination);
			}
			break;
		case'q':
			break;
		default:
			break;
		}
		printf("\n");
	} while (command != 'q');
	return 0;
}