#include "ReplacementSelection.h"
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <random>
#include <cmath>
#include <sstream>
using namespace std;

//generates a random string, borrowed from 
//http://stackoverflow.com/a/440240/5476556
void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}
void generateTestFileStrings(ostream& outfile){
	int strlength=0;
	int fileLen;
	fileLen = rand() % (2000+rand() % 10000); 
	for(int i=0; i<fileLen;i++){
		strlength= 20;
		char* str = (char*) malloc(strlength * (sizeof(char)));
		gen_random(str,strlength);
		outfile<<str<<" ";
		if(i%5==0){
			outfile<<"\n";
		}

	}
	
}
template<class obj>
void generateTestFile(ostream& outfile){
	int strlength;
	int fileLen;
	fileLen = rand() % (2000+rand() % 10000);
	for(int i=0; i<fileLen;i++){
		strlength=1+rand()%6;
		obj num = rand()%(int)(pow(10,strlength));
		outfile<<num<<" ";
		if(i%5==0){
			outfile<<"\n";
			}

		}
}
template <class obj> 
void checkRuns(istream& infile,ostream& outfile, bool ascending){
	int runLength=0;
	int maxRunLength=0;
	int totalObj=0;
	int totalRuns=0;
	bool isSorted=true;
	obj prevObj;
	obj curObj;
	string curLine;
	while(getline(infile, curLine)){
		runLength=0;
		outfile<<curLine<<"\n";
		totalRuns++;
		istringstream stringStream(curLine);
		stringStream>>prevObj;
		runLength++;
		totalObj++;
		while(!stringStream.eof()){
			stringStream>>curObj;
			runLength++;
			totalObj++;
			if(ascending){
				if(curObj<prevObj){
					isSorted=false;
				}
			}
			else {
				if(curObj>prevObj){
					isSorted=false;
				}

			}
		}
		if(runLength>maxRunLength){
			maxRunLength=runLength;
		}
	}
	outfile <<"\n\n-----------------------------------\n"
			<<"averageRunLength = "<<totalObj/totalRuns
			<<"\nthe file is sorted: "<< (isSorted? "True":"False")
			<<"\nMax run length: "<<maxRunLength
			<<"\ntotal Runs: "<<totalRuns
			<<"\ntotal items: "<<totalObj
			<<"\n---------------------------------------\n\n";
}
void testStrings(string inputFileName, string outputFileName, bool ascending){
	ofstream randomTestFile(inputFileName);
	cout<<"generating test filestr\n";
	generateTestFileStrings(randomTestFile); 
	randomTestFile.close();

	ifstream inputFile(inputFileName);
	ofstream otemp("temp.txt");
	cout<<"replacement selection\n";
	ReplacementSelection<string> sortedRuns(rand()%10+5,inputFile,otemp,ascending);
	otemp.close();

	cout<<"checking runs\n";
	ifstream itemp("temp.txt");
	ofstream outputFile(outputFileName);
	checkRuns<string>(itemp, outputFile, ascending);
}
template <class obj>
void test(string inputFileName, string outputFileName, bool ascending){
	ofstream randomTestFile(inputFileName);
	cout<<"generating test file\n";
	generateTestFile<obj>(randomTestFile); 
	randomTestFile.close();

	ifstream inputFile(inputFileName);
	ofstream otemp("temp.txt");
	cout<<"rs\n";
	ReplacementSelection<obj> sortedRuns(15,inputFile,otemp,ascending);
	otemp.close();

	ifstream itemp("temp.txt");
	ofstream outputFile(outputFileName);
	cout<<"out\n";
	checkRuns<obj>(itemp, outputFile, ascending);
}
int main(){
	srand(1);
	ifstream integers("intinput.txt");
	ifstream strings("stringinput.txt");
	ofstream intOut("integeroutput.txt");
	ofstream strOut("stringoutput.txt");
	ReplacementSelection<int> sortedints(10,integers,intOut);
	ReplacementSelection<string> sortedstrings(5,strings,strOut);
	strOut.close();
	intOut.close();
	integers.close();
	strings.close();
	test<double> ("randomDoubleTest_in.txt", "ascending_randomDoubleTest_out.txt", true);
	test<int> ("randomIntTest_in.txt","ascending_randomIntTest_out",true);
	test<double> ("randomDoubleTest_in.txt", "descending_randomDoubleTest_out.txt", false);
	test<int> ("randomIntTest_in.txt","descending_randomIntTest_out",false);
	testStrings ("randomStringTest_in.txt", "ascending_randomStringTest_out",true);
	testStrings ("randomStringTest_in.txt", "descending_randomStringTest_out",false);
	test<float> ("randomfloatTest_in.txt", "ascending_randomfloatTest_out",true);
	test<float> ("randomfloatTest_in.txt", "descending_randomfloatTest_out",false);


	return 0;
}
