/*
* how to play: player controls ball to eat food to grow up to eat other player
 
	goal:
		1.moving
		2.food generation
		3.eat food
		4.ball growing

*/

#include<easyx.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>


#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600
//define speed
#define SPEED 5
#define MAXFOOD MAPSIZE / 10

//define map size
#define MAPSIZE 5000

struct
{
	int s_pos_x;
	int s_pos_y;

}MapPosition;

// state
enum {
	ALIVE,DEAD
};


//define ball class
typedef struct Ball
{
	int radius;
	
	int pos_x;//position
	int pos_y;
	int color;
	int state;

};Ball;
//player
Ball player;
//food struction
Ball foods[MAXFOOD];


void playinitial()
{
	//initial player attribute
	player.radius = 10;
	player.pos_x = WINDOWWIDTH / 2 - 5;
	player.pos_y = WINDOWHEIGHT / 2 - 5;
	player.color = 0xdfdfdf;
	player.state = ALIVE;
}

void Mapinit()
{
	//initial map position
	MapPosition.s_pos_x = 0 - (MAPSIZE - WINDOWWIDTH) / 2;
	MapPosition.s_pos_y = 0 - (MAPSIZE - WINDOWHEIGHT) / 2;
}

void Foodinit()
{
	//initial food 
	for (int count = 0; count < MAXFOOD; count++)
	{
		//RGB
		foods[count].color = rand() % 255 << 16 | rand() % 255 << 8 | rand() % 255;
		//random position
		foods[count].pos_x = rand() % MAPSIZE + (MapPosition.s_pos_x);
		foods[count].pos_y = rand() % MAPSIZE + (MapPosition.s_pos_y);

		foods[count].radius = rand() % 5 + 1;
		foods[count].state = ALIVE;
	}

}
//initialization
void Init()
{
	//initial window
	initgraph(WINDOWWIDTH, WINDOWHEIGHT);
	//set random seed
	srand(time(0));

	playinitial();

	Mapinit();

	Foodinit();

	//Open Double buffering
	BeginBatchDraw();
}


void CreateFood()
{
	int count = 0; // records of enten food
	int nullfoods[MAXFOOD * MAXFOOD] = {0};//record the food which be eaten

	//get eaten food
	for (int size = 0; size < MAXFOOD; size++)
	{
		if (foods[size].state == DEAD)
		{ 
			nullfoods[count++] = size;
		}
	}
	// get a random eaten food as a new food
	int rand_pos = nullfoods[rand() % count];
	rand() % count;
	foods[rand_pos].pos_x = rand() % MAPSIZE + (MapPosition.s_pos_x);
	foods[rand_pos].pos_y = rand() % MAPSIZE + (MapPosition.s_pos_y);

	foods[rand_pos].radius = rand() % 5 + 1;
	foods[rand_pos].state = ALIVE;


}

void DrawMap()
{
	//set lines color
	setlinecolor(RGB(0x50, 0x51, 0x4F));

	for (int count = 0; count < MAPSIZE; count += 10)
	{
		//Horizontal line
		line(count + MapPosition.s_pos_x, MapPosition.s_pos_y, 
			 count + MapPosition.s_pos_x, MapPosition.s_pos_y + MAPSIZE);

		//Vertical Line
		line(MapPosition.s_pos_x, MapPosition.s_pos_y + count,
			 MapPosition.s_pos_x + MAPSIZE, MapPosition.s_pos_y + count);
	}
}

//control player moving
void PlayerMove()
{
	//peek_msg.
	
	/*ExMessage msg;
	if (peekmessage(&msg, EX_KEY))
	{
		switch (msg.vkcode)
		{
		case VK_UP:
			
			player.pos_y -= SPEED;
			// move up
			break;
		case VK_DOWN:
			player.pos_y += SPEED;
			//move down
			break;
		case VK_LEFT:
			player.pos_x -= SPEED;
			//move left
			break;
		case VK_RIGHT:
			player.pos_x += SPEED;
			//move right
			break;
		}
	}*/
	if (GetAsyncKeyState(VK_UP))
	{
		if (player.pos_y <= WINDOWHEIGHT/2 && MapPosition.s_pos_y < 0)
		{
			MapPosition.s_pos_y += SPEED;

			//food moving with map moving 
			for (int count = 0; count < MAXFOOD; count++)
				foods[count].pos_y += SPEED;
		}
		else
		{
			player.pos_y -= SPEED;

			//Bounds Checking 
			if (player.pos_y < player.radius)
				player.pos_y = player.radius;
		}

	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (player.pos_y >= WINDOWHEIGHT / 2 && MapPosition.s_pos_y + MAPSIZE > WINDOWHEIGHT)
		{ 
			MapPosition.s_pos_y -= SPEED;
			for (int count = 0; count < MAXFOOD; count++)
				foods[count].pos_y -= SPEED;
		}
		else
		{
			player.pos_y += SPEED;
			if (player.pos_y > WINDOWHEIGHT - player.radius)
				player.pos_y = WINDOWHEIGHT - player.radius;
		}
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		if (player.pos_x <= WINDOWWIDTH / 2 && MapPosition.s_pos_x < 0)
		{
			MapPosition.s_pos_x += SPEED;
			for (int count = 0; count < MAXFOOD; count++)
				foods[count].pos_x += SPEED;
		}
		else
		{
			player.pos_x -= SPEED;
			if (player.pos_x < player.radius)
				player.pos_x = player.radius;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (player.pos_x >= WINDOWWIDTH / 2 && MapPosition.s_pos_x + MAPSIZE > WINDOWWIDTH)
		{
			MapPosition.s_pos_x -= SPEED;
			for (int count = 0; count < MAXFOOD; count++)
				foods[count].pos_x -= SPEED;

		}
		else
		{
			player.pos_x += SPEED;
			if (player.pos_x > WINDOWWIDTH - player.radius)
				player.pos_x = WINDOWWIDTH - player.radius;
		}
	}
}

void Playupdate()
{
	//update player's position
	setfillcolor(player.color);//set color
	fillcircle(player.pos_x, player.pos_y, player.radius);
}

void Foodupdate()
{
	//refresh food
	for (int count = 0; count < MAXFOOD; count++)
	{
		if (foods[count].state == ALIVE && abs(player.pos_x - foods[count].pos_x) + abs(player.pos_y - foods[count].pos_y) < player.radius)
		{
			player.radius += 1;
			foods[count].state = DEAD;
			CreateFood();
			continue;
		}
		if (foods[count].state == ALIVE)
		{
			setfillcolor(foods[count].color);
			fillcircle(foods[count].pos_x, foods[count].pos_y, foods[count].radius);
		}

	}
}
void Update()
{
	//clear past figure
	cleardevice();
	//player operation
	PlayerMove();

	DrawMap();

	Foodupdate();

	Playupdate();

	//refresh buffering
	FlushBatchDraw();

	//delay for decreasing speed
	Sleep(1);
}
void Uninit()
{
	//close double buffering
	EndBatchDraw();
	//close window
	closegraph();
}

int main()
{
	//initialization
	Init();
	//keep updating
	while (1)
	{
		Update();
	};
	//uninitialization
	Uninit();
	return 0;
}