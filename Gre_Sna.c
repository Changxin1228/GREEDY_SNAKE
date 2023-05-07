#pragma warning (disable:4996)//禁用掉一些警告
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
//定义所有游戏中的名词转会为数值
#define ROW 22//行
#define COL 42//列
#define WALL 1//墙
#define KONG 0//空白
#define HEAD 77//蛇头
#define BODY 88//蛇身
#define FOOD 99//食物
#define UP 72 //上
#define DOWN 80 //下
#define LEFT 75 //左
#define RIGHT 77 //右
#define SPACE 32 //暂停
#define ESC 27 //退出
struct snake_head//定义蛇的头部和身长
{
	int len;//身长
	//蛇在游戏空空间中 头的坐标
	int x;
	int y;
}snake;
struct snake_body//定义
{
	//某一蛇身坐标 x，y
	int x;
	int y;
}body[ROW*COL];//定一个一足够记录所有蛇身体的坐标二维数组

int face[ROW][COL];//记录下游戏界面所有位置的信息

int max, grade;//最高分，和高得分记录



//函数定义
void ColorForInterface(int c)//设置当前界面颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置，获取控制台句柄
}
void CursorJump(int x ,int y)//光标跳转
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(handle, pos);
	//SetConsoleCursorPosition是光标跳转函数通过获取控制台句柄以及结构体COORD中的x，y坐标确定光标位置
}
char GameBegin()//游戏初始界面
{
	printf("************************************************************************************************************************\n");
	printf("\n");
	ColorForInterface(4);//红色的greedy snake
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
	ColorForInterface(7);
	printf("\t\t\t\t■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("\t\t\t\t\t\t开始游戏(y/n):");
	scanf("%c", &a);
	getchar();//吃掉回车
	if (a=='n'||a=='N')
	{
		system("cls");
		printf("\t\t\t欢迎下次再来");
		exit(-1);
	}
	return a;
}
void HideCursor()//光标隐藏
{
	CONSOLE_CURSOR_INFO cursor;//定义光标结构体CONSOLE_CURSOR_INFO
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &cursor);//光标隐藏函数，同样需要获取句柄
}
void ReadMaxGrade()//读取最高成绩到max
{
	FILE* fp = fopen("greedy_snake.txt", "r");
	if (fp == NULL)
	{
		fp = fopen("greedy_snake.txt", "w");
		fwrite(&max, sizeof(int), 1, fp);
	}
	//打开或者创建文件
	fseek(fp, 0, SEEK_SET);//光标定位到文件的开头
	fread(&max, sizeof(int), 1, fp);//读取一个max
	fclose(fp);
	fp = NULL;
}
void UpDateGread()//更新最高分
{
	FILE* fp = fopen("greedy_snake.txt", "w");
	if (fp == NULL)
	{
		printf("找不到指定文件");
		exit(-1);
	}
	fwrite(&grade, sizeof(int), 1, fp);//格式化写入
	fclose(fp);
	fp = NULL;
}
void InitInterface()//初始化游戏界面 包括墙 空白 
{
	ColorForInterface(4);//墙设置为红色
	int i, j;
	for (i = 0; i < ROW; i++)//打印墙，选取行列的最大值进行打印
	{
		for (j = 0; j < COL;j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL;//将此位置记录成墙
				CursorJump(2 * j, i);//光标跳转，2*j扩大游戏版面
				printf("■ ");
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL;
				printf("■ ");
			}
			else
			{
				face[i][j] = KONG;//其他的位置暂时设置为空白位置
			}
		}
	}
	ColorForInterface(7); //颜色设置为白色
	//在游戏下方打印得分和最高得分
	CursorJump(0, ROW);
	printf("当前得分:%d", grade);
	CursorJump(COL, ROW);
	printf("历史最高得分:%d", max);
}
void InitSnake()//对蛇的初始化，包括蛇的初始位置，蛇位置的记录
{
	snake.len = 2;//初始蛇的长度是2
	snake.x = COL / 2;//定义蛇头的位置
	snake.y = ROW / 2;
	body[0].x = COL / 2 - 1;//蛇身体的位置，在蛇的后面
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	face[snake.y][snake.x] = HEAD;//记录位置为蛇头
	face[body[0].y][body[0].x] = BODY;//记录位置为蛇身体
	face[body[1].y][body[1].x] = BODY;
}
void RandFood()//随机函数
{
	int x, y;
	do
	{
		y = rand() % COL;//生成在行列以内的随机坐标
		x = rand() % ROW;
	} while (face[x][y] != KONG);//生成在位置为空的位置
	face[x][y] = FOOD;//记录位置是食物位置
	CursorJump(2*y,x);
	printf("●");
}
void DrawSnake(int flag) //打印蛇 蛇的移动覆盖
{
	if (flag == 1)//打印蛇
	{
		ColorForInterface(10);//蛇的颜色为绿色
		CursorJump(2*snake.x, snake.y);
		printf("●");
		for (int i = 0; i < snake.len; i++)
		{
			CursorJump(2*body[i].x, body[i].y);
			printf("□");
		}
	}
	else//关于覆盖蛇
	{
		if (body[snake.len - 1].x != KONG)//为了避免蛇的尾部的旁边是墙
		{
			CursorJump(2 * body[snake.len - 1].x, body[snake.len - 1].y);//跳转到蛇尾打印空格
			printf(" ");
		}
	}
}
void JudgeFunc(int x, int y)//判断游戏进度
{
	//若蛇头即将到达的位置是食物，则得分
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //蛇身加长
		grade += 10; //更新当前得分
		ColorForInterface(7); //颜色设置为白色
		CursorJump(0, ROW);
		printf("当前得分:%d", grade); //重新打印当前得分
		RandFood(); //重新随机生成食物
	}
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //留给玩家反应时间
		system("cls"); //清空屏幕
		ColorForInterface(7); //颜色设置为白色
		CursorJump(2 * (COL / 3), ROW / 2 - 3);
		if (grade > max)
		{
			printf("恭喜你打破最高记录，最高记录更新为%d", grade);
			UpDateGread();
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
void MoveSnake(int x, int y)//蛇的移动
{
	DrawSnake(0); //先覆盖当前所显示的蛇
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //蛇移动后蛇尾重新标记为空
	face[snake.y][snake.x] = BODY; //蛇移动后蛇头的位置变为蛇身
	//蛇移动后各个蛇身位置坐标需要更新，更改成其后一个位置的状态
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
void PressTheKeyToGet(int x, int y)//获取键盘按键
{
	int t=0;
	while (1)
	{
		if (t == 0)
		{
			if (grade <=50)
				t = 10000;//表示获取按键的时间间隔
			if (grade > 50 && grade <= 100)
				t = 9000;
			if (grade > 100)
				t = 8000;
		}
			
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
void Gamebodylogic()
{
	int n = RIGHT;//默认的蛇向右走
	int MovingDirection = 0;//记录蛇移动的方向
	goto first;//按键初始不同调整直接进first
	while (1)
	{
		n = getch();
		//键值的调整，向上/下就不能直接向下/上了
		//向左/右就不能直接向右/左了
		switch (n)
		{
		case UP :
		case DOWN:
			if (MovingDirection != LEFT && MovingDirection != RIGHT)//向上/下就不能直接向下/上了
			{
				n = MovingDirection;
			}
			break;
		case RIGHT:
		case LEFT:
			if (MovingDirection != UP && MovingDirection != DOWN)	//向左/右就不能直接向右/左了
			{
				n = MovingDirection;
			}
		case SPACE:
		case ESC:
		case 'r':
		case 'R':
			break;
		default:
			n = MovingDirection;//其他按键均保持和上次运行方向一致
			break;
		}
	first:
		switch (n)
		{
		case UP:
			//因为坐标系建在左上角，向上运动是纵坐标是减1，其他同理
			PressTheKeyToGet(0, -1);
			MovingDirection = UP;
			break;
		case DOWN:
			PressTheKeyToGet(0, 1);
			MovingDirection = DOWN;
			break;
		case LEFT:
			PressTheKeyToGet(-1,0);
			MovingDirection = LEFT;
			break;
		case RIGHT:
			PressTheKeyToGet(1, 0);
			MovingDirection = RIGHT;
			break;
		case SPACE:
			system("pause>nul");
			break;
		case ESC:
			system("cls");
			ColorForInterface(7);
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
	a = GameBegin();//游戏初始界面
	ColorForInterface(7);
	system("cls");
	HideCursor();//光标隐藏
	ReadMaxGrade();//读取max
	InitInterface(); //初始化界面
	InitSnake(); //初始化蛇
	srand((unsigned int)time(NULL)); //设置随机数生成起点
	RandFood(); //随机生成食物
	DrawSnake(1); //打印蛇
	Gamebodylogic();
	return 0;

}

















