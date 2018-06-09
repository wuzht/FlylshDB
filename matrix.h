#include<stdio.h> 
#include"myVector.h"
#include"stdlib.h"
#include<time.h>
#include<math.h>
#define PI 3.141592654
#define PROBABILITY 0.1
typedef myVector* myMatrix;

myMatrix create_Gaussian_matrix(int n ,int dimension ){
	srand(time(0));
	int i, j;
	myMatrix matrix = new myVector[n];
	for(i = 0; i < n; i++){
		matrix[i].newForPoint(dimension);
	}
	
	for(i = 0; i < n; i++){
		for(j = 0; j < dimension; j++){
			double u1, u2;
			u1 = (rand() + 1)/((double)RAND_MAX + 2);
			u2 = (rand() + 1)/((double)RAND_MAX + 2);
			matrix[i][j] = (float)sqrt(-2.0 * log(u1))* sin(2.0 * PI * u2);
		} 
	}
	return matrix;
}

myMatrix create_fly_matrix(int n, int dimension){
	srand(time(0));
	int num = PROBABILITY * dimension;
	int i, j, k;
	
	myMatrix matrix = new myVector[n];
	for(i = 0; i < n; i++){
		matrix[i].newForPoint(num);
	}		
	for(i = 0; i < n; i++){
		int arr[dimension];
		int index;
		for(j = 0; j < dimension; j++){
			arr[j] = j;
		}
		for(j = 0; j < dimension; j++){
			index = rand() % (dimension - j) + j;
			int tem = arr[j];
			arr[j] = arr[index];
			arr[index] = tem;
		}
		for(j = 0; j < num; j++){
			matrix[i][j] = arr[j];
		}
	}
	return matrix;	
}
//matrix1 投影矩阵
//matrix2 输入矩阵
//n 投影矩阵的行 
//num 该页有多少个向量 ，即输入矩阵的行 
//type 0 高斯矩阵 type 1 果蝇矩阵 
//生成的矩阵为num * n 
myMatrix product(myMatrix matrix1, myMatrix matrix2, int n, int num, int type){
	int d = matrix1[0].getDimension();
	int i, j, k;
	myMatrix matrix = new myVector[num];
	for(i = 0; i < num; i++){
		matrix[i].newForPoint(n);

	}
	//投影矩阵
	if(type == 0){
		for(i = 0; i < num; i++){
			for(j = 0; j < n; j++){
				float tem = 0;
				for(k = 0; k < d; k++){
					tem += matrix1[j][k] * matrix2[i][k];
				}
				matrix[i][j] = tem;
			}
		}
	}
	//果蝇矩阵 
	else{
		for(i = 0; i < num; i++){
			for(j = 0; j < n; j++){
				float tem = 0;
				for(k = 0; k < d; k++){
					tem += matrix2[i][matrix1[j][k]];
				}
				matrix[i][j] = tem;
			}
		}
	}
	
	for(i = 0; i < num; i++){
		matrix[i].setId(matrix2[i].getId());
	}
	
	return matrix;
}



