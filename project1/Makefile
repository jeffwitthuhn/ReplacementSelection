
all: test 
test: main.cpp ReplacementSelection.o
	g++ ReplacementSelection.o main.cpp -std=c++11 -o test
ReplacementSelection.o:ReplacementSelection.cpp ReplacementSelection.h
	g++ -c ReplacementSelection.cpp
