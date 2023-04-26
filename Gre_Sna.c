#pragma warning (disable:4996)
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#define ROW 22//行
#define COL 42//列
#define WALL 1//墙
#define KONG 0//空
#define HEAD 77//蛇头
#define BODY 88//蛇身
#define FOOD 99//食物
#define UP 72 //上
#define DOWN 80 //下
#define LEFT 75 //左
#define RIGHT 77 //右
#define SPACE 32 //暂停
#define ESC 27 //退出
struct snake_head
{
	int len;
	int x;
	int y;
}snake;
struct snake_body
{
	int x;
	int y;
}body[ROW*COL];

int face[ROW][COL];

int max, grade;



//函数定义
void color(int c)//颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置
}
void CursorJump(int x ,int y)//光标跳转
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(handle, pos);
}
char GameBegin()//游戏初始界面
{
	printf("************************************************************************************************************************\n");
	//system("color 70");
	//printf("\t\t\t\t\t\tGREEDY_SNAKE\n");
	printf("\n");
	color(4);
	printf("\t\t\t\t ■■ ■  ■■■■■   ■■■■■■ ■■■■■■ ■■■■   ■■■ ■■■\n");
	printf("\t\t\t\t■  ■■   ■   ■   ■      ■      ■  ■   ■   ■ \n");
	printf("\t\t\t\t■   ■   ■   ■   ■      ■      ■   ■  ■   ■ \n");
	printf("\t\t\t\t■       ■   ■   ■      ■      ■   ■   ■ ■ \n");
	printf("\t\t\t\t■       ■■■■    ■■■■   ■■■■   ■   ■    ■ \n");
	printf("\t\t\t\t■  ■■■  ■ ■     ■      ■      ■   ■    ■\n");
	printf("\t\t\t\t■   ■   ■  ■    ■      ■      ■   ■    ■\n");
	printf("\t\t\t\t■   ■   ■   ■   ■      ■      ■  ■     ■   \n");
	printf("\t\t\t\t ■■■   ■■■  ■■ ■■■■■■ ■■■■■■ ■■■■     ■■■ \n");
	printf("\n");
	printf("\t\t\t\t    ■■■  ■■  ■■■    ■    ■■■  ■■ ■■■■■■\n");
	printf("\t\t\t\t   ■   ■  ■   ■     ■     ■   ■   ■\n");
	printf("\t\t\t\t   ■      ■■  ■    ■ ■    ■  ■    ■\n");
	printf("\t\t\t\t   ■      ■■  ■    ■ ■    ■  ■    ■\n");
	printf("\t\t\t\t     ■    ■ ■ ■   ■   ■   ■■■     ■■■■\n");
	printf("\t\t\t\t      ■   ■  ■■   ■■■■■   ■ ■     ■\n");
	printf("\t\t\t\t       ■  ■  ■■   ■   ■   ■  ■    ■\n");
	printf("\t\t\t\t   ■   ■  ■   ■   ■   ■   ■   ■   ■ \n");
	printf("\t\t\t\t    ■■■  ■■■  ■  ■■■ ■■■ ■■■  ■■ ■■■■■■\n");
	char a;
	printf("\n");
	printf("\n\n\n\n");
	color(7);
	printf("\t\t\t\t■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	
	printf("\t\t\t\t\t\t开始游戏(y/n):");
	scanf("%c", &a);
	return a;
}
void HideCursor()//光标隐藏
{
	CONSOLE_CURSOR_INFO cursor;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &cursor);
}
void ReadMaxGrade()//读取最高成绩到max
{
	FILE* fp = fopen("greedy_snake.txt", "r");
	if (fp == NULL)
	{
		fp = fopen("greedy_snake.txt", "w");
		fwrite(&max, sizeof(int), 1, fp);
	}
	fseek(fp, 0, SEEK_SET);
	fread(&max, sizeof(int), 1, fp);
	fclose(fp);
	fp = NULL;
}
void WriteGrade()//更新最高分
{
	FILE* fp = fopen("greedy_snake.txt", "w");
	if (fp == NULL)
	{
		printf("找不到指定文件");
		exit(-1);
	}
	fwrite(&grade, sizeof(int), 1, fp);
	fclose(fp);
	fp = NULL;
}
void InitInterface()
{
	color(4);//墙设置为红色
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL;j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL;
				CursorJump(2 * j, i);
				printf("■ ");
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL;
				printf("■ ");
			}
			else
			{
				face[i][j] = KONG;
			}
		}
	}
	color(7); //颜色设置为白色
	CursorJump(0, ROW);
	printf("当前得分:%d", grade);
	CursorJump(COL, ROW);
	printf("历史最高得分:%d", max);
}
void InitSnake()//初始化蛇
{
	snake.len = 2;
	snake.x = COL / 2;
	snake.y = ROW / 2;
	body[0].x = COL / 2 - 1;
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	face[snake.y][snake.x] = HEAD;
	face[body[0].y][body[0].x] = BODY;
	face[body[1].y][body[1].x] = BODY;
}
void RandFood()//随机函数
{
	int x, y;
	do
	{
		y = rand() % COL;
		x = rand() % ROW;
	} while (face[x][y] != KONG);
	face[x][y] = FOOD;
	CursorJump(2*y,x);//       *
	printf("●");
}
void DrawSnake(int flag) //打印蛇 蛇的移动覆盖
{
	if (flag == 1)
	{
		color(10);
		CursorJump(2*snake.x, snake.y);
		printf("●");
		for (int i = 0; i < snake.len; i++)
		{
			CursorJump(2*body[i].x, body[i].y);
			printf("□");
		}
	}
	else
	{
		if (body[snake.len - 1].x != KONG)
		{
			CursorJump(2 * body[snake.len - 1].x, body[snake.len - 1].y);
			printf(" ");
		}
	}
}
void JudgeFunc(int x, int y)
{
	//若蛇头即将到达的位置是食物，则得分
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //蛇身加长
		grade += 10; //更新当前得分
		color(7); //颜色设置为白色
		CursorJump(0, ROW);
		printf("当前得分:%d", grade); //重新打印当前得分
		RandFood(); //重新随机生成食物
	}
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //留给玩家反应时间
		system("cls"); //清空屏幕
		color(7); //颜色设置为白色
		CursorJump(2 * (COL / 3), ROW / 2 - 3);
		if (grade > max)
		{
			printf("恭喜你打破最高记录，最高记录更新为%d", grade);
			WriteGrade();
		}
		else if (grade == max)
		{
			printf("与最高记录持平，加油再创佳绩");
		}
		else
		{
			printf("请继续加油，当前与最高记录相差%d", max - grade);
		}
		CursorJump(2 * (COL / 3), ROW / 2);
		printf("GAME OVER");
		while (1) //询问玩家是否再来一局
		{
			char ch;
			CursorJump(2 * (COL / 3), ROW / 2 + 3);
			printf("再来一局?(y/n):");
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				system("cls");
				main();
			}
			else if (ch == 'n' || ch == 'N')
			{
				CursorJump(2 * (COL / 3), ROW / 2 + 5);
				exit(0);
			}
			else
			{
				CursorJump(2 * (COL / 3), ROW / 2 + 5);
				printf("选择错误，请再次选择");
			}
		}
	}
}
void MoveSnake(int x, int y)
{
	DrawSnake(0); //先覆盖当前所显示的蛇
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //蛇移动后蛇尾重新标记为空
	face[snake.y][snake.x] = BODY; //蛇移动后蛇头的位置变为蛇身
	//蛇移动后各个蛇身位置坐标需要更新
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//蛇移动后蛇头位置信息变为第0个蛇身的位置信息
	body[0].x = snake.x;
	body[0].y = snake.y;
	//蛇头的位置更改
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	DrawSnake(1); //打印移动后的蛇
}
void run(int x, int y)
{
	int t=0;
	while (1)
	{
		if(t==0)
		t = 10000;
		while (--t)
		{
			if (kbhit() != 0) //若键盘被敲击，则退出循环
				break;
		}
		if (t == 0)
		{
			JudgeFunc(x, y); //判断到达该位置后，是否得分与游戏结束
			MoveSnake(x, y); //移动蛇
		}
		else
		{
			break;
		}
	}
}
void Game()
{
	int n = RIGHT;
	int tmp = 0;
	goto first;
	while (1)
	{
		n = getch();
		//键值的调整
		switch (n)
		{
		case UP :
		case DOWN:
			if (tmp != LEFT && tmp != RIGHT)
			{
				n = tmp;
			}
			break;
		case RIGHT:
		case LEFT:
			if (tmp != UP && tmp != DOWN)
			{
				n = tmp;
			}
		case SPACE:
		case ESC:
		case 'r':
		case 'R':
			break;
		default:
			n = tmp;
			break;
		}
	first:
		switch (n)
		{
		case UP:
			run(0, -1);
			tmp = UP;
			break;
		case DOWN:
			run(0, 1);
			tmp = DOWN;
			break;
		case LEFT:
			run(-1,0);
			tmp = LEFT;
			break;
		case RIGHT:
			run(1, 0);
			tmp = RIGHT;
			break;
		case SPACE:
			system("pause>nul");
			break;
		case ESC:
			system("cls");
			color(7);
			CursorJump(COL - 8, ROW / 2);
			printf("  游戏结束  ");
			CursorJump(COL - 8, ROW / 2 + 2);
			exit(0);
		case 'r':
		case 'R':
			system("cls");
			main();
		}
	}

}

int main()
{
	max = 0, grade = 0;
	char a;
	system("title 贪吃蛇");
	a = GameBegin();
	color(7);
	system("cls");
	HideCursor();//光标隐藏
	ReadMaxGrade();//读取max
	InitInterface(); //初始化界面
	InitSnake(); //初始化蛇
	srand((unsigned int)time(NULL)); //设置随机数生成起点
	RandFood(); //随机生成食物
	DrawSnake(1); //打印蛇
	Game();
	return 0;

}

















