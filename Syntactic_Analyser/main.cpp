// #include<iostream>
// #include<fstream>
// using namespace std;
#include"LR1.hpp"

int main()
{
	LR1 lr1;
	freopen("input.txt", "r", stdin);
	char word[100], type[100];
	while (scanf("%s%s", type, word) != EOF)
		lr1.lex.push_back(Lex(word, type));
	lr1.lex.push_back(Lex("#", "halt"));
	for (int i = 0, j = lr1.lex.size()-1; i < j; i++, j--)
		swap(lr1.lex[i], lr1.lex[j]);
	fclose(stdin);

	// for (int i = 0; i < lex.size(); i++)
 //    	cout<<lex[i].type<' '<<lex[i].word<<endl;

	lr1.printForm();
	lr1.judge();

	return 0;
}