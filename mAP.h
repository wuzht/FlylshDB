#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <fstream>
#include <sstream>
#include"myFileIO.h"
#include "function.h"
#include "List.h"
#include "myVector.h"
#include <time.h>

using namespace std;

#define TOTAL_NUMBER_OF_VECTOR 60000
#define DIMENSION_OF_GAUSS 2
#define DIMENSION_OF_FLY 7840
#define DIMENSION_OF_ORIGNAL 784
#define NUMBEROFFILE 3000 
#define RUNTIMES 1000
#define K 200
#define DIMENSION_OF_RANDOM 2
#define VECTOR_NUMBER_IN_EACHFILE 20

//the in method and get method have been created in class List
//find the ap of two knns
//groud_truth : the true knn
//approximate : the knn after map
//@return : the ap of these two knn
double ap(List* ground_truth, List* approximate) {
	double truth = 0;
	int k = ground_truth->size();
	double precision = 0;
	for(int i = 0; i < k; i++) {
		if(ground_truth->in(approximate->get(i))) {
			truth++;
			
			precision += truth / (i + 1);
//			printf("%lf\n", precision);
		}
//		else {
//			printf("--1--\n");
//		}
	}
	return precision / k;
}

// //has created the copy construtor of myVector
// //get the vector by its id
// //randomID : the id of the vector
// //n : the number of vector in one page
// //d : the dimension of the vector
// //fileName : the file which contains the vector
// //@return : the vector whose id is randomId
// myVector getAVector(int randomID, int n, int d, const char* fileName) {
	

// 	return randVec;
// }

//find k nearest neighbor
//randomID : the id of the vector
//k : the number of neighbors needed to find
//spendTime : the time spending to find the neighbors
//fileName ： the name of file which contains the vectors
//file_to_find_dim : a file used to find the dimension of the vector
//@return ： a list which contains the k nearest neighbors of the vector
List* KNN(int dim ,int randomID ,int k, double& spendTime, const char* fileName) {	
	int n = VECTOR_NUMBER_IN_EACHFILE;
	int d = dim;
	int index = 0, sumVec = 0;

	int lastIndexOfFiles = 0;
	lastIndexOfFiles = NUMBEROFFILE - 1;
	int vecNumOfLastFile = TOTAL_NUMBER_OF_VECTOR - n * lastIndexOfFiles;
	int page;
	//find the page index of the random vector
	findPage(page, index, randomID, n);

	//read the random vector
	myVector *randPageVec = myRead(page, n, d, lastIndexOfFiles,  vecNumOfLastFile, fileName);

	myVector randVec = randPageVec[index];

	//printf("random vector:%d\n", randVec.getId());
	
	//set up the list
	List* nearestNeibor = new List(k);
	//count the wasting time
	clock_t start, finish;
	start = clock();
	//find the knn
	for(int i = 0; i <= lastIndexOfFiles; i++) {
		//read each page
		myVector* v = myRead(i, n, d, lastIndexOfFiles, vecNumOfLastFile, fileName);
		int v_size = i == lastIndexOfFiles ? vecNumOfLastFile : n;
		for(int j = 0; j < v_size; j++) {
			//calculte all the distance from the vector to the others
			if(v[j].getId() != randVec.getId()) {
				float dist = nn(randVec.getVector(), v[j].getVector(), d);
				nearestNeibor->push(v[j].getId(), dist);
			}
		}
		//release the memory
		if(v != nullptr) {
			delete[] v;
		} 
	}
	finish = clock();
	//calculte the spending time
	spendTime = (double)(finish - start)/CLOCKS_PER_SEC;

	if(randPageVec != nullptr) delete[] randPageVec;

	return nearestNeibor;
}

//calculate the mAP of two groups of vectors
//file1 : the file name of the original file or preprocessing file
//file2 : the file name of the file after mapping
//file_to_fine_dim : a file used to calculate the dimension of the vector
//@return : the mAP of these two group of vectors
double mAP(int dim1, int dim2, const char* file1, const char* file2) {
	double meanAP = 0;
	double totalSpendTime = 0;
	srand((int)time(0));
	for(int i = 0; i < RUNTIMES; i++) {
		if (i % 20 == 0) {
			printf("%d ", i);
		}
		//find the random vector	
		int randomID = xrand(TOTAL_NUMBER_OF_VECTOR);
		double spendTime1 = 0;
		double spendTime2 = 0;
		List* knn1 = KNN(dim1,randomID , K, spendTime1, file1);
		List* knn2 = KNN(dim2,randomID , K, spendTime2, file2);
		
//		knn1->print();
//		knn2->print(); 
		
		meanAP += ap(knn1, knn2);
		//printf("%lf\n", meanAP);
		if(knn1) delete knn1;
		if(knn2) delete knn2;
	}	
	printf("\n");
	return (meanAP / RUNTIMES);
}
#endif
