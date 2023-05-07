#pragma warning (disable:4996)//���õ�һЩ����
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
//����������Ϸ�е�����ת��Ϊ��ֵ
#define ROW 22//��
#define COL 42//��
#define WALL 1//ǽ
#define KONG 0//�հ�
#define HEAD 77//��ͷ
#define BODY 88//����
#define FOOD 99//ʳ��
#define UP 72 //��
#define DOWN 80 //��
#define LEFT 75 //��
#define RIGHT 77 //��
#define SPACE 32 //��ͣ
#define ESC 27 //�˳�
struct snake_head//�����ߵ�ͷ������
{
	int len;//��
	//������Ϸ�տռ��� ͷ������
	int x;
	int y;
}snake;
struct snake_body//����
{
	//ĳһ�������� x��y
	int x;
	int y;
}body[ROW*COL];//��һ��һ�㹻��¼����������������ά����

int face[ROW][COL];//��¼����Ϸ��������λ�õ���Ϣ

int max, grade;//��߷֣��͸ߵ÷ּ�¼



//��������
void ColorForInterface(int c)//���õ�ǰ������ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //��ɫ���ã���ȡ����̨���
}
void CursorJump(int x ,int y)//�����ת
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(handle, pos);
	//SetConsoleCursorPosition�ǹ����ת����ͨ����ȡ����̨����Լ��ṹ��COORD�е�x��y����ȷ�����λ��
}
char GameBegin()//��Ϸ��ʼ����
{
	printf("************************************************************************************************************************\n");
	printf("\n");
	ColorForInterface(4);//��ɫ��greedy snake
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
	ColorForInterface(7);
	printf("\t\t\t\t��������������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t��ʼ��Ϸ(y/n):");
	scanf("%c", &a);
	getchar();//�Ե��س�
	if (a=='n'||a=='N')
	{
		system("cls");
		printf("\t\t\t��ӭ�´�����");
		exit(-1);
	}
	return a;
}
void HideCursor()//�������
{
	CONSOLE_CURSOR_INFO cursor;//������ṹ��CONSOLE_CURSOR_INFO
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &cursor);//������غ�����ͬ����Ҫ��ȡ���
}
void ReadMaxGrade()//��ȡ��߳ɼ���max
{
	FILE* fp = fopen("greedy_snake.txt", "r");
	if (fp == NULL)
	{
		fp = fopen("greedy_snake.txt", "w");
		fwrite(&max, sizeof(int), 1, fp);
	}
	//�򿪻��ߴ����ļ�
	fseek(fp, 0, SEEK_SET);//��궨λ���ļ��Ŀ�ͷ
	fread(&max, sizeof(int), 1, fp);//��ȡһ��max
	fclose(fp);
	fp = NULL;
}
void UpDateGread()//������߷�
{
	FILE* fp = fopen("greedy_snake.txt", "w");
	if (fp == NULL)
	{
		printf("�Ҳ���ָ���ļ�");
		exit(-1);
	}
	fwrite(&grade, sizeof(int), 1, fp);//��ʽ��д��
	fclose(fp);
	fp = NULL;
}
void InitInterface()//��ʼ����Ϸ���� ����ǽ �հ� 
{
	ColorForInterface(4);//ǽ����Ϊ��ɫ
	int i, j;
	for (i = 0; i < ROW; i++)//��ӡǽ��ѡȡ���е����ֵ���д�ӡ
	{
		for (j = 0; j < COL;j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL;//����λ�ü�¼��ǽ
				CursorJump(2 * j, i);//�����ת��2*j������Ϸ����
				printf("�� ");
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL;
				printf("�� ");
			}
			else
			{
				face[i][j] = KONG;//������λ����ʱ����Ϊ�հ�λ��
			}
		}
	}
	ColorForInterface(7); //��ɫ����Ϊ��ɫ
	//����Ϸ�·���ӡ�÷ֺ���ߵ÷�
	CursorJump(0, ROW);
	printf("��ǰ�÷�:%d", grade);
	CursorJump(COL, ROW);
	printf("��ʷ��ߵ÷�:%d", max);
}
void InitSnake()//���ߵĳ�ʼ���������ߵĳ�ʼλ�ã���λ�õļ�¼
{
	snake.len = 2;//��ʼ�ߵĳ�����2
	snake.x = COL / 2;//������ͷ��λ��
	snake.y = ROW / 2;
	body[0].x = COL / 2 - 1;//�������λ�ã����ߵĺ���
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	face[snake.y][snake.x] = HEAD;//��¼λ��Ϊ��ͷ
	face[body[0].y][body[0].x] = BODY;//��¼λ��Ϊ������
	face[body[1].y][body[1].x] = BODY;
}
void RandFood()//�������
{
	int x, y;
	do
	{
		y = rand() % COL;//�������������ڵ��������
		x = rand() % ROW;
	} while (face[x][y] != KONG);//������λ��Ϊ�յ�λ��
	face[x][y] = FOOD;//��¼λ����ʳ��λ��
	CursorJump(2*y,x);
	printf("��");
}
void DrawSnake(int flag) //��ӡ�� �ߵ��ƶ�����
{
	if (flag == 1)//��ӡ��
	{
		ColorForInterface(10);//�ߵ���ɫΪ��ɫ
		CursorJump(2*snake.x, snake.y);
		printf("��");
		for (int i = 0; i < snake.len; i++)
		{
			CursorJump(2*body[i].x, body[i].y);
			printf("��");
		}
	}
	else//���ڸ�����
	{
		if (body[snake.len - 1].x != KONG)//Ϊ�˱����ߵ�β�����Ա���ǽ
		{
			CursorJump(2 * body[snake.len - 1].x, body[snake.len - 1].y);//��ת����β��ӡ�ո�
			printf(" ");
		}
	}
}
void JudgeFunc(int x, int y)//�ж���Ϸ����
{
	//����ͷ���������λ����ʳ���÷�
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //����ӳ�
		grade += 10; //���µ�ǰ�÷�
		ColorForInterface(7); //��ɫ����Ϊ��ɫ
		CursorJump(0, ROW);
		printf("��ǰ�÷�:%d", grade); //���´�ӡ��ǰ�÷�
		RandFood(); //�����������ʳ��
	}
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //������ҷ�Ӧʱ��
		system("cls"); //�����Ļ
		ColorForInterface(7); //��ɫ����Ϊ��ɫ
		CursorJump(2 * (COL / 3), ROW / 2 - 3);
		if (grade > max)
		{
			printf("��ϲ�������߼�¼����߼�¼����Ϊ%d", grade);
			UpDateGread();
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
void MoveSnake(int x, int y)//�ߵ��ƶ�
{
	DrawSnake(0); //�ȸ��ǵ�ǰ����ʾ����
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //���ƶ�����β���±��Ϊ��
	face[snake.y][snake.x] = BODY; //���ƶ�����ͷ��λ�ñ�Ϊ����
	//���ƶ����������λ��������Ҫ���£����ĳ����һ��λ�õ�״̬
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
void PressTheKeyToGet(int x, int y)//��ȡ���̰���
{
	int t=0;
	while (1)
	{
		if (t == 0)
		{
			if (grade <=50)
				t = 10000;//��ʾ��ȡ������ʱ����
			if (grade > 50 && grade <= 100)
				t = 9000;
			if (grade > 100)
				t = 8000;
		}
			
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
void Gamebodylogic()
{
	int n = RIGHT;//Ĭ�ϵ���������
	int MovingDirection = 0;//��¼���ƶ��ķ���
	goto first;//������ʼ��ͬ����ֱ�ӽ�first
	while (1)
	{
		n = getch();
		//��ֵ�ĵ���������/�¾Ͳ���ֱ������/����
		//����/�ҾͲ���ֱ������/����
		switch (n)
		{
		case UP :
		case DOWN:
			if (MovingDirection != LEFT && MovingDirection != RIGHT)//����/�¾Ͳ���ֱ������/����
			{
				n = MovingDirection;
			}
			break;
		case RIGHT:
		case LEFT:
			if (MovingDirection != UP && MovingDirection != DOWN)	//����/�ҾͲ���ֱ������/����
			{
				n = MovingDirection;
			}
		case SPACE:
		case ESC:
		case 'r':
		case 'R':
			break;
		default:
			n = MovingDirection;//�������������ֺ��ϴ����з���һ��
			break;
		}
	first:
		switch (n)
		{
		case UP:
			//��Ϊ����ϵ�������Ͻǣ������˶����������Ǽ�1������ͬ��
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
	a = GameBegin();//��Ϸ��ʼ����
	ColorForInterface(7);
	system("cls");
	HideCursor();//�������
	ReadMaxGrade();//��ȡmax
	InitInterface(); //��ʼ������
	InitSnake(); //��ʼ����
	srand((unsigned int)time(NULL)); //����������������
	RandFood(); //�������ʳ��
	DrawSnake(1); //��ӡ��
	Gamebodylogic();
	return 0;

}

















