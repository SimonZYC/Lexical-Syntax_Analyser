Syntactic_Analyser: main.o LR1.o
	g++ -o Syntactic_Analyser main.o LR1.o
main.o: main.cpp
	g++ -c main.cpp
LR1.o: LR1.hpp LR1.cpp
	g++ -c LR1.hpp LR1.cpp

clean:
	rm *.gch *.o Syntactic_Analyser