#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FIELD_WIDTH 800
#define FIELD_HEIGHT 1200
#define ENEMY_NUM 10

struct enemy {
	int x, y;
	int num;
	bool isDestroyed;
} 

int main(void) {
	struct enemy* enemies;
	init(enemies);
	int i = 0;
	for (i=0; i<ENEMY_NUM-1; i++) {
		printf("enemy[%s]: point(%d,%d) and %s",
									 	i, enemies[i].x, enemies[i].y, enemies[i].isDestroyed ? "destroyed" : "alive");
	}
	return 0;
}

int init(struct enemy* enemies) {
	int enemy_width = FIELD_WIDTH / ENEMY_NUM;
	int enemy_height = FIELD_WIDTH / ENEMY_NUM;
	
	int i;
	for (i=1; i<ENEMY_NUM; i++)	{
		enemies[i-1] = {enemy_width*(i-0.5), FIELD_HEIGHT-enemy_height/2, i, false};
	}		
	return 0;
}
