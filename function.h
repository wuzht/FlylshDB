#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdlib.h>
#include <time.h>
#include <cmath>
//calculate the distance between vec_1 and vec_2
//vec_1, first vector
//vec_2, second vector
//dim, the dimension of the vector
float nn(float* vec_1, float* vec_2, int dim) {
	float dis = 0;
	for(int i = 0; i < dim; i++) {
		float x = (vec_1[i] - vec_2[i]) * (vec_1[i] - vec_2[i]);
		dis += x;
	}
	return sqrt(dis);
}
//return a random number, whose range is from 0 to maxNumber - 1
int xrand(int maxNumber) {

	
	return (int)(((1.0 * rand()) / RAND_MAX) * maxNumber);
}

//find the page number and the index
void findPage(int &page, int &index, int id, int vecNumberOfEachPage) {
	page = id/vecNumberOfEachPage;
	index = id % vecNumberOfEachPage;
}

#endif
