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

		// 결과 출력
		switch (result) {
		case 1:
			printf("\nO O O @ // 도\n-> 말을 1 칸 움직입니다.\n");
			break;
		case 2:
			printf("\nO O @ @ // 개\n-> 말을 2 칸 움직입니다.\n");
			break;
		case 3:
			printf("\nO @ @ @ // 걸\n-> 말을 3 칸 움직입니다.\n");
			break;
		case 4:
			printf("\n@ @ @ @ // 윷\n-> 말을 4 칸 움직입니다, 다시 던집니다.\n");
			break;
		case 5:
			printf("\nO O O O // 모\n-> 말을 5 칸 움직입니다, 다시 던집니다.\n");
			break;
		}

	} while (result == 4 || result == 5); // 윷이나 모가 나오면 반복
}
void move(char map[mapsize][mapsize], int command, int* rndresult, Player* p1, Player* p2) {
	char direction = 0;
	for (int repeat = 0; repeat < *rndresult; repeat++)
	{
		map[p1->x][p1->y] = '*';
		printf("\nw : 위로 이동\na : 왼쪽으로 이동\ns : 아래로 이동\nd : 오른쪽으로 이동\ni : 왼쪽 위로 이동\nk : 오른쪽 위로 이동\nj : 왼쪽 위로 이동\nl : 오른쪽 위로 이동\nf : 마지막 지점에서의 이동\n움직일 방향을 입력하시오 : ");
		scanf(" %c", &direction);
		switch (direction) {
		case'w': //위로 이동
			if (p1->y == 10 && p1->x % 2 == 0 && p1->x != 0) {
				p1->x -= 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'a': // 왼쪽 로 이동
			if (p1->x == 0 && p1->y % 2 == 0 && p1->y != 0) {
				p1->y -= 2;
			}
			else
			{
				repeat--;
			}
			break;
		case 's': // 아래로 이동
			if (p1->y == 0 && p1->x % 2 == 0 && p1->x != 10) {
				p1->x += 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'd': // 오른쪽으로 이동
			if (p1->x == 10 && p1->y % 2 == 0 && p1->y != 10)
			{
				p1->y += 2;
			}
			else
			{
				repeat--;
			}
			break;
		case'j': // 왼쪽 아래 대각선으로 움직임
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
		case'l': // 오른쪽 아래 대각선으로 움직임
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
		case'k': // 오른쪽 위로 대각선으로 움직임
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
		case'i': // 왼쪽 위 대각선으로 움직임
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
			printf("게임 종료\n\n\n");
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
	// 플레이어들 초기좌표 입력
	Player p1 = { 10,10,'O' };
	Player p2 = { 10,10 ,'X' };
	make_map(map);



	do
	{
		printf("Command List\n'p' : 게임 플레이\n'1' : 1번플레이어 턴\n'2' : 2번플레이어 턴\n명령어를 입력하시오 : ");
		scanf(" %c", &command);
		switch (command)
		{
		case'p':
			printf("\n게임을 시작합니다.\n\n");
			make_map(map);
			printmap(map, &p1, &p2);
			break;
		case'1': // player 1 차례
			printf("1번 플레이어가 움직일 차례입니다.");
			throwyut(&rndresult);
			move(map, command, &rndresult, &p1, &p2);

			if ((p1.x == p2.x) && (p1.y == p2.y))
			{

				p2.x = 10;
				p2.y = 10;
				printmap(map, &p1, &p2);
				printf("상대방의 말을 잡았습니다 다시 한번 움직입니다.\n\n");
				throwyut(&rndresult);
				move(map, command, &rndresult, &p1, &p2);
			}

			break;
		case'2': // player 2 차례
			printf("2번 플레이어가 움직일 차례입니다.");
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
				printf("상대방의 말을 잡았습니다 다시 한번 움직입니다.\n\n");
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