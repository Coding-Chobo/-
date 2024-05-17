#include <stdio.h>
#include <stdlib.h>
#define planetcount 8

typedef struct
{
	char name[10];			//�༺ �̸�
	double size;			// �༺ ������
	int satellite;			//������ ����
	int revolution;			//���� �ֱ�
	double rotating;		//���� �ֱ�
	double inclination;		//����
	double distance;		//�¾����κ����� �Ÿ�
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
		printf("��ɾ� : ");
		scanf(" %c", &command);
		switch (command)
		{
		case'p':
			printf("�� �༺�� ����");
			for (i = 0; i < planetcount; i++)
			{
				printf("\n%s �༺�� ����\n�༺�� ũ�� : %.2f\n������ ���� : %d\n������ �ֱ� : %d\n������ �ֱ� : %.2f\n���� : %.2f\n�¾����κ����� �Ÿ� : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'a': // ������ �ֱ� �������� ���
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
				printf("\n%s �༺�� ����\n�༺�� ũ�� : %.2f\n������ ���� : %d\n������ �ֱ� : %d\n������ �ֱ� : %.2f\n���� : %.2f\n�¾����κ����� �Ÿ� : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'b':	//�༺�� ũ�⿡ ���� ������������ ���
			for (i = 0; i < planetcount - 1; i++) {
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].size < planet[j + 1].size) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			printf("�༺�� ũ�⿡ ���� ������������ ���\n");
			for (i = 0; i < planetcount; i++) {
				printf("\n%s �༺�� ����\n�༺�� ũ�� : %.2f\n������ ���� : %d\n������ �ֱ� : %d\n������ �ֱ� : %.2f\n���� : %.2f\n�¾����κ����� �Ÿ� : %.2f\n",
					planet[i].name, planet[i].size, planet[i].satellite, planet[i].revolution, planet[i].rotating, planet[i].inclination, planet[i].distance);
			}
			break;
		case'c':	// �� �༺���� �Ÿ��� ���Ͽ� ������������ ���
			for (i = 0; i < planetcount - 1; i++) { // �¾����κ����� �Ÿ������� �������� ����
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].distance > planet[j + 1].distance) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			for (i = 0; i < planetcount; i++) // diff�迭�� �Ÿ��� ����
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
			for (i = 0; i < planetcount - 1; i++) {			//�Ÿ����� ����ü �迭 ���ÿ� �������� ����
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
					printf("%s�� ���� �༺ ������ �Ÿ� ����: %.2f\n", planet[i].name, diff[i]);
				}
				else { // Mercury�� ���
					printf("�¾�� %s�� �Ÿ�: %.2f\n", planet[i].name, diff[i]);
				}
			}
			break;
		case'd':
			for (i = 0; i < planetcount; i++)
			{
				printf("%s �� ���� �ֱ� : %f �ð�\n",planet[i].name,planet[i].rotating * 24);
			}
			break;
		case'e':	//���⸦ ������������ ���
			for (i = 0; i < planetcount - 1; i++) {
				for (j = 0; j < planetcount - i - 1; j++) {
					if (planet[j].inclination > planet[j + 1].inclination) {
						Planet temp = planet[j];
						planet[j] = planet[j + 1];
						planet[j + 1] = temp;
					}
				}
			}
			printf("���������� ����\n");
			for (i = 0; i < planetcount; i++)
			{
				printf("%s �� ���� %.2f\n",planet[i].name,planet[i].inclination);
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