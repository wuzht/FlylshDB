#ifndef MYFILEIO_H_
#define MYFILEIO_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include "myVector.h"
using namespace std;

/*
Read data from dataset and do something you like.
infile : filename of dataset
n      : number of data point to be read
d      : dimension of a data point
*/
void fileIO(const char* infile, const int n, const int d, int* index, int* sumVec) {
	int indexOfFile = 0;
	ifstream fin(infile, ios::in);
	if (!fin.is_open()) {
		cout << "Can not open!" << endl;
		return;
	}
	
	string linestr;
	string str;
	
	myVector v(d);
	int count = 0;
	int* slot = new int[n];
	int numOfSlots = 0;
	int sum = 0;
		
	while (getline(fin, linestr, '\n')) {
		string fileName = "mnistFiles/file";
		
		stringstream s0;
		s0 << indexOfFile;

		fileName += s0.str();
//		fileName += ".txt";
	
		ofstream fout(fileName, ios::binary | ios::app);
//		ofstream fout(fileName, ios::binary);	
		if (!fout.is_open()) {
			cout << "Can not open!" << endl;
			return;
		}
		stringstream ss;
		ss << linestr;
		
		int flag = 0;
		int index = 0;
		int temp1 = 0;
		while (getline(ss, str, ' ')) {
			string t_str = "";
			for (unsigned i = 0; i < str.size(); i ++) {
				if (str[i] != ' ') {
					t_str += str[i];
				}
			}
			
			if (flag == 0) {
				sum++;
				temp1 = stoi(t_str);
				v.setId(temp1);
			}
			else {
				v[index] = stof(t_str);	
				index ++;
			}
			
			flag ++;	
		}

		fout.write((char *)&temp1, sizeof(int));
		fout.write((char *)v.getVector(), d * sizeof(float));
		
		slot[count] = 1; 
		numOfSlots ++;
		count ++;
		if (count == n) {
			
			slot[count] = 1; 
			indexOfFile ++;	
			
			fout.write((char *)slot, n * sizeof(int));
			fout.write((char *)&numOfSlots, sizeof(int));
			
			for (int i = 0; i < numOfSlots; i ++) {
				slot[i] = 0;
			}
			numOfSlots = 0;
			count = 0;

			fout.close();
		}
	}
	
	//fout.close();
	fin.close();
	*index = indexOfFile - 1;
	*sumVec = sum;
}

/*
	write the data in v into the file with a name fileName
	n - the number of vector in the file
	d - the dimension of vector
*/
void myWrite(string fileName, myVector* v, const int n, const int d) {	
	int numOfSlots = 0;
	int* slots = new int[n];
	ofstream fout(fileName, ios::binary);
	if (!fout.is_open()) {
		cout << "myWrite can not open the file!" << endl;
		return;
	}
	
	for (int i = 0; i < n; i ++) {
		int temp = v[i].getId();
		fout.write((char *)&temp, sizeof(int));
		fout.write((char *)v[i].getVector(), d * sizeof(float));
		
		slots[i] = 1;
	}
	
	fout.write((char *)slots, n * sizeof(int));
	fout.write((char *)&numOfSlots, sizeof(int));
	
	fout.close();
	if (slots != nullptr) {
		delete []slots;
		slots = nullptr;
	}
}

/*
	read a file with a index
	indexOfFile - the index of the file to be read
	n - the number of vector in the file
	d - the dimension of vector
	indexOfLastFile - the index of the last file, to change the value of n if the file to be read is the last file
	numOfVecInLastFil - the number of vectors in the last file
*/
 
myVector* myRead(const int indexOfFile, int n, const int d, const int indexOfLastFile, const int numOfVecInLastFile, string fileName = "files/file") {
	// 获取文件名 
	stringstream s0;
	s0 << indexOfFile;

	fileName += s0.str();
//	fileName += ".txt";
	
	//string fileName = "file43920.txt";
	
	ifstream file(fileName, ios::binary);
	if (!file) {
		cout << fileName << endl;
		cout << "Can not open!" << endl;
		return NULL;
	} 
	
	if (indexOfFile == indexOfLastFile) n = numOfVecInLastFile; 
	
	myVector *v = new myVector[n];
	if(v == nullptr){
		cout << "new vectors failed" << endl;
	}

	for (int i = 0; i < n; i ++) {
		bool test = v[i].newForPoint(d);
		if(!test){
			cout << "new for point failed" << endl;
		}
		
	}

	for (int i = 0; i < n; i ++) {
		int id = 0;
		file.read((char *)&id, sizeof(int));
		v[i].setId(id);
		file.read((char *)v[i].getVector(), d * sizeof(float));
	}
	
	
	int* slots = new int[n];
	int numOfSlots = 0;
	file.read((char *)slots, n * sizeof(int));
	
	file.read((char *)&numOfSlots, sizeof(int));
	
	file.close();

	if (slots != nullptr) {
		delete []slots;
		slots = nullptr;
	}
	return v;
}

#endif
