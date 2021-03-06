// 扫雷_vs2017.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
// 扫雷.cpp: 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TURE 1
#define FALSE 0
#define  max 100
typedef struct sweep        //设置结构体，用来定义扫雷 
{
	int row;   //行 
	int col;   //列 
	int mine;
	char sweep_mine[max][max];        //初始棋盘 
	char sweep_password[max][max];    //游戏棋盘 
	int visit[max][max];              //查看结点是否被访问过 
	int win;
}swp;

void init_sweep(swp &S)    //初始化扫雷棋盘 
{
	int i = 0, j = 0;
	for (i = 0; i < S.row; i++)
	{
		for (j = 0; j < S.col; j++)
		{
			S.sweep_mine[i][j] = '0';         //初始棋盘无雷地点为0 
			S.sweep_password[i][j] = '-';     //将游戏棋盘设置为‘-’ 
		}
	}
}
void display(swp &S)          //建立初始棋盘 
{
	int i = 0, j = 0;
	printf("  ");
	for (i = 1; i <= S.col - 2; i++)
	{
		printf(" %2d ", i);
	}
	printf("\n");
	for (i = 1; i < S.row - 1; i++)
	{
		printf("%2d", i);
		for (j = 1; j < S.col - 1; j++)
		{
			printf(" %2c ", S.sweep_password[i][j]);
		}
		printf("\n");
	}

}
void display1(swp &S)    //建立游戏完整棋盘 
{
	int i = 0, j = 0;
	printf("  ");
	for (i = 1; i <= S.col - 2; i++)
	{
		printf(" %2d ", i);
	}
	printf("\n");
	for (i = 1; i < S.row - 1; i++)
	{
		printf("%2d", i);
		for (j = 1; j < S.col - 1; j++)
		{
			if (S.sweep_mine[i][j] == '1')
				printf("  * ");
			else
				printf("  - ");
		}
		printf("\n");
	}
	printf("\n\n");
}

void  mine_layout(swp &S)  //用随机数布置雷 
{
	int x, y, Mine;
	Mine = S.mine;
	srand((unsigned)time(NULL));
	while (Mine)
	{
		x = (rand() % (S.row - 2)) + 1;
		y = (rand() % (S.col - 2)) + 1;
		if (S.sweep_mine[x][y] == '0')        //将布置的雷写入初始棋盘，0为空，1为雷 
		{
			S.sweep_mine[x][y] = '1';
			Mine--;
		}
	}
}
int num(swp S, int x, int y)         //计算周围8个格雷的数量 
{
	int count = 0;
	int a, b;
	for (a = x - 1; a <= x + 1; a++)             //遍历棋盘，并防止溢出
	{
		for (b = y - 1; b <= y + 1; b++)
		{

			if (S.sweep_mine[a][b] == '1')
			{

				count++;
			}
		}
	}
	return count;
}
void remove(swp &S, int x, int y)            //棋盘结点的扩展，类似dfs
{

	int a;
	int b;
	int ret = num(S, x, y), ret1;
	if (ret == 0)              //如果结点周围8个结点为0，则进行扩展
	{
		if (S.sweep_password[x][y] == 'P')
			S.sweep_password[x][y] = 'P' + 0;
		else
			S.sweep_password[x][y] = ret + '0';
		for (a = x - 1; a <= x + 1; a++)              //深度优先搜索
		{

			for (b = y - 1; b <= y + 1; b++)
			{
				if (a == 0 || b == 0 || a == S.row - 1 || b == S.col - 1)    //围出棋盘的边界，防止扩展时溢出
				{
					continue;
				}
				if (S.visit[a][b] == 0)             //如果结点没有被访问过，则标记visit为1，并将周围雷数写入游戏棋盘
				{
					ret1 = num(S, a, b);
					if (S.sweep_password[a][b] == 'P')
						S.sweep_password[a][b] = 'P' + 0;
					else
						S.sweep_password[a][b] = ret1 + '0';
					S.visit[a][b] = TURE;
					S.win++;
					if (ret1 == 0)
						remove(S, a, b);             //递归进行搜索
				}
			}
		}
	}
	else                                       //如果结点周围雷数不为0，则直接输出到游戏棋盘上
	{
		if (S.sweep_password[x][y] == 'P')
			S.sweep_password[x][y] = 'P' + 0;
		else
			S.sweep_password[x][y] = ret + '0';
		S.visit[x][y] = TURE;
		S.win++;

	}
}
int Sweep(swp &S)
{
	S.win = 0;
	int choise;
	int count = 0;
	int x = 0;
	int y = 0;
	int i;
	int j;
	int m;
	m = S.mine;
	for (i = 0; i < S.row; i++)
	{
		for (j = 0; j < S.col; j++)
			S.visit[i][j] = FALSE;             //设置所有棋盘上的结点初始为false
	}
	while (S.win != ((S.row - 2)*(S.col - 2)) - S.mine)    //判断玩家是否赢得比赛
	{
		printf("当前剩余雷数：%d\n", m);
		printf("请选择点开棋盘还是标记为雷\n1.点开棋盘  2.标记为雷 3.取消标记\n");
		scanf_s("%d", &choise);
		if (choise == 1)
		{
			printf("请输入坐标：");
			scanf_s("%d%d", &x, &y);
			if (S.sweep_mine[x][y] == '1')                      //判断玩家是否踩到雷
			{
				printf("你踩到雷了\n游戏结束！\n雷的位置如下:\n");
				display1(S);
				return 0;
			}
			else
			{
				remove(S, x, y);
				display(S);
			}
		}
		if (choise == 2)
		{
			printf("请输入坐标：");
			scanf_s("%d%d", &x, &y);
			if (S.sweep_password[x][y] == '-')
				S.sweep_password[x][y] = 'P';
			else
				printf("该位置已被点开，请重新输入标记位置:\n");
			display(S);
			if (S.sweep_mine[x][y] == '1')
			{
				m--;
			}
			if (m == 0)
				break;
		}

		if (choise == 3)
		{
			printf("请输入坐标：");
			scanf_s("%d%d", &x, &y);
			S.sweep_password[x][y] = '-';
			display(S);
			if (S.sweep_mine[x][y] == '1')
			{
				m++;
			}
		}
	}
	printf("恭喜你赢了！\n");
	return 0;
}
int main()
{
	int row, col;
	char point;
	double total_time;
	clock_t start, finish;
	swp S;
	while (1)
	{
		printf("请输入雷的数量：\n");
		scanf_s("%d", &S.mine);
		printf("请输入行和列：\n");
		scanf_s("%d%d", &row, &col);
		S.row = row + 2;
		S.col = col + 2;
		init_sweep(S);
		mine_layout(S);
		display(S);
		start = clock();
		Sweep(S);
		finish = clock();
		total_time = double(finish - start) / CLOCKS_PER_SEC;
		printf("本次游戏用时%.2lf秒\n", total_time);
		printf("再来一局？ y/n\n");
		scanf_s(" %c", &point);
		system("cls");
		if (point == 'y')
			continue;
		else if (point == 'n')
			exit(0);
	}
	return 0;
}

