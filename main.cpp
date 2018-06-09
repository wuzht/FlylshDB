#include <iostream>
#include"myFileIO.h"
#include"myFileProcess.h"

using namespace std;

#define NUMOFVECINMNIST 60000
#define NUMOFVECINGLOVE 2196017
#define MEAN 100

void preProcess(int mean) {
	int lastIndexOfFiles = 0;
	int n = 0, d = 0;
	int index = -1, sumVec = 0;	
	
	findDimension("mnist/mnist", &n, &d);
	lastIndexOfFiles = 2999;
	int vecNumOfLastFile = NUMOFVECINMNIST - n * lastIndexOfFiles;
	process(mean, n, d, lastIndexOfFiles, vecNumOfLastFile);
}

int main(void) {
	//preProcess(MEAN);
	process(100, 20, 784, 2999, 20);
	return 0;
}

