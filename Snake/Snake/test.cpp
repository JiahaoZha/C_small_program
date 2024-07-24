#include <easyx.h>
#include <deque>

#define WIDTH 640
#define HEIGHT 480

#define SIZE 20 
#define DELAY 200

//direction of snake
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//snake
struct Snake
{
	int x;
	int y;
	Direction dir;//direction
};

//food
struct Food
{
	int x;
	int y;
};
// draw snake
void drawsnake(std::deque<Snake>& snake)
{
	setfillcolor(GREEN);
	for (const auto& body : snake)
	{
		fillrectangle(body.x *SIZE, body.y * SIZE, (body.x+1) * SIZE, (body.y + 1) * SIZE);

	}
	Sleep(DELAY);
}

void isOver(std::deque<Snake>& snake)
{
	for (auto iter = snake.begin() + 1; iter != snake.end(); ++iter)
	{
		if ((snake.front().x == iter->x && snake.front().y == iter->y) // Crash, game over.
			|| (snake.front().x < 0) || (snake.front().x >= WIDTH / SIZE)// Cross the line, game over
			|| (snake.front().y < 0) || (snake.front().y >= HEIGHT / SIZE)
			)
		{
			//Font setting
			settextcolor(RED);
			settextstyle(100, 0, _T("Consolas"));
			TCHAR endStr[16];
			_stprintf_s(endStr, _T("GAME OVER"));
			//position of text
			outtextxy(WIDTH / 6, HEIGHT / 4, endStr);
			Sleep(3000);
			closegraph();
			exit(0);
		}
	}
}
//snake moving
void movesnake(std::deque<Snake>& snake, bool grow = false)
{
	//get head
	Snake newHead = snake.front();
	//get direction
	switch (newHead.dir)
	{
	case UP:
		newHead.y--;
		break;
	case DOWN:
		newHead.y++;
		break;
	case LEFT:
		newHead.x--;
		break;
	case RIGHT:
		newHead.x++;
		break;
	}
	snake.push_front(newHead);
	isOver(snake);
	if (!grow)
	{
		clearrectangle(snake.back().x *SIZE, snake.back().y * SIZE, (snake.back().x+1) * SIZE, (snake.back().y + 1) * SIZE);
		snake.pop_back();
	}
	drawsnake(snake);
}

// change direction
void changedirection(std::deque<Snake>& snake)
{
	ExMessage msg = { 0 };
	peekmessage(&msg, EX_KEY);
	if (msg.message == WM_KEYDOWN)
	{
		switch (msg.vkcode)
		{
		case 'w':
		case VK_UP:
		case 'W'://Cannot move in the reverse direction
			if (snake.front().dir != DOWN)
				snake.front().dir = UP;
			break;

		case 's':
		case VK_DOWN:
		case 'S'://Cannot move in the reverse direction
			if (snake.front().dir != UP)
				snake.front().dir = DOWN;
			break;


		case 'a':
		case VK_LEFT:
		case 'A'://Cannot move in the reverse direction
			if (snake.front().dir != RIGHT)
				snake.front().dir = LEFT;
			break;

		case 'd':
		case VK_RIGHT:
		case 'D'://Cannot move in the reverse direction
			if (snake.front().dir != LEFT)
				snake.front().dir = RIGHT;
			break;
		}
	}
}

void generatefood_position(Food & food, std::deque<Snake>& snake)
{
	while (true)
	{
		food.x = rand() % (WIDTH / SIZE);
		food.y = rand() % (HEIGHT / SIZE);
		bool overlap = false;
		for (const auto& body : snake)
		{
			if (food.x == body.x && food.y == body.y)
			{
				overlap = true;
				break;
			}
		}
		if (!overlap)
		{
			break;
		}
	}
}

bool checkCollision(std::deque<Snake>& snake, Food& food)
{
	if (snake.front().x == food.x && snake.front().y == food.y)
	{
		return true;

	}
	for (auto iter = snake.begin() + 1; iter != snake.end(); ++iter)
	{
		if (iter->x == food.x && iter->y == food.y)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	initgraph(WIDTH, HEIGHT); // draw window
	std::deque<Snake>snake;
	snake.push_front({WIDTH / SIZE / 2 ,HEIGHT / SIZE / 2 , RIGHT });
	Food food;
	generatefood_position(food, snake);
	setfillcolor(RED);
	fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);
	drawsnake(snake);
	int score = 0;
	while (true) 
	{
		movesnake(snake);
		changedirection(snake);

		if (checkCollision(snake, food))
		{
			score++;
			generatefood_position(food, snake);
			movesnake(snake, true);
		}
		setfillcolor(RED);
		fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);
		settextcolor(YELLOW);
		settextstyle(20, 0, _T("Consolas"));
		TCHAR strScore[16];
		_stprintf_s(strScore, _T("Score: %d"), score);
		outtextxy(10, 10, strScore);

	}
	system("pause");
	closegraph();//closing
	return 0;
}