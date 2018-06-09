#ifndef MYFILEPROCESS_H_
#define MYFILEPROCESS_H_
#include"myFileIO.h"
#include<math.h>

int findN(int d){
	int vec = (d + 1) * 4;
	int n = 1;
	while(vec * n + n + 4 < pow(2,16)){
		n++;
	}
	return n - 1;
}

void findDimension(const char* infile,int* n,int* d){
	int index = 0;
	ifstream fin(infile, ios::in);
	if (!fin.is_open()) {
		cout << "Can not open!" << endl;
		return;
	}
	
	string linestr;
	string str;
		
	if(getline(fin, linestr, '\n')) {
		stringstream ss;
		ss << linestr;
		
		int flag = 0;
		while (getline(ss, str, ' ')) {
			string t_str = "";
			for (unsigned i = 0; i < str.size(); i ++) {
				if (str[i] != ' ') {
					t_str += str[i];
				}
			}
			
			if (flag == 0) 
				flag++;
			else
				index++;
		}		
	}
	*d = index;
	*n = findN(*d);
}

int lastFile(const int indexOfFile,const int n,const int sumVec){
	int sum = indexOfFile * n;
	int res = sumVec - sum;
	return res;
}

float sum(myVector m,const int d){
	float res = 0;
	for(int i = 0;i < d;i++){
		res += m[i];
	}
	return res / d;
}

void process(int mean,int n,const int d,const int indexOfFile,const int lastFile){
	for(int index = 0;index <= indexOfFile;index++){
		string fileName = "pmnistFiles/file";
		
		stringstream s0;
		s0 << index;
		
		fileName += s0.str();
//		fileName += ".txt";
		myVector* m = myRead(index,n,d,indexOfFile,lastFile);
		
		if(m == nullptr) {
			printf("No vector");
			exit(1);
		}
		
		if (index == indexOfFile) 
			n = lastFile;
		for(int no = 0;no < n;no++){
			float res = sum(m[no],d);
			if(res == 0)
				continue;
			for(int i = 0;i < d;i++){
				m[no][i] *= (float)(mean / res);
			}
		}
//		printf("test");
		myWrite(fileName,m,n,d);
	}
}
#endif
