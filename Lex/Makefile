myLex:main.o Lex.o
	g++ -std=c++11 -o myLex main.o Lex.o
main.o:main.cpp
	g++ -std=c++11 -c main.cpp
Lex.o:Lex.hpp Lex.cpp
	g++ -std=c++11 -c Lex.cpp Lex.hpp

clean:
	rm *o *.gch myLex