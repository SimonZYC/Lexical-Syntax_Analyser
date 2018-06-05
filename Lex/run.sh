#!/bin/bash
echo -e "编译与链接:"
make
echo -e "\n运行myLex程序:"
./myLex
echo -e "\n编译生成词法分析程序:"
gcc -o lexAnalyser out.c
echo -e "\n运行词法分析程序:"
./lexAnalyser temp.c
echo -e "\n清除编译链接生成的程序:"
make clean
rm lexAnalyser