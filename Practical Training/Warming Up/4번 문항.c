#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
typedef struct
{
	int x;
	int y;
	char stoneshape;
}STONE;
char RandomShapes[5] = { '+', '&', '#', '@', '$' };

void placestone(char arr[10][10],STONE *stone1,STONE *stone2) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			arr[i][j] = '*';
		}
	}
	arr[stone1->x][stone1->y] = stone1->stoneshape;
	arr[stone2->x][stone2->y] = stone2->stoneshape;
}
void printmap(char arr[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%c ", arr[i][j]);
		}
		printf("\n");
	}
}
void stonemove(STONE *stone1,STONE *stone2, char direction, char arr[10][10]) {
	if (stone1->x == stone2->x && stone1->y == stone2->y)
	{
		stone1->stoneshape = RandomShapes[rand() % 5];
	}else
	arr[stone1->x][stone1->y] = '*';
	switch (direction) {
	case 'w': case'i':
		if (stone1->x > 0)
		{ stone1->x -= 1; }
		else { 
			stone1->x = 9;
			stone1->stoneshape = RandomShapes[rand() % 5];
		}
		break;
	case 's': case'k':
		if (stone1->x < 9){ 
			stone1->x += 1;
		}else { 
			stone1->x = 0;
			stone1->stoneshape = RandomShapes[rand() % 5];
		}
		break;
	case 'a': case'j':
		if (stone1->y > 0){
			stone1->y -= 1; }
		else { 
			stone1->y = 9;
			stone1->stoneshape = RandomShapes[rand() % 5];
		}
		break;
	case 'd': case 'l':
		if (stone1->y < 9) stone1->y += 1;
		else { 
			stone1->y = 0;
			stone1->stoneshape = RandomShapes[rand() % 5];
		}
		break;

	}
	arr[stone2->x][stone2->y] = stone2->stoneshape;
	arr[stone1->x][stone1->y] = stone1->stoneshape;
	if (stone1->x == stone2->x && stone1->y == stone2->y)
	{
		Beep(750, 100);
		arr[stone1->x][stone1->y] = stone1->stoneshape;
	}
	
}

int main() {
	srand(time(NULL));
	char map[10][10];
	char command = 0;
	bool turn = true; // true는 첫 번째 플레이어의 차례, false는 두 번째 플레이어의 차례
	STONE stone1 = { rand() % 10,rand() % 10,'x'};
	STONE stone2 = { rand() % 10,rand() % 10,'o'};
	placestone(map,&stone1,&stone2);
	printmap(map);
	printf("\n");
	do
	{
		printf("Input Command : ");
		scanf(" %c", &command);

		if (turn) {
			if (command == 'w' || command == 'a' || command == 's' || command == 'd') {
				stonemove(&stone1, &stone2, command, map);
				turn = false;
				printmap(map);
			}
		}
		else {
			if (command == 'i' || command == 'j' || command == 'k' || command == 'l') {
				stonemove(&stone2, &stone1, command, map);
				turn = true;
				printmap(map);
			}
		}
		if (command == 'r') {
			placestone(map, &stone1, &stone2);
			printmap(map);
			turn = true;
		}
		printf("\n");
	} while (command != 'q');
	return 0;
}