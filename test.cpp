#include"matrix.h"
#include"myFileIO.h"
#include"myFileProcess.h" 
#include<iostream>
// mnist n = 20, dimension = 784
int main(){
	int n, dimension;
	findDimension("mnist/mnist",&n,&dimension);
	
	cout << "n:" << n << " dimension:" << dimension << endl;
//	myMatrix matrix1 = create_Gaussian_matrix(32, dimension);
	
	myMatrix matrix1 = create_fly_matrix(10 * dimension, dimension);
	
	cout << "create successfully" << endl;
	myMatrix matrix2 =  myRead(0, n, dimension, 100, 1);

	myMatrix result = product(matrix1, matrix2, 10 * dimension, n, 1);
	cout << "result dimension" << result[0].getDimension();
	/*
	for(int i = 0; i < n; i++){
		for(int j = 0; j < 20; j++){
			cout << result[i][j] << ' ';
		}
	}
	*/
}
