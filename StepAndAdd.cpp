#include "Reversi.h"

#define MODE 3//MODE 1����ʦ���ĺ���
//MODE 2������Ӹ����ĺ���
//MODE 3�Ǽ������㷨�Ĺ���

//�����ǲ��Ե�ģʽ
#define POWER_TEST 0//���Ϊ1�����������Ȩ��
//����

const int power[8][8] = { { 4,-3, 2, 0, 0, 2,-3, 4 },
						  {-3,-3,-2, 0, 0,-2,-3,-3 },
						  { 2,-2, 2, 0, 0, 2,-2, 2 },
						  { 0, 0, 0, 0, 0, 0, 0, 0 },
						  { 0, 0, 0, 0, 0, 0, 0, 0 },
						  { 2,-2, 2, 0, 0, 2,-2, 2 },
						  {-3,-3,-2, 0, 0,-2,-3,-3 },
						  { 4,-3, 2, 0, 0, 2,-3, 4 } };//�����Ϊ�µ�Ȩ�صı�ʾ���������е��Բ���---------->����1.1���б�д
//����


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
		cout << "--------------------------->����:�Է� = " << black << ":" << white << endl;
	else
		cout << "--------------------------->����:�Է� = " << white << ":" << black << endl;
	return;
}

pair<int, int> Reversi::step()
{
	//algorithm
	cout << "--------------------------->step����" << endl;
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
	int ban_place[4][2] = { -10,-10,-10,-10,-10,-10,-10,-10 };//��Ϊ���ֵĵص�
	if (now_row != -1 && now_col != -1)
	{
		ban_place[0][0] = now_row - 1, ban_place[0][1] = now_col;
		ban_place[1][0] = now_row + 1, ban_place[1][1] = now_col;
		ban_place[2][0] = now_row, ban_place[2][1] = now_col - 1;
		ban_place[3][0] = now_row, ban_place[3][1] = now_col + 1;
	}
	//���ֵĸ����Ѿ������˼�¼

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
		//��ʱ����о�̬Ȩ�ص�����		
		for (int i = 0; i < choice.size(); i++)//�������е�ѡ��ı���
			choice[i][2] += power[choice[i][0]][choice[i][1]];//����̬Ȩ�ظ���ָ��ѡ���Ȩ��
															  //��̬Ȩ����ֹ
		int best_choice[2] = { 0,-100000 };
		for (int i = 0; i < choice.size(); i++)
		{
			//������һ���۲�Ȩ�ص����
#if POWER_TEST == 1
			cout << "--------------------------->(" << choice[i][0] << "," << choice[i][1] << ")Ȩ��Ϊ" << choice[i][2] << endl;
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
	cout << "--------------------------->���������" << endl;
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
	cout << "--------------------------->���ߺ�ʱ" << (double)(end.QuadPart - begin.QuadPart) / (double)TIME.QuadPart << "s" << endl;
	return make_pair(r, c);
}

void Reversi::Build_Next_Point(GRP *now)//�����ӽ�ͷ���ų���������������
{
	for (int now_point = 0; now_point < now->choice.size(); now_point++)//ÿһ��ѡ������һ����һ��
	{
		GRP *p = new GRP;
		p->before = now;//��ǰָ��
		p->before_num = now_point;
		now->next[now_point] = p;//�����������

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				p->board[i][j] = now->board[i][j];//�������̵ĸ���
		this->Flap(p->board, now->choice[now_point][0], now->choice[now_point][1], now->color, true);//���з�ת

		if (now->color == ownColor)
			p->color = oppositeColor;
		else
			p->color = ownColor;//����һ������ɫ����ȷ��

		int ban[4][2];//����ȷ��banλ
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
					continue;//������Լ������
				bool flag = false;
				int flap_num = this->Flap(p->board, i, j, p->color, false);
				if (flap_num > 0)
				{
					if (ban_flag)//�������banλ�����
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
						flap_num = -flap_num;//����ǶԷ������ӣ����ǻ�����Լ��ĸ�������

					p->power.push_back(flap_num);
					p->next.push_back(NULL);
				}
			}
		if (p->choice.size() == 0)
		{
			//cout << "���������ӿ������" << endl;
			vector<int> mid;
			mid.push_back(-1);
			mid.push_back(-1);
			p->choice.push_back(mid);
			if (p->color == ownColor)
			{
				p->power.push_back(0);//�Լ��������Ǵ��
				//now->power[now_point] += -20;
			}
			else
			{
				p->power.push_back(0);//�Է��������Ǻ���
				//now->power[now_point] += 20;
			}
			p->next.push_back(NULL);
		}
	}
	return;
}

GRP* Reversi::Build_Tree(const int board[][8], int row, int col, int level, vector<GRP*> &tail)
{
	GRP *HEAD = new GRP;//����ͷ���
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

	cout << "--------------------------->��ͷ�������" << endl;
	if (HEAD->choice.size() == 0)
		return NULL;//������ӿ��£����ؿ�ָ�룬����֮��Ĳ���
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
			cout << "--------------------------->��" << now_level + 1 << "�㽨�����" << endl;
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
			//��Ҫ�������Ӧ��Ȩ����Сֵ
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