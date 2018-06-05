echo "编译链接语法分析程序:"
make
echo -e "\n运行语法分析程序, 结果保存在RESULT文件中"
./Syntactic_Analyser

echo -e "\n清除编译链接生成的程序"
make clean