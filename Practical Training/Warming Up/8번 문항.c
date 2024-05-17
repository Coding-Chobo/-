#include <stdio.h>
#include <stdlib.h>
#include <time.h >
#define mapsize 11
typedef struct {
	int x;
	int y;
	char shape;
}Player;
void make_map(char arr[mapsize][mapsize]) {
	for (int i = 0; i < mapsize; i++) {
		for (int j = 0; j < mapsize; j++) {
			arr[i][j] = ' ';
		}
	}
	for (int i = 0; i < mapsize; i++) {
		for (int j = 0; j < mapsize; j++) {
			if ((i == 0 || i == 10) && j % 2 == 0) {
				arr[i][j] = '*';
			}
			else if ((j == 0 || j == 10) && i % 2 == 0) {
				arr[i][j] = '*';
			}
			else if (i == j && i % 2 == 1) {
				arr[i][j] = '*';
			}
			else if ((i == (mapsize - j - 1)) && i % 2 == 1) {
				arr[i][j] = '*';
			}
		}
	}
}
void printmap(char map[mapsize][mapsize], Player* p1, Player* p2) {

	map[p1->x][p1->y] = p1->shape;
	map[p2->x][p2->y] = p2->shape;
	for (int i = 0; i < mapsize; i++) {
		for (int j = 0; j < mapsize; j++) {
			printf("%c ", map[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n\n");
}
void throwyut(int* totalResult) {
	int result = 0;
	*totalResult = 0; 

	do {
		result = rand() % 5 + 1; 
		*totalResult += result; 

		// ��� ���
		switch (result) {
		case 1:
			printf("\nO O O @ // ��\n-> ���� 1 ĭ �����Դϴ�.\n");
			break;
		case 2:
			printf("\nO O @ @ // ��\n-> ���� 2 ĭ �����Դϴ�.\n");
			break;
		case 3:
			printf("\nO @ @ @ // ��\n-> ���� 3 ĭ �����Դϴ�.\n");
			break;
		case 4:
			printf("\n@ @ @ @ // ��\n-> ���� 4 ĭ �����Դϴ�, �ٽ� �����ϴ�.\n");
			break;
		case 5:
			printf("\nO O O O // ��\n-> ���� 5 ĭ �����Դϴ�, �ٽ� �����ϴ�.\n");
			break;
		}

	} while (result == 4 || result == 5); // ���̳� �� ������ �ݺ�
}
void move(char map[mapsize][mapsize], int command, int* rndresult, Player* p1, Player* p2) {
	char direction = 0;
	for (int repeat = 0; repeat < *rndresult; repeat++)
	{
		map[p1->x][p1->y] = '*';
		printf("\nw : ���� �̵�\na : �������� �̵�\ns : �Ʒ��� �̵�\nd : ���������� �̵�\ni : ���� ���� �̵�\nk : ������ ���� �̵�\nj : ���� ���� �̵�\nl : ������ ���� �̵�\nf : ������ ���������� �̵�\n������ ������ �Է��Ͻÿ� : ");
		scanf(" %c", &direction);
		switch (direction) {
		case'w': //���� �̵�
			if (p1->y == 10 && p1->x % 2 == 0 && p1->x != 0) {
				p1->x -= 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'a': // ���� �� �̵�
			if (p1->x == 0 && p1->y % 2 == 0 && p1->y != 0) {
				p1->y -= 2;
			}
			else
			{
				repeat--;
			}
			break;
		case 's': // �Ʒ��� �̵�
			if (p1->y == 0 && p1->x % 2 == 0 && p1->x != 10) {
				p1->x += 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'd': // ���������� �̵�
			if (p1->x == 10 && p1->y % 2 == 0 && p1->y != 10)
			{
				p1->y += 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'j': // ���� �Ʒ� �밢������ ������
			if ((p1->x + p1->y == mapsize - 1) && (p1->x % 2 == 1) && (p1->x != 9)) {
				p1->x += 2;
				p1->y -= 2;
			}
			else if ((p1->x == 0 && p1->y == 10) || (p1->x == 9 && p1->y == 1))
			{
				p1->x += 1;
				p1->y -= 1;
			}
			else
			{
				repeat--;
			}

			break;
		case'l': // ������ �Ʒ� �밢������ ������
			if ((p1->x == p1->y) && (p1->x % 2 == 1) && (p1->x != 9)) {
				p1->x += 2;
				p1->y += 2;
			}
			else if ((p1->x == 0 && p1->y == 0) || (p1->x == 9 && p1->y == 9))
			{
				p1->x += 1;
				p1->y += 1;
			}
			else
			{
				repeat--;
			}
			break;
		case'k': // ������ ���� �밢������ ������
			if ((p1->x == p1->y) && (p1->x % 2 == 1) && (p1->x != 1)) {
				p1->x -= 2;
				p1->y -= 2;
				repeat--;
			}
			else if ((p1->x == 10 && p1->y == 10) || (p1->x == 1 && p1->y == 1))
			{
				p1->x += 1;
				p1->y += 1;
				repeat--;
			}
			else
			{
				repeat--;
			}
			break;
		case'i': // ���� �� �밢������ ������
			if ((p1->x + p1->y == mapsize - 1) && (p1->x % 2 == 1) && (p1->x != 9)) {
				p1->x += 2;
				p1->y -= 2;
				repeat--;
			}
			else if ((p1->x == 10 && p1->y == 0) || (p1->x == 1 && p1->y == 9))
			{
				p1->x += 1;
				p1->y -= 1;
			}
			else
			{
				repeat--;
			}

			break;
		case 'f': if (direction == 'f')
		{
			p1->x = 9;
			p1->y = 5;
			printf("���� ����\n\n\n");
			repeat = *rndresult;
		}
				break;
		default:
			break;
		}

		printmap(map, p1, p2);
	}

}
int main() {
	srand(time(NULL));
	char command = 0;
	int rndresult = 0;
	char map[mapsize][mapsize] = { 0 };
	// �÷��̾�� �ʱ���ǥ �Է�
	Player p1 = { 10,10,'O' };
	Player p2 = { 10,10 ,'X' };
	make_map(map);



	do
	{
		printf("Command List\n'p' : ���� �÷���\n'1' : 1���÷��̾� ��\n'2' : 2���÷��̾� ��\n��ɾ �Է��Ͻÿ� : ");
		scanf(" %c", &command);
		switch (command)
		{
		case'p':
			printf("\n������ �����մϴ�.\n\n");
			make_map(map);
			printmap(map, &p1, &p2);
			break;
		case'1': // player 1 ����
			printf("1�� �÷��̾ ������ �����Դϴ�.");
			throwyut(&rndresult);
			move(map, command, &rndresult, &p1, &p2);

			if ((p1.x == p2.x) && (p1.y == p2.y))
			{

				p2.x = 10;
				p2.y = 10;
				printmap(map, &p1, &p2);
				printf("������ ���� ��ҽ��ϴ� �ٽ� �ѹ� �����Դϴ�.\n\n");
				throwyut(&rndresult);
				move(map, command, &rndresult, &p1, &p2);
			}

			break;
		case'2': // player 2 ����
			printf("2�� �÷��̾ ������ �����Դϴ�.");
			throwyut(&rndresult);
			if (rndresult == 4 || rndresult == 5)
			{
				throwyut(&rndresult);
			}
			move(map, command, &rndresult, &p2, &p1);

			if ((p1.x == p2.x) && (p1.y == p2.y))
			{

				p1.x = 10;
				p1.y = 10;
				printmap(map, &p1, &p2);
				printf("������ ���� ��ҽ��ϴ� �ٽ� �ѹ� �����Դϴ�.\n\n");
				throwyut(&rndresult);
				move(map, command, &rndresult, &p2, &p1);
			}

			break;
		default:
			break;
		}
	} while (!(p1.x == 9 && p1.y == 5) && !(p2.x == 9 && p2.y == 5));

	return 0;
}