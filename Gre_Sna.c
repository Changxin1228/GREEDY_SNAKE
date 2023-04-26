#pragma warning (disable:4996)
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#define ROW 22//��
#define COL 42//��
#define WALL 1//ǽ
#define KONG 0//��
#define HEAD 77//��ͷ
#define BODY 88//����
#define FOOD 99//ʳ��
#define UP 72 //��
#define DOWN 80 //��
#define LEFT 75 //��
#define RIGHT 77 //��
#define SPACE 32 //��ͣ
#define ESC 27 //�˳�
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



//��������
void color(int c)//��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //��ɫ����
}
void CursorJump(int x ,int y)//�����ת
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(handle, pos);
}
char GameBegin()//��Ϸ��ʼ����
{
	printf("************************************************************************************************************************\n");
	//system("color 70");
	//printf("\t\t\t\t\t\tGREEDY_SNAKE\n");
	printf("\n");
	color(4);
	printf("\t\t\t\t ���� ��  ����������   ������������ ������������ ��������   ������ ������\n");
	printf("\t\t\t\t��  ����   ��   ��   ��      ��      ��  ��   ��   �� \n");
	printf("\t\t\t\t��   ��   ��   ��   ��      ��      ��   ��  ��   �� \n");
	printf("\t\t\t\t��       ��   ��   ��      ��      ��   ��   �� �� \n");
	printf("\t\t\t\t��       ��������    ��������   ��������   ��   ��    �� \n");
	printf("\t\t\t\t��  ������  �� ��     ��      ��      ��   ��    ��\n");
	printf("\t\t\t\t��   ��   ��  ��    ��      ��      ��   ��    ��\n");
	printf("\t\t\t\t��   ��   ��   ��   ��      ��      ��  ��     ��   \n");
	printf("\t\t\t\t ������   ������  ���� ������������ ������������ ��������     ������ \n");
	printf("\n");
	printf("\t\t\t\t    ������  ����  ������    ��    ������  ���� ������������\n");
	printf("\t\t\t\t   ��   ��  ��   ��     ��     ��   ��   ��\n");
	printf("\t\t\t\t   ��      ����  ��    �� ��    ��  ��    ��\n");
	printf("\t\t\t\t   ��      ����  ��    �� ��    ��  ��    ��\n");
	printf("\t\t\t\t     ��    �� �� ��   ��   ��   ������     ��������\n");
	printf("\t\t\t\t      ��   ��  ����   ����������   �� ��     ��\n");
	printf("\t\t\t\t       ��  ��  ����   ��   ��   ��  ��    ��\n");
	printf("\t\t\t\t   ��   ��  ��   ��   ��   ��   ��   ��   �� \n");
	printf("\t\t\t\t    ������  ������  ��  ������ ������ ������  ���� ������������\n");
	char a;
	printf("\n");
	printf("\n\n\n\n");
	color(7);
	printf("\t\t\t\t��������������������������������������������������������������������������������������������������������\n");
	
	printf("\t\t\t\t\t\t��ʼ��Ϸ(y/n):");
	scanf("%c", &a);
	return a;
}
void HideCursor()//�������
{
	CONSOLE_CURSOR_INFO cursor;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &cursor);
}
void ReadMaxGrade()//��ȡ��߳ɼ���max
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
void WriteGrade()//������߷�
{
	FILE* fp = fopen("greedy_snake.txt", "w");
	if (fp == NULL)
	{
		printf("�Ҳ���ָ���ļ�");
		exit(-1);
	}
	fwrite(&grade, sizeof(int), 1, fp);
	fclose(fp);
	fp = NULL;
}
void InitInterface()
{
	color(4);//ǽ����Ϊ��ɫ
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL;j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL;
				CursorJump(2 * j, i);
				printf("�� ");
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL;
				printf("�� ");
			}
			else
			{
				face[i][j] = KONG;
			}
		}
	}
	color(7); //��ɫ����Ϊ��ɫ
	CursorJump(0, ROW);
	printf("��ǰ�÷�:%d", grade);
	CursorJump(COL, ROW);
	printf("��ʷ��ߵ÷�:%d", max);
}
void InitSnake()//��ʼ����
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
void RandFood()//�������
{
	int x, y;
	do
	{
		y = rand() % COL;
		x = rand() % ROW;
	} while (face[x][y] != KONG);
	face[x][y] = FOOD;
	CursorJump(2*y,x);//       *
	printf("��");
}
void DrawSnake(int flag) //��ӡ�� �ߵ��ƶ�����
{
	if (flag == 1)
	{
		color(10);
		CursorJump(2*snake.x, snake.y);
		printf("��");
		for (int i = 0; i < snake.len; i++)
		{
			CursorJump(2*body[i].x, body[i].y);
			printf("��");
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
	//����ͷ���������λ����ʳ���÷�
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //����ӳ�
		grade += 10; //���µ�ǰ�÷�
		color(7); //��ɫ����Ϊ��ɫ
		CursorJump(0, ROW);
		printf("��ǰ�÷�:%d", grade); //���´�ӡ��ǰ�÷�
		RandFood(); //�����������ʳ��
	}
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //������ҷ�Ӧʱ��
		system("cls"); //�����Ļ
		color(7); //��ɫ����Ϊ��ɫ
		CursorJump(2 * (COL / 3), ROW / 2 - 3);
		if (grade > max)
		{
			printf("��ϲ�������߼�¼����߼�¼����Ϊ%d", grade);
			WriteGrade();
		}
		else if (grade == max)
		{
			printf("����߼�¼��ƽ�������ٴ��Ѽ�");
		}
		else
		{
			printf("��������ͣ���ǰ����߼�¼���%d", max - grade);
		}
		CursorJump(2 * (COL / 3), ROW / 2);
		printf("GAME OVER");
		while (1) //ѯ������Ƿ�����һ��
		{
			char ch;
			CursorJump(2 * (COL / 3), ROW / 2 + 3);
			printf("����һ��?(y/n):");
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
				printf("ѡ��������ٴ�ѡ��");
			}
		}
	}
}
void MoveSnake(int x, int y)
{
	DrawSnake(0); //�ȸ��ǵ�ǰ����ʾ����
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //���ƶ�����β���±��Ϊ��
	face[snake.y][snake.x] = BODY; //���ƶ�����ͷ��λ�ñ�Ϊ����
	//���ƶ����������λ��������Ҫ����
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//���ƶ�����ͷλ����Ϣ��Ϊ��0�������λ����Ϣ
	body[0].x = snake.x;
	body[0].y = snake.y;
	//��ͷ��λ�ø���
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	DrawSnake(1); //��ӡ�ƶ������
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
			if (kbhit() != 0) //�����̱��û������˳�ѭ��
				break;
		}
		if (t == 0)
		{
			JudgeFunc(x, y); //�жϵ����λ�ú��Ƿ�÷�����Ϸ����
			MoveSnake(x, y); //�ƶ���
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
		//��ֵ�ĵ���
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
			printf("  ��Ϸ����  ");
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
	system("title ̰����");
	a = GameBegin();
	color(7);
	system("cls");
	HideCursor();//�������
	ReadMaxGrade();//��ȡmax
	InitInterface(); //��ʼ������
	InitSnake(); //��ʼ����
	srand((unsigned int)time(NULL)); //����������������
	RandFood(); //�������ʳ��
	DrawSnake(1); //��ӡ��
	Game();
	return 0;

}

















