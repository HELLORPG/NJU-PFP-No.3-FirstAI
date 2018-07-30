#include "Reversi.h"

#define MODE 3//MODE 1是老师给的函数
//MODE 2是最大翻子个数的函数
//MODE 3是加入了算法的构造

//下面是测试的模式
#define POWER_TEST 0//如果为1，则输出测试权重
//终了

const int power[8][8] = { { 4,-3, 2, 0, 0, 2,-3, 4 },
						  {-3,-3,-2, 0, 0,-2,-3,-3 },
						  { 2,-2, 2, 0, 0, 2,-2, 2 },
						  { 0, 0, 0, 0, 0, 0, 0, 0 },
						  { 0, 0, 0, 0, 0, 0, 0, 0 },
						  { 2,-2, 2, 0, 0, 2,-2, 2 },
						  {-3,-3,-2, 0, 0,-2,-3,-3 },
						  { 4,-3, 2, 0, 0, 2,-3, 4 } };//这个作为新的权重的表示方法，进行调试测试---------->呆子1.1进行编写
//终了


void Reversi::Judge(const int board[][8]) const
{
	int black = 0, white = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{

			if (board[i][j] == 1)
				black++;
			else if (board[i][j] == 2)
				white++;
		}
	if (ownColor == 0)
		cout << "--------------------------->己方:对方 = " << black << ":" << white << endl;
	else
		cout << "--------------------------->己方:对方 = " << white << ":" << black << endl;
	return;
}

pair<int, int> Reversi::step()
{
	//algorithm
	cout << "--------------------------->step调用" << endl;
	int r = -1;
	int c = -1;
	LARGE_INTEGER begin, end;
	QueryPerformanceCounter(&begin);
#if MODE == 1
	r = 5;
	c = 3;
#endif

#if MODE == 2
	int board[8][8] = { 0 };
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = chessboard[i][j];
	int ban_place[4][2] = { -10,-10,-10,-10,-10,-10,-10,-10 };//作为禁手的地点
	if (now_row != -1 && now_col != -1)
	{
		ban_place[0][0] = now_row - 1, ban_place[0][1] = now_col;
		ban_place[1][0] = now_row + 1, ban_place[1][1] = now_col;
		ban_place[2][0] = now_row, ban_place[2][1] = now_col - 1;
		ban_place[3][0] = now_row, ban_place[3][1] = now_col + 1;
	}
	//禁手的格子已经进行了记录

	vector<vector<int> > choice;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] != 0)
				continue;
			int num = this->Flap(board, i, j, ownColor, false);
			if (num > 0)
			{
				bool flag = false;
				for (int k = 0; k < 4; k++)
				{
					if (i == ban_place[k][0] && j == ban_place[k][1])
					{
						flag = true;
						break;
					}
				}
				if (flag)
				{
					flag = false;
					continue;
				}
				vector<int> mid;
				mid.push_back(i), mid.push_back(j), mid.push_back(num);
				choice.push_back(mid);
			}
		}
	}
	if (choice.size() > 0)
	{
		//这时候进行静态权重的评估		
		for (int i = 0; i < choice.size(); i++)//对于所有的选择的遍历
			choice[i][2] += power[choice[i][0]][choice[i][1]];//将静态权重赋予指定选择的权重
															  //静态权重终止
		int best_choice[2] = { 0,-100000 };
		for (int i = 0; i < choice.size(); i++)
		{
			//这里是一个观察权重的语句
#if POWER_TEST == 1
			cout << "--------------------------->(" << choice[i][0] << "," << choice[i][1] << ")权重为" << choice[i][2] << endl;
#endif
			if (choice[i][2] > best_choice[1])
				best_choice[0] = i, best_choice[1] = choice[i][2];
		}
		r = choice[best_choice[0]][0];
		c = choice[best_choice[0]][1];
	}
#endif

#if MODE == 3
	vector<GRP*> tail;
	GRP* HEAD;
	int board[8][8] = { 0 };
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = chessboard[i][j];
	HEAD = this->Build_Tree(board, now_row, now_col, 4, tail);
	cout << "--------------------------->树建立完毕" << endl;
	//cout << "?" << endl;
	if (!HEAD)
		r = -1, c = -1;
	else
	{
		this->Compute(tail, 4);
		//cout << "?" << endl;
		int best_choice[3] = { 0,0,-1000000 };
		for (int i = 0; i < HEAD->choice.size(); i++)
		{
			/*if (HEAD->choice[i][0] == 0 && HEAD->choice[i][1] == 0)
				HEAD->power[i] += 5;
			else if (HEAD->choice[i][0] == 0 && HEAD->choice[i][1] == 7)
				HEAD->power[i] += 5;
			else if (HEAD->choice[i][0] == 7 && HEAD->choice[i][1] == 0)
				HEAD->power[i] += 5;
			else if (HEAD->choice[i][0] == 7 && HEAD->choice[i][1] == 7)
				HEAD->power[i] += 5;*/
			HEAD->power[i] += power[HEAD->choice[i][0]][HEAD->choice[i][1]];
		}
		for (int i = 0; i < HEAD->choice.size(); i++)
		{
			cout << "(" << HEAD->choice[i][0] << "," << HEAD->choice[i][1] << ") " << HEAD->power[i] << endl;
			if (HEAD->power[i] > best_choice[2])
			{
				best_choice[0] = HEAD->choice[i][0];
				best_choice[1] = HEAD->choice[i][1];
				best_choice[2] = HEAD->power[i];
			}
		}

		r = best_choice[0];
		c = best_choice[1];
	}



#endif
	QueryPerformanceCounter(&end);
	cout << "--------------------------->决策耗时" << (double)(end.QuadPart - begin.QuadPart) / (double)TIME.QuadPart << "s" << endl;
	return make_pair(r, c);
}

void Reversi::Build_Next_Point(GRP *now)//这样子将头结点放出来单独进行讨论
{
	for (int now_point = 0; now_point < now->choice.size(); now_point++)//每一个选择生成一种下一步
	{
		GRP *p = new GRP;
		p->before = now;//向前指针
		p->before_num = now_point;
		now->next[now_point] = p;//将点进行连接

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				p->board[i][j] = now->board[i][j];//进行棋盘的复制
		this->Flap(p->board, now->choice[now_point][0], now->choice[now_point][1], now->color, true);//进行翻转

		if (now->color == ownColor)
			p->color = oppositeColor;
		else
			p->color = ownColor;//将下一步的颜色进行确定

		int ban[4][2];//用来确定ban位
		ban[0][0] = now->choice[now_point][0] - 1, ban[0][1] = now->choice[now_point][1];
		ban[1][0] = now->choice[now_point][0] + 1, ban[1][1] = now->choice[now_point][1];
		ban[2][0] = now->choice[now_point][0], ban[2][1] = now->choice[now_point][1] - 1;
		ban[3][0] = now->choice[now_point][0], ban[3][1] = now->choice[now_point][1] + 1;

		bool ban_flag = true;
		if (now->choice[now_point][0] == -1 && now->choice[now_point][1] == -1)
			ban_flag = false;

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				if (p->board[i][j] != 0)
					continue;//跳过节约计算量
				bool flag = false;
				int flap_num = this->Flap(p->board, i, j, p->color, false);
				if (flap_num > 0)
				{
					if (ban_flag)//如果是有ban位的情况
					{
						for (int k = 0; k < 4; k++)
						{
							if (ban[k][0] == i && ban[k][1] == j)
							{
								flag = true;
								break;
							}
						}
						if (flag)
							continue;
					}
					vector<int> mid;
					mid.push_back(i);
					mid.push_back(j);
					p->choice.push_back(mid);
					mid.clear();

					if (p->color != ownColor)
						flap_num = -flap_num;//如果是对方的落子，就是会对于自己的负数翻子

					p->power.push_back(flap_num);
					p->next.push_back(NULL);
				}
			}
		if (p->choice.size() == 0)
		{
			//cout << "发现了无子可下情况" << endl;
			vector<int> mid;
			mid.push_back(-1);
			mid.push_back(-1);
			p->choice.push_back(mid);
			if (p->color == ownColor)
			{
				p->power.push_back(0);//自己被禁手是大忌
				//now->power[now_point] += -20;
			}
			else
			{
				p->power.push_back(0);//对方被禁手是好事
				//now->power[now_point] += 20;
			}
			p->next.push_back(NULL);
		}
	}
	return;
}

GRP* Reversi::Build_Tree(const int board[][8], int row, int col, int level, vector<GRP*> &tail)
{
	GRP *HEAD = new GRP;//建立头结点
	HEAD->before = NULL;
	HEAD->color = ownColor;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			HEAD->board[i][j] = board[i][j];
	int ban[4][2] = { -10,-10,-10,-10,-10,-10,-10,-10 };

	if (row != -1 && col != -1)
	{
		ban[0][0] = row - 1, ban[0][1] = col;
		ban[1][0] = row + 1, ban[1][1] = col;
		ban[2][0] = row, ban[2][1] = col - 1;
		ban[3][0] = row, ban[3][1] = col + 1;
	}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			bool flag = false;
			if (board[i][j] != 0)
				continue;
			int flap_num = this->Flap(HEAD->board, i, j, ownColor, false);
			if (flap_num > 0)
			{
				for (int k = 0; k < 4; k++)
				{
					if (i == ban[k][0] && j == ban[k][1])
					{
						flag = true;
						break;
					}
				}
				if (flag)
					continue;
				vector<int> mid;
				mid.push_back(i);
				mid.push_back(j);
				HEAD->choice.push_back(mid);
				HEAD->power.push_back(flap_num);
				HEAD->next.push_back(NULL);
			}			
		}

	cout << "--------------------------->树头建立完毕" << endl;
	if (HEAD->choice.size() == 0)
		return NULL;//如果无子可下，返回空指针，进行之后的操作
	else
	{
		vector<GRP*> record;
		vector<GRP*> next_record;
		record.push_back(HEAD);
		for (int now_level = 0; now_level < level; now_level++)
		{
			next_record.clear();
			for (int i = 0; i < record.size(); i++)
			{
				this->Build_Next_Point(record[i]);
				vector<GRP*> mid;
				for (int j = 0; j < record[i]->next.size(); j++)
					next_record.push_back(record[i]->next[j]);
			}
			record.clear();
			record = next_record;
			cout << "--------------------------->第" << now_level + 1 << "层建立完毕" << endl;
		}
		tail.clear();
		tail = next_record;
	}
	return HEAD;
}

void Reversi::Compute(vector<GRP*> tail, int level)
{
	vector<GRP*> before_record;
	GRP* before_p = NULL;
	for (;;)
	{
		before_record.clear();
		for (int i = 0; i < tail.size(); i++)
		{
			GRP* p = tail[i]->before;
			if (p != before_p)
				before_record.push_back(p);
			//需要计算相对应的权重最小值
			if (tail[i]->color == oppositeColor)
			{
				int min = 100000;
				for (int k = 0; k < tail[i]->power.size(); k++)
					if (tail[i]->power[k] < min)
						min = tail[i]->power[k];
				p->power[tail[i]->before_num] += min;
			}
			else
			{
				int max = -100000;
				for (int k = 0; k < tail[i]->power.size(); k++)
					if (tail[i]->power[k] > max)
						max = tail[i]->power[k];
				p->power[tail[i]->before_num] += max;
			}

			before_p = p;
		}
		tail.clear();
		tail = before_record;
		//cout << "?" << endl;
		if (tail[0]->before == NULL)
			break;
	}
	//cout << "?" << endl;
	return;
}