/**
	@mainpage Project 1: implementing the replacement selection algorithm
	@author Jeff Witthuhn 
	@USER MANUAL:
	-a) class ReplacementSelection
	-b) This class on instantiation will generaterate an output file 
	that contains each run of run of the replacement selection algorithm
	with the data set specified. The user specifies the type of data, size of
	simulated memory, input file, output file, and specifies the direction of
	the sort. 
	-c) On my computer it is located in C:\Research\331\project1
	-d) compile the program simply run the make program on the project's directory
	-e) EXAMPLE RUN
		-INPUT.txt:
			1 2 3 4 5 -1 -2 -3 -4 -5 1 2 3 4 5 1 2 3 4 5 -1 -2 -3 -4 -5	9 8 7 6 5 4 3 2 1 
			1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 9 8 7 6 5 4 3 2 1 1 2 3 4 5 6 7 8 9 1 2 
			3 4 5 6 7 8 9 9 8 7 6 5 4 3 2 1 9 8 7 6 5 4 3 2 1

		-PROGRAM: 	
			ifstream integers("INPUT.txt")
			ofstream intOut("OUTPUT.txt")
			//default is ascending=true order (last parameter)
			ReplacementSelection<int> sortedints(10,integers,intOut);


		-OUTPUT.txt:
			-5 -4 -3 -2 -1 1 1 1 2 2 2 3 3 3 4 4 4 5 5 5 5 6 7 8 9 
			-5 -4 -3 -2 -1 1 1 2 2 3 3 4 4 4 5 5 6 6 7 7 8 8 8 9 9 9 
			0 1 1 1 2 2 2 3 3 3 4 4 5 5 6 6 6 7 7 7 8 8 8 9 9 9 
			1 2 2 3 3 4 4 5 5 5 6 6 7 7 8 9 
			1 1 2 3 4 
	as you can see, each individual run is sorted in ascending order. These will
	later be merged into a complely sorted file in a later program. 

	-f) known errors are few, but it has only been tested for memory sizes less than 100
		The test driver crashes when specifying random string lengths when generating 
		test files for the class. 



**/


/**
	* @file ReplacementSelection.h
	* Header file for the replacement selection algorithm implementation
	* @author Jeffrey Witthuhn

**/
#ifndef REPLACEMENT_SELECTION
#define REPLACEMENT_SELECTION
#include <vector> 
#include <iostream>
#include <fstream>
#include <functional>
#include <typeinfo>


using namespace std; 
template <class obj>
class ReplacementSelection{
private: 

	///data members
	bool activeLeft; ///true => left (lower) indices are active
	bool ascending;///true => sort in ascending order
	int size; ///size of the simulated memory, does not change during execution
	int leftStart;//root index of left heap
	int leftEnd;///final index in left heap
	int rightStart;///root index of right heap
	int rightEnd;///final index of right heap
	vector<obj> heap;//the dual heap used in the algorithm

	///sorting methods
	/**
	heapify using the sift down method, the paramter leftSide indicates which side
	of the dual heap is to be heapified. 
	**/
	void heapify (bool leftSide);
	void siftUp(int index, bool leftSide);
	void siftDown (int index, bool leftSide);
	/**
	*left,right,parent will return the left,right or parent indices
	*given an element and which side of the dual heap it belongs to. 
	**/
	int left(int index, bool leftSide);
	int right(int index, bool leftSide);
	int parent(int index, bool leftSide);
	void swap(int index1, int index2);
	/**
	*removes the top item in the heap and maintains the heap properties
	**/
	obj pop();
	obj fill(istream& infile);/// initializes the heap for the first part of the algorithm
	/**
	*pushes entry into the corresponding heap and maintains the heap property.
	**/
	void current_heap_push(const obj entry);
	void pending_heap_push (const obj entry);
	void toggleActive();///toggles which heap is the 'active' heap
	void printHeap();//prints the heap to cout for monitoring purposes
	/**
	*the driving method for the algorithm. 
	**/
	void sortPreMerge(istream& infile,ostream& outfile);

public: 
	///constructor, on initialization runs the algorithm. 
	ReplacementSelection(const int _size=10,  istream& _infile = new ifstream("input.txt"),
				ostream& _outfile = cout,  const bool _sort = true);
};
#endif