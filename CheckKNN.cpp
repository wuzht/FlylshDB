#include <iostream>
#include <fstream>
#include <sstream>
#include"myFileIO.h"
#include"myFileProcess.h"
#include "function.h"
#include "List.h"
#include <time.h>

using namespace std;

#define NUMOFVECINMNIST 60000
#define NUMOFVECINGLOVE 2196017
#define NUMBEROFFILE 3000 
#define RUNTIMES 100
#define K 200

using namespace std;
double averageTime = 0;
 // 从源文件中读写数据，已完成 
void readFileFromSource(const char* fileName) {
	int lastIndexOfFiles = 0;
	int n = 0, d = 0;
	int index = 0, sumVec = 0;
	
	findDimension(fileName, &n, &d);
	//lastIndexOfFiles = NUMBEROFFILE - 1;
	int vecNumOfLastFile = NUMOFVECINMNIST - n * lastIndexOfFiles;

	fileIO(fileName, n, d, &index, &sumVec);
}

//KNN in one time
//k, find k nearest neighbor
void oneKNN(int k) {
	int lastIndexOfFiles = 0;
	int n = -1, d = 0;
	int index = 0, sumVec = 0;

	//deal with the parameters
	findDimension("mnist/mnist", &n, &d);
	lastIndexOfFiles = NUMBEROFFILE - 1;
	int vecNumOfLastFile = NUMOFVECINMNIST - n * lastIndexOfFiles;
	
	//find the random vector
	srand((int)time(0));
	int randomID = xrand(NUMOFVECINMNIST);
//	cout << randomID << endl;
	int page;
	//find the page index of the random vector
	findPage(page, index, randomID, n);

	//read the random vector
	myVector *randPageVec = myRead(page, n, d, lastIndexOfFiles,  vecNumOfLastFile, "files/file");

	myVector randVec = randPageVec[index];

	printf("random vector:%d\n", randVec.getId());
	
	//set up the list
	List nearestNeibor = List(k);
	//count the wasting time
	clock_t start, finish;
	start = clock();
	//find the knn
	for(int i = 0; i <= lastIndexOfFiles; i++) {
		//read each page
		myVector* v = myRead(i, n, d, lastIndexOfFiles, vecNumOfLastFile);
		int v_size = i == lastIndexOfFiles ? vecNumOfLastFile : n;
		for(int j = 0; j < v_size; j++) {
			//calculte all the distance from the vector to the others
			if(v[j].getId() != randVec.getId()) {
				float dist = nn(randVec.getVector(), v[j].getVector(), d);
				nearestNeibor.push(v[j].getId(), dist);
			}
		}
		//release the memory
		if(v != nullptr) {
			delete[] v;
		} 
	}
	finish = clock();
	//calculte the spending time
	double totalTime = (double)(finish - start)/CLOCKS_PER_SEC;
	printf("Spending time:%lf seconds\n", totalTime); 
	
	nearestNeibor.print();
	printf("%d\n", nearestNeibor.size());
}

//KNN in one thousand time
//k, find k neareast neighbors
//fileName, the name of the file that the result will be written in
void thousandKNN(int k, const char* fileName) {
	
	int lastIndexOfFiles = 0;
	int n = 0, d = 0;
	int index = -1, sumVec = 0;

	findDimension("mnist/mnist", &n, &d);
	lastIndexOfFiles = NUMBEROFFILE - 1;
	int vecNumOfLastFile = NUMOFVECINMNIST - n * lastIndexOfFiles;
	
	int times = RUNTIMES;
	double sumTime = 0;
	
	ofstream file;
	file.open(fileName);
	if(file == nullptr) {
		printf("open KNNresult.txt failed!\n");
		exit(1); 
	}
	srand((int)time(0));
	while(times--) {
		//find the random vector
		int randomID = xrand(NUMOFVECINMNIST);
		int page;
		findPage(page, index, randomID, n);
		myVector *randPageVec = myRead(page, n, d, lastIndexOfFiles, vecNumOfLastFile);
		myVector randVec = randPageVec[index];
		file << "random vector:" << randVec.getId() << '\n';
	
		//set up the list
		List nearestNeibor = List(k);
		clock_t start, finish;
		start = clock();
		//find the knn
		for(int i = 0; i <= lastIndexOfFiles; i++) {
			myVector* v = myRead(i, n, d, lastIndexOfFiles, vecNumOfLastFile);
			int v_size = i == lastIndexOfFiles ? vecNumOfLastFile : n;
			for(int j = 0; j < v_size; j++) {
				if(v[j].getId() != randVec.getId()) {
					float dist = nn(randVec.getVector(), v[j].getVector(), d);
					nearestNeibor.push(v[j].getId(), dist);
				}
			}
			if(v != nullptr) {
				delete[] v;
			} 
		}
		finish = clock();
		double totalTime = (double)(finish - start)/CLOCKS_PER_SEC;
		sumTime += totalTime;
		file << "Spending time:" << totalTime << '\n';
		
		nearestNeibor.print_in_file(file);
	}
	//calculate the average spending time
	file << "The average time is : " << sumTime / RUNTIMES << endl; 
	averageTime += sumTime / RUNTIMES;
//	cout << "The average time is : " << sumTime / RUNTIMES << endl; 

	file.close();
}

int main() {
//	readFileFromSource("mnist/mnist");
		
	oneKNN(K);
	/*
	for(int i = 0; i < 5; i++){
		string fileName = "pResultOfKNN/pResultOfKNN";
		stringstream s0;
		s0 << i;
		fileName += s0.str();
		fileName += ".txt";
		thousandKNN(K, fileName.data());
	}
	
	cout << "averageTime:" << averageTime / 5;*/

//	thousandKNN(K, "ResultOfKNN.txt");
	return 0;
}
