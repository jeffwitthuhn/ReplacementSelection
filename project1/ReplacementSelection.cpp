#include "ReplacementSelection.h"
#include <string>
template <class obj>
ReplacementSelection<obj>::ReplacementSelection(const int _size, istream& _infile,
				ostream& _outfile,  const bool _sort){
		size=_size;
		ascending=_sort;
		activeLeft=true;
		leftStart=0;
		leftEnd=0;
		rightStart=size-1;
		rightEnd=size;
		heap.resize(size);
		sortPreMerge(_infile, _outfile);
}
template <class obj>
obj ReplacementSelection<obj>::fill(istream& infile){
	obj newelement;
	int i;
	int newIndex;
	infile>>newelement; 
	for(i=0; i<size && !infile.eof(); i++){
		heap[i]=newelement;
		leftEnd=i;
		heapify(true);
		infile>>newelement;

	}
	return newelement;
}

template <class obj>
void ReplacementSelection<obj>::sortPreMerge(std::istream& infile,std::ostream& outfile){
	vector<vector<obj> >sortedLists;
	vector<obj> currentList;
	obj newelement;
	obj lastElement;
	bool pendingFull=false;
	bool currentHeapEligible;
	bool endOfFile=false;
	///while replacement selection vector is not full, heapify a new element
	newelement = fill(infile);
	///while there is a new element
	while(!endOfFile){
		///Pop the root of the current heap, adding it to the current list.
		if(infile.eof()){
			endOfFile=true;
		}
		lastElement=pop();
		currentList.push_back(lastElement);
		if(ascending){
			if(newelement>=lastElement){
				currentHeapEligible=true;
			}
			else{
				currentHeapEligible=false;
			}
		}
		else {
			if(newelement<=lastElement){
				currentHeapEligible=true;
			}
			else{
				currentHeapEligible=false;
			}
		}
		///If the new element is greater than or equal to the last element popped, push it onto the current heap
		if(currentHeapEligible){
					current_heap_push(newelement);
				}
		///If the new element is less than (for a min-heap) to the last element popped,
		else {
			///push it onto the pending heap

			pending_heap_push(newelement);
			if(activeLeft){
				if(leftStart>leftEnd){
					pendingFull=true;
				}
				else{
				}
			}
			else {
				if(rightStart<rightEnd){
					pendingFull=true;
				}
				else {
				}
			}

			if(pendingFull){
				sortedLists.push_back(currentList);
				currentList.resize(0);
				toggleActive();
				pendingFull=false;
			}
		}

		infile>>newelement;
	}
	if(activeLeft){
		while(leftStart<=leftEnd){
			currentList.push_back(pop());
			leftEnd--;

		}
	}
	else {
		while(rightStart>=rightEnd){
			currentList.push_back(pop());
			rightEnd++;

		}
	}
	sortedLists.push_back(currentList);
	currentList.resize(0);
	toggleActive();
	if(activeLeft){
		while(leftStart<=leftEnd){
			currentList.push_back(pop());
			leftEnd--;

		}
	}
	else {
		while(rightStart>=rightEnd){
			currentList.push_back(pop());
			rightEnd++;

		}
	}
	sortedLists.push_back(currentList);
	for(int i=0; i<sortedLists.size(); i++){
		for(int j=0; j<sortedLists[i].size(); j++)
			outfile<<sortedLists[i][j]<<" ";
		outfile<<endl;
	}
	
}

template <class obj>
void ReplacementSelection<obj>::heapify(bool leftSide){
	int index;
	if(leftSide){
		index=parent(leftEnd, leftSide);
		while(index>=leftStart){
			siftDown(index, leftSide);
			index-=1;
		}
	}
	else {
		index=parent(rightEnd,leftSide);
		while(index<=rightStart){
			siftDown(index,leftSide);
			index+=1;
		}
	}
}

template <class obj>
void ReplacementSelection<obj>::siftUp(int index, bool leftSide){
	int p=parent(index, leftSide);
	int toSwap = index;
	if(leftSide&&ascending){
		if((p>=leftStart)&&(heap[index]<heap[p]))
			toSwap=p;
	}
	else if(leftSide){//&&!ascending
		if((p>=leftStart)&&(heap[index]>heap[p]))
			toSwap=p;
	}
	else if(ascending){//&&!left
		if((p<=rightStart)&&(heap[index]<heap[p]))
			toSwap=p;
	}
	else {//!ascending&&!left
		if((p<=rightStart)&&(heap[index]>heap[p]))
			toSwap=p;
	}
	if(toSwap!=index){
		swap(index,toSwap);
		siftUp(toSwap,leftSide);
	}
}

template <class obj>
void ReplacementSelection<obj>::siftDown(int index, bool leftSide){
	int l, r, toSwap;
	l=left(index,leftSide);
	r=right(index,leftSide);
	if(leftSide&&ascending){
		if(l<=leftEnd&&heap[l]<heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r<=leftEnd&&heap[r]<heap[toSwap])
			toSwap=r;
	}
	else if(leftSide){//&&!ascending
		if(l<=leftEnd&&heap[l]>heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r<=leftEnd&&heap[r]>heap[toSwap])
			toSwap=r;
	}
	else if(ascending){//&&!left
		if(l>=rightEnd&&heap[l]<heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r>=rightEnd&&heap[r]<heap[toSwap])
			toSwap=r;
	}
	else {//!ascending&&!left
		if(l>=rightEnd&&heap[l]>heap[index])
			toSwap=l;
		else 
			toSwap=index;
		if(r>=rightEnd&&heap[r]>heap[toSwap])
			toSwap=r;
	}
	if(toSwap!=index){
		swap(index,toSwap);
		siftDown(toSwap,leftSide);
	}
}

template <class obj>
obj ReplacementSelection<obj>::pop(){
	obj out; 
	if(activeLeft){
		out=heap[leftStart];
		swap(leftStart,leftEnd);
		leftEnd--;
		siftDown(leftStart,activeLeft);
		leftEnd++;
	}
	else{
		out=heap[rightStart];
		swap(rightStart,rightEnd);
		rightEnd++;
		siftDown(rightStart, activeLeft);
		rightEnd--;
	}
	return out;
} 

template <class obj>
void ReplacementSelection<obj>::current_heap_push(obj entry){
	if(activeLeft){
		heap[leftEnd]=entry;
		siftUp(leftEnd,activeLeft);
	}
	else{
		heap[rightEnd]=entry;
		siftUp(rightEnd,activeLeft);
	}
}

template <class obj>
void ReplacementSelection<obj>::pending_heap_push(obj entry){
	if(activeLeft){
		leftEnd--;
		rightEnd--;
		heap[rightEnd]=entry;
		siftUp(rightEnd,!activeLeft);
	}
	else{
		leftEnd++;
		rightEnd++;
		heap[leftEnd]=entry;
		siftUp(leftEnd,!activeLeft);
	}
}

template <class obj>
void ReplacementSelection<obj>::toggleActive(){
	activeLeft=!activeLeft;
}
template <class obj>
void ReplacementSelection<obj>::swap(int index1, int index2){
	obj temp;
	temp=heap[index1];
	heap[index1]=heap[index2];
	heap[index2]=temp;
}

template <class obj>
int ReplacementSelection<obj>::left(int index, bool leftSide){
	return (leftSide? 2*index +1 : rightStart-(rightStart-index)*2-1);
}
template <class obj>
int ReplacementSelection<obj>::right(int index, bool leftSide){
	return (leftSide? 2*index +2 : rightStart-(rightStart-index)*2-2);
}
template <class obj>
int ReplacementSelection<obj>::parent(int index, bool leftSide){
	return (leftSide? (index-1)/2 : (index-rightStart+1)/2+rightStart);
}

template <class obj>
void ReplacementSelection<obj>::printHeap(){
		if(activeLeft){
			cout<<"active:("<<leftStart<<", "<<leftEnd<<") ";
			for(int i=leftStart; i<=leftEnd; i++)
				cout<<heap[i]<<" ";
			cout<<"  pending:("<<rightStart<<", "<<rightEnd<<") ";
			for(int i=rightStart; i>=rightEnd; i--)
				cout<<heap[i]<<" ";
			cout<<endl;
			}
		else{
			cout<<"active:("<<rightStart<<", "<<rightEnd<<") ";
			for(int i=rightStart; i>=rightEnd; i--)
				cout<<heap[i]<<" ";
			cout<<"  pending:("<<leftStart<<", "<<leftEnd<<") ";
			for(int i=leftStart; i<=leftEnd; i++)
				cout<<heap[i]<<" ";
			cout<<endl;
		}
}

/**
*allows ints, strings, doubles, and floats to be used with the class. 
*to allow more, add similar lines with desired types
**/
template class ReplacementSelection<int>;
template class ReplacementSelection<string>;
template class ReplacementSelection<double>;
template class ReplacementSelection<float>;