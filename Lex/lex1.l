%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int KEY_NUM = 32;
const char* KEY_SET[] = {
    "auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long", "register",
	"return", "short", "signed", "sizeof", "static", "struct",
	"switch", "typedef", "union", "unsigned", "void", "volatile",
	"while"
};

int isDigit(char ch)
{
	if(ch <= '9' && ch >= '0')
		return 1;
	return 0;
}

int isLetter(char ch)
{
	if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	return 0;
}

%}

%!
noone=isNoOne
notow=isNoTow
letter=isLetter
digit=isDigit
%!


%%
a(a|b)* {
	int id = getKeyId(SYLEX_TEXT);
	if(id != 0)
		printf("<%s,->\n", SYLEX_TEXT);
	else
	{
		printf("<$ID,%s>\n", SYLEX_TEXT);
	}
}
%%