// para2ndIIR.cpp : Defines the entry point for the console application.
//
#include "para2ndIIR.h"
#include<iostream>
#include <fstream>

using namespace std;

const int dataLen = 128;
double dataS[dataLen] = { 1, }; // creat the pulse of len: dataLen
const int fs = 48000;

void main()
{
	para2ndIIR filter125, filter250, filter500, filter1k, filter2k, filter4k, filter8k;
	filter125.setPara(125, 1.414, 0, fs);
	filter250.setPara(250, 1.414, 0, fs);

	// for data writing
	const int maxPathLen = 256;
	char filename[maxPathLen];
	cout << "Enter a file name and press ENTER:  ";
	cin.getline(filename, maxPathLen);
	ofstream file_out(filename);
	if (!file_out) {
		cout << filename << " could not be opened."<<endl;
	}
	cout << filename << " was opened." << endl;
	cout << endl;
	for (int i = 0; i < dataLen; i++) {
		file_out << filter125.filter(dataS[i]) << endl;
	}
	file_out.close();

}




