plagiarismCatcher: main.o
	g++ -o plagiarismCatcher main.o
	rm main.o
main.o: main.cpp hash.h
	g++ -std=c++0x -c main.cpp
