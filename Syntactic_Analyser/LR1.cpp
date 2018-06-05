#include "LR1.hpp"

LR1::LR1()
{
	Init();
	getProjectset();
}

void LR1::Init()//读入文法
{
	vector<string> right_tmp;
	right_tmp.push_back("S");
	set<string> search_tmp;
	search_tmp.insert("#");
	init.push_back(LR1Item(string("S'"), right_tmp, search_tmp, 0));//0号产生式
	ifstream file_in("syntax.txt");
	string buffer;
	while (getline(file_in, buffer))
	{
		string left, tmp;
		vector<string> right;
		stringstream stringcin(buffer);
		stringcin >> left >> tmp;
		vn.insert(left);
		vnt.insert(left);
		while (stringcin >> tmp)
		{
			right.push_back(tmp);
			if (tmp == "@")
				continue;
			vnt.insert(tmp);
			if (tmp[0] >= 'A' && tmp[0] <= 'Z')
				vn.insert(tmp);//非终结符用大写字母
			else
				vt.insert(tmp);//终结符用小写字母
		}
		init.push_back(LR1Item(left, right, search_tmp, 0));				
	}
	vt.insert("#");
	vnt.insert("#");
	file_in.close();
}

void LR1::printInit()
{
	for (vector<LR1Item>::iterator iter = init.begin(); iter != init.end(); iter++)
	{
		cout<<iter->left<<" -> ";
		for (int i = 0; i < iter->right.size(); i++)
		{
			if (i == iter->position)
				cout<<".";
			cout<<(iter->right)[i]<<' ';
		}
		cout<<",";
		for (set<string>::iterator it = iter->search.begin(); it != iter->search.end(); it++)
			cout<<*it<<' ';
		cout<<endl;
	}
}

		
set<string> LR1::dfs(vector<string> &afterset)
{
			set<string> firstset;
			for (int i = 0; i < afterset.size(); i++)
			{
				string after = afterset[i];
				if (vt.find(after) != vt.end() || after == "@")
				{
					firstset.insert(after);
					return firstset;
				}
				bool flag = true;
				for (int j = 0; j < init.size(); j++)
				{
					if (init[j].left == after)
					{
						if (init[j].right[0] == after)
							continue;
						set<string> first = dfs(init[j].right);
						if (first.find("@") == first.end())
						{
							flag = false;
							firstset.insert(first.begin(), first.end());
						}
						else
						{
							flag = true;
							if ((i + 1) == afterset.size())
								firstset.insert(first.begin(), first.end());
							else
							{
								for (set<string>::iterator iter = first.begin(); iter != first.end(); iter++)
								{
									if (*iter != "@")
										firstset.insert(*iter);
								}
							}
						}

					}
				}
				if (!flag)
					break;
			}
			return firstset;
}
		
		
set<LR1Item> LR1::Derive(LR1Item &pre)
{
	set<LR1Item> derivation;
	set<string> first;
	derivation.insert(pre);
	string next = pre.right[pre.position];
	if (vn.find(next) == vn.end() || next == "@")
		return derivation;//点之后的符号不是非终结符直接返回
	if ((pre.position + 1) < pre.right.size())//点之后除非终结符外还有别的符号
	{
		vector<string> after;
		for (int i = pre.position + 1; i < pre.right.size(); i++)
			after.push_back(pre.right[i]);//点之后的部分提取出来
		first = dfs(after);
		if (first.count("@") > 0)
		{
			first.insert(pre.search.begin(), pre.search.end());
			first.erase("@");
		}
	}
	else//点之后只有一个非终结符
	{
		first.insert(pre.search.begin(), pre.search.end());
		for (set<string>::iterator iter = pre.search.begin(); iter != pre.search.end(); iter++)
			first.insert(*iter);
	}
	for (int i = 0; i < init.size(); i++)
	{
		if (init[i].left == next)
			derivation.insert(LR1Item(next, init[i].right, first, 0));

	}
	return derivation;
}

void LR1::getProjectset()
{
	LR1Item accept = init[0];
	accept.position++;
	set<LR1Item> I0 = Derive(init[0]);//第0个状态I0,由S'-> ·S推导出的
	while (true)//第0个状态I0,由上述状态其他产生式非终结符推导出的
	{
		int len = I0.size();
		for (set<LR1Item>::iterator iter = I0.begin(); iter != I0.end(); iter++)
		{
			if (iter->position < iter->right.size())
			{
				LR1Item tmp = *iter;
				set<LR1Item> derivation = Derive(tmp);
				I0.insert(derivation.begin(), derivation.end());
			}
		}
		if (len == I0.size())
			break;
	}


	cout<<"I0:\n";
	for (set<LR1Item>::iterator iter = I0.begin(); iter != I0.end(); iter++)
	{
		cout<<iter->left<<"->";
		for (int i = 0; i < iter->right.size(); i++)
		{
			if (iter->position == i)
				cout<<". ";
			cout<<iter->right[i]<<" ";
		}

		if (iter->position == iter->right.size())
			cout<<".";
		cout<<",";
		for (set<string>::iterator it = iter->search.begin(); it != iter->search.end(); it++)
			cout<<'/'<<*it;
		cout<<endl;
	}
	cout<<endl;

			///////////////////
	for (set<LR1Item>::iterator iter = I0.begin(); iter != I0.end(); iter++)
	{
		if (iter->right[0] == "@")//处理I0中的可以导出空集的产生式
		{
			LR1Item tmp = *iter;
			int id = 0;
			for (; id < init.size(); id++)
				if(init[id].left == tmp.left && init[id].right == tmp.right)
					break;
			set<string> search = iter->search;
			for (set<string>::iterator it = search.begin(); it != search.end(); it++)
				Action[0][*it] = id+guiyue;
		}
	}
	I.push_back(I0);
	for (int i = 0; i < I.size(); i++)//i是状态号
	{
		set<LR1Item> iset = I[i];
		for (set<string>::iterator iter = vnt.begin(); iter != vnt.end(); iter++)//对于每一个状态遍历所有符号
		{
			set<LR1Item> niset;
			for (set<LR1Item>:: iterator it = iset.begin(); it != iset.end(); it++)//这里是移点，将新的LR(1)项加入的操作
			{
				if (it->position < it->right.size() && (it->right[it->position]) == (*iter))
					niset.insert(LR1Item(it->left,it->right, it->search, it->position+1));
			}
					
			while (true)//移动点之后的扩展
			{
				int len = niset.size();
				for (set<LR1Item>:: iterator it = niset.begin(); it != niset.end(); it++)
				{
					if (it->position < it->right.size())
					{
						LR1Item tmp = (*it);
						set<LR1Item> derivation = Derive(tmp);
						niset.insert(derivation.begin(), derivation.end());
					}
				}
				if (len == niset.size())
					break;
			}
			if (niset.empty())
				continue;
					
			bool isnew = true;
			for (int j = 0; j < I.size(); j++)
			{
				if (I[j] == niset)//新的状态是已经存在的
				{
					isnew = false;
					if (vt.find(*iter) != vt.end())
						Action[i][*iter] = j;//说明当前符号是非终结符
					else
						Goto[i][*iter] = j;//符号是终结符
					break;
				}
			}

			
			if (isnew)//输出DFA的状态，此段代码可注释
			{
				cout<<"I"<<I.size()<<':'<<endl;
				for (set<LR1Item>::iterator ite = niset.begin(); ite != niset.end(); ite++)
				{
					cout<<ite->left<<"->";
					for (int i = 0; i < ite->right.size(); i++)
					{
						if (ite->position == i)
							cout<<". ";
						cout<<ite->right[i]<<" ";
					}
					if (ite->position == ite->right.size())
						cout<<'.';
					cout<<",";
					for(set<string>::iterator t = ite->search.begin(); t != ite->search.end(); t++)
						cout<<'/'<<*t;
					cout<<endl;
				}
				cout<<endl;
			}
					

			if (isnew)
			{
				I.push_back(niset);
				if (vt.find(*iter) != vt.end())
					Action[i][*iter] = I.size()-1;
				else
					Goto[i][*iter] = I.size()-1;
				if (niset.find(accept) != niset.end())
					Action[I.size()-1]["#"] = acc;
				else
				{
					for (set<LR1Item>::iterator it = niset.begin(); it != niset.end(); it++)
					{
						if (it->position == it->right.size() || it->right[0] == "@")
						{
							LR1Item tmp = *it;
							int id = 0;
							for (; id < init.size(); id++)
								if(init[id].left == tmp.left && init[id].right == tmp.right)
									break;
							for (set<string>::iterator t = it->search.begin(); t != it->search.end(); t++)
									Action[I.size()-1][*t] = id+guiyue;
						}
					}
				}
			}
		}

	}
}
	
void LR1::judge()
{
	freopen("RESULT", "w", stdout);
	stack<int> status;
	stack<string> symbol;
	status.push(0);
	while (lex.size() > 0)
	{
		Lex frolex = lex.back();
		int topsta = status.top();
		if (Action[topsta][frolex.word] == acc)
		{
			cout<<"Yeah!\n";
			return;
		}
		if (Action[topsta][frolex.word] != 0 || Action[topsta][frolex.type] != 0)
		{
			int number = max(Action[topsta][frolex.word], Action[topsta][frolex.type]);
			if (number >= guiyue)
			{
				int r = number - guiyue;
				LR1Item pre = init[r];

				if (pre.right[0] != "@")
				{
					for (int i = 0; i < pre.right.size(); i++)
						symbol.pop(), status.pop();
				}

				symbol.push(pre.left);

				cout << pre.left << " -> ";
				for(vector<string>::iterator it = pre.right.begin(); it != pre.right.end(); it++)
					cout << (*it) << " ";
				cout << endl;

				if (Goto[status.top()][pre.left] == 0)
				{
					cout<<"NO!\n";
					return;
				}
				else
					status.push(Goto[status.top()][pre.left]);
			}
			else
			{
				lex.pop_back();
				symbol.push(frolex.word);
				status.push(number);
			}
		}
		else
		{
			cout<<"NO!\n";
			return;
		}
	}
	fclose(stdout);
}

void LR1::printForm()
{
	int num = init.size();
	cout << "以下是Action子表\n";
	for(int i = 0; i < num; i++)
	{
		for(map<string,int>::iterator iter = Action[i].begin(); iter != Action[i].end(); iter++)
		{
			cout<< i << " - " << (iter->first) << " - " ;
			if((iter->second) == acc)
				cout << "accept\n";
			else if((iter->second) > guiyue)
				cout<< (iter->second)-guiyue << "（规约）"<<endl;
			else
				cout<< (iter->second) << endl;
	
		}
		cout << "-=-=-=-=-=-=-=" << endl;
	}
	
	cout << "\n\n以下是Goto子表\n";
	for(int i = 0; i < num; i++)
	{
		for(map<string,int>::iterator iter = Goto[i].begin(); iter != Goto[i].end(); iter++)
		{
			cout << i << " - " << (iter->first) << " - " << (iter->second) << endl;
		}
		cout << "-=-=-=-=-=-=-=" << endl;
	}
}