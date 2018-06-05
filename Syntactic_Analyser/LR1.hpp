#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <set>
using namespace std;

const int state_maxn = 300;
const int guiyue = 1<<20;
const int acc = 1<<30;

struct LR1Item
{
	string left;
	vector<string> right;
	set<string> search;
	int position;

	LR1Item(string left, vector<string> right, set<string> search, int position):
	left(left), right(right), search(search), position(position) {}

	bool operator < (LR1Item const &other) const
	{
		if (position != other.position)
			return position < other.position;
		if (left != other.left)
			return left < other.left;
		if (right != other.right)
		{
			if (right.size() != other.right.size())
				return right.size() < other.right.size();
			for (int i = 0; i < right.size(); i++)
				if (right[i] != other.right[i])
					return right[i] < other.right[i];
		}
		if (search != other.search)
			return search < other.search;
		return false;
	}

	bool operator == (LR1Item const &other) const
	{
		if (other.left == left && other.right == right && 
			other.search == search && other.position == position)
			return true;
		else  
			return false;
	}
};
struct Lex
{
    string word;
    string type;
    Lex(string word, string type): word(word), type(type){}
};


class LR1
{
	public:
		vector<Lex> lex;
		map<string, int> Action[state_maxn], Goto[state_maxn];
		vector<set<LR1Item> > I;
		vector<LR1Item> init;	//存放LR(1)项
		set<string> vn, vt, vnt;

		void Init();

		void printInit();

		set<string> dfs(vector<string> &afterset);		
		
		set<LR1Item> Derive(LR1Item &pre);
	
		void getProjectset();

		LR1();
		
		void judge();

		void printForm();
		
};
