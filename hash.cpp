#include "matrix.h"
#include "myFileIO.h"
#include "myFileProcess.h" 
#include "WTAList.h"
#include <iostream>
// mnist n = 20, dimension = 784
#define k_GAUSS_OUTPUT_VEC_DIM 2
#define k_OUTPUT_VEC_DIM k_GAUSS_OUTPUT_VEC_DIM

#define m_FLY_OUTPUT_VEC_DIM 7840
#define TYPE_GAUSS 0
#define TYPE_FLY 1
#define NUMOFVECINMNIST 60000

/**
 * @param matrix: the matrix to save
 * @param dim: the dimension (784)
 * @param type: the maxtix type (Gauss 0 or Fly 1)
 */
void saveMatrix(const myMatrix matrix, int dim, const int type) {	
	int n = findN(dim);
	int ouputVecDim = k_GAUSS_OUTPUT_VEC_DIM;
	string folderName = "GaussMatrix/";
		
	if (type == TYPE_FLY) {
		dim = (int)(PROBABILITY * dim);
		n = findN(dim) - 2;		// not doing this would make the files greater than 64k
		ouputVecDim = m_FLY_OUTPUT_VEC_DIM;
		folderName = "FlyMatrix/";
	}

	const int indexOfFile = (ouputVecDim - 1) / n;

	for (int index = 0; index <= indexOfFile; index++) {
		string fileName = folderName + "file";
		stringstream s0;
		s0 << index;

		fileName += s0.str();

		int numOfVecInFile = n;
		if (index == indexOfFile)
			numOfVecInFile = ouputVecDim % n;

		myWrite(fileName, matrix + n * index, numOfVecInFile, dim);
	}
}

/**
 * @param dim: the dimension (784)
 * @param type: the maxtix type (gauss 0 or fly 1)
 */
myMatrix readMatrix(int dim, const int type) {
	int n = findN(dim);
	int ouputVecDim = k_GAUSS_OUTPUT_VEC_DIM;
	string folderName = "GaussMatrix/";
		
	if (type == TYPE_FLY) {
		dim = (int)(PROBABILITY * dim);
		n = findN(dim) - 2;		// not doing this would make the files greater than 64k
		ouputVecDim = m_FLY_OUTPUT_VEC_DIM;
		folderName = "FlyMatrix/";
	}

	myMatrix matrix = new myVector[ouputVecDim];

	for(int i = 0; i < ouputVecDim; i++){
		matrix[i].newForPoint(dim);
	}

	const int indexOfFile = (ouputVecDim - 1) / n;

	for (int index = 0; index <= indexOfFile; index++) {
		int numOfVecInFile = n;
		if (index == indexOfFile)
			numOfVecInFile = ouputVecDim % n;

		myVector* vecs = myRead(index, n, dim, indexOfFile, ouputVecDim % n, folderName + "file");

		for (int i = 0; i < numOfVecInFile; i++) {
			matrix[i + n * index] = vecs[i];
		}
		if (vecs != nullptr) {
			delete []vecs;
			vecs = nullptr;
		}
	}
	return matrix;
}

/**
 * Generate the Gauss or Fly Matrix And Save (Gauss or Fly) Hashed Vectors after product
 * Be sure that you've created the corresponding folders first.
 * Notice that each page of the file stores n (20) vectors,
 * the dimension of GaussHashedVector is k (32),
 * the dimension of FlyHashedVector is m = 10 * d (7840).
 * @param type: the maxtix type (Gauss 0 or Fly 1)
 */
void generateMatrixAndSaveHashedVectorsAfterProduct(int type) {
	int errCount = 0;

	int n, dimension;
	findDimension("mnist/mnist", &n, &dimension);
	string folderName = "GaussHashedVectors/";
	if (type == TYPE_FLY)
		folderName = "FlyHashedVectors/";

	cout << "n:" << n << " dimension:" << dimension << endl;
	
	myMatrix matrix1;
	if (type == TYPE_GAUSS) {
		matrix1 = create_Gaussian_matrix(k_GAUSS_OUTPUT_VEC_DIM, dimension);
	}
	else {
		matrix1 = create_fly_matrix(m_FLY_OUTPUT_VEC_DIM, dimension);	
	}
	saveMatrix(matrix1, dimension, type);
	myMatrix matrixRead = readMatrix(dimension, type);

	// assert equal
	const int ouputVecDim = (type == TYPE_GAUSS) ? k_GAUSS_OUTPUT_VEC_DIM : m_FLY_OUTPUT_VEC_DIM;
	const int dim = (type == TYPE_GAUSS) ? dimension : (int)(PROBABILITY * dimension);
	for (int i = 0; i < ouputVecDim; i++) {
		for(int j = 0; j < dim; j++) {
			if (matrix1[i][j] != matrixRead[i][j]) {
				cout << "[MY IO ERROR] hash.cpp: In matrix(type" << type << ") "
				<< i << " " << j << " FALSE!" << endl;
				errCount++;
				break;
			}
		}
	}
	cout << "Matrix(type" << type <<") create successfully" << endl;


	//*******************************************
	cout << "[Hashing] Please wait, this could take a few minutes..." << endl << "Processed File Num: ";
	const int indexOfFile = 2999;
	const int vecNumOfLastFile = NUMOFVECINMNIST - n * indexOfFile;
	for (int index = 0; index <= indexOfFile; index++) {
		myMatrix matrix2 =  myRead(index, n, dimension, indexOfFile, vecNumOfLastFile, "files/file");
		myMatrix result = product(matrixRead, matrix2, ouputVecDim, n, type);
		//cout << index << "result dimension" << result[0].getDimension() << endl;
		for (int i = 0; i < n; i++) {
			result[i].setId(matrix2[i].getId());
			cout << result[i].getId() << " ";
		}
		cout << endl;
//		if (index % 100 == 0)
//			cout << index << " ";

		string fileName = folderName + "file";
		stringstream s0;
		s0 << index;
		fileName += s0.str();

		int numOfVecInFile = n;
		if (index == indexOfFile)
			numOfVecInFile = vecNumOfLastFile;
		
		myWrite(fileName, result, numOfVecInFile, ouputVecDim);
		myMatrix vecsRead = myRead(index, n, ouputVecDim, indexOfFile, vecNumOfLastFile, folderName + "file");
		
		// assert equal
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < ouputVecDim; j++) {
				if (result[i][j] != vecsRead[i][j]) {
					cout << "[MY IO ERROR] hash.cpp: In result vecs" << endl;
					errCount++;
					break;
				}
			}
		}
		if (matrix2 != nullptr) {
			delete []matrix2;
			matrix2 = nullptr;
		}
		if (result != nullptr) {
			delete []result;
			result = nullptr;
		}
		if (vecsRead != nullptr) {
			delete []vecsRead;
			vecsRead = nullptr;
		}
	}	

	if (matrix1 != nullptr) {
		delete []matrix1;
		matrix1 = nullptr;
	}
	if (matrixRead != nullptr) {
		delete []matrixRead;
		matrixRead = nullptr;
	}
	if (errCount != 0)
		cout << "\n[Hash] ERROR errCount: " << errCount << endl;
	else
		cout << "\n[Hash] Succeed!" << endl;
}

void processRandom(const int k) {
	int errCount = 0;
	int dim = m_FLY_OUTPUT_VEC_DIM;
	int arr[m_FLY_OUTPUT_VEC_DIM];
	for (int i = 0; i < dim; i++) {
		arr[i] = i;
	}
	srand(time(0));
	for(int j = 0; j < dim; j++){
		int index = rand() % (dim - j) + j;
		int tem = arr[j];
		arr[j] = arr[index];
		arr[index] = tem;
	}

	int n, dimension;
	findDimension("mnist/mnist", &n, &dimension);
	const int indexOfFile = 2999;
	const int vecNumOfLastFile = NUMOFVECINMNIST - n * indexOfFile;
	string srcFolderName = "FlyHashedVectors/";
	for (int index = 0; index <= indexOfFile; index++) {
		int numOfVecInFile = n;
		if (index == indexOfFile)
			numOfVecInFile = vecNumOfLastFile;

		myMatrix result = new myVector[numOfVecInFile];
		for (int i = 0; i < numOfVecInFile; i++) {
			result[i].newForPoint(k);
			
		}
		
		myMatrix vecsRead = myRead(index, numOfVecInFile, dim, indexOfFile, vecNumOfLastFile, srcFolderName + "file");
		for (int i = 0; i < numOfVecInFile; i++) {
			result[i].setId(vecsRead[i].getId());
		}
		for (int i = 0; i < numOfVecInFile; i++) {
			for (int j = 0; j < k; j++) {
				result[i][j] = vecsRead[i][arr[j]];
			}
		}
		
		
		string folderName = "RandomVectors/";
		string fileName = folderName + "file";
		stringstream s0;
		s0 << index;
		fileName += s0.str();
		myWrite(fileName, result, numOfVecInFile, k);
		myMatrix randVecsRead = myRead(index, numOfVecInFile, k, indexOfFile, vecNumOfLastFile, folderName + "file");

		// assert equal
		for (int i = 0; i < numOfVecInFile; i++) {
			for (int j = 0; j < k; j++) {
				if (result[i][j] != randVecsRead[i][j]) {
					cout << "[MY IO ERROR] hash.cpp: In result vecs" << endl;
					errCount++;
					break;
				}
			}	
		}
		
		if (index % 500 == 0) {
			for (int i = 0; i < k; i++) {
				cout << result[0][i] << " ";
			}
			cout << endl;
			for (int i = 0; i < k; i++) {
				cout << randVecsRead[0][i] << " ";
			}
			cout << endl;
		}


		if (result) {
			delete []result;
			result = nullptr;
		}
		if (vecsRead) {
			delete []vecsRead;
			vecsRead = nullptr;
		}
		if (randVecsRead) {
			delete []randVecsRead;
			randVecsRead = nullptr;
		}
	}
	if (errCount != 0)
		cout << "\n[processRandom] ERROR errCount: " << errCount << endl;
	else
		cout << "\n[processRandom] Succeed!" << endl;
}

void processWTAandBinary(int k) {
	
	
	int errCount = 0;
	int dim = m_FLY_OUTPUT_VEC_DIM;
	int n, dimension;
	findDimension("mnist/mnist", &n, &dimension);
	const int indexOfFile = 2999;
	const int vecNumOfLastFile = NUMOFVECINMNIST - n * indexOfFile;
	string srcFolderName = "FlyHashedVectors/";

	cout << "[processWTAandBinary] Please wait, this could take a few minutes..." << endl;
	for (int index = 0; index <= indexOfFile; index++) {
		int numOfVecInFile = n;
		if (index == indexOfFile)
			numOfVecInFile = vecNumOfLastFile;

		myMatrix binaryResult = new myVector[numOfVecInFile];
		myMatrix wtaResult = new myVector[numOfVecInFile];

		for (int i = 0; i < numOfVecInFile; i++) {
			binaryResult[i].newForPoint(k);
			wtaResult[i].newForPoint(2 * k);
		}
		
		myMatrix vecsRead = myRead(index, numOfVecInFile, dim, indexOfFile, vecNumOfLastFile, srcFolderName + "file");
		//vecsRead[0].printVec();
		
		for (int i = 0; i < numOfVecInFile; i++) {
			binaryResult[i].setId(vecsRead[i].getId());
			wtaResult[i].setId(vecsRead[i].getId());
		}

		for (int i = 0; i < numOfVecInFile; i++) {
			WTAList listForWTA(k);
			for (int j = 0; j < dim; j++) {
				listForWTA.push(j, vecsRead[i][j]);
			}
			for (int j = 0; j < k; j++) {
				float valueIndex = listForWTA.pop();
				binaryResult[i][j] = valueIndex;
				binaryResult[i].setId(vecsRead[i].getId());
				wtaResult[i][j] = valueIndex;
				wtaResult[i][j + k] = vecsRead[i][valueIndex];
				wtaResult[i].setId(vecsRead[i].getId());
			}	
		}
		
		string binaryFolderName = "BinaryVectors/";
		string wtaFolderName = "WTAVectors/";

		string binaryFileName = binaryFolderName + "file";
		string wtaFileName = wtaFolderName + "file";
		stringstream s0;
		s0 << index;
		binaryFileName += s0.str();
		wtaFileName += s0.str();

		// binary
		myWrite(binaryFileName, binaryResult, numOfVecInFile, k);
		myMatrix binVecsRead = myRead(index, numOfVecInFile, k, indexOfFile, vecNumOfLastFile, binaryFolderName + "file");
		
		// assert equal
		for (int i = 0; i < numOfVecInFile; i++) {
			for (int j = 0; j < k; j++) {
				if (binaryResult[i][j] != binVecsRead[i][j]) {
					cout << "[MY IO ERROR] hash.cpp: In binaryResult vecs" << endl;
					errCount++;
					break;
				}
			}	
		}

		// WTA
		myWrite(wtaFileName, wtaResult, numOfVecInFile, 2 * k);
		myMatrix wtaVecsRead = myRead(index, numOfVecInFile, 2 * k, indexOfFile, vecNumOfLastFile, wtaFolderName + "file");
		if (index % 1000 == 0)
			wtaVecsRead[8].printVec();
		// assert equal
		for (int i = 0; i < numOfVecInFile; i++) {
			for (int j = 0; j < 2 * k; j++) {
				if (wtaResult[i][j] != wtaVecsRead[i][j]) {
					cout << "[MY IO ERROR] hash.cpp: In wtaResult vecs" << endl;
					errCount++;
					break;
				}
			}	
		}

		if (binaryResult) {
			delete []binaryResult;
			binaryResult = nullptr;
		}
		if (wtaResult) {
			delete []wtaResult;
			wtaResult = nullptr;
		}
		if (vecsRead) {
			delete []vecsRead;
			vecsRead = nullptr;
		}
		if (binVecsRead) {
			delete []binVecsRead;
			binVecsRead = nullptr;
		}
		if (wtaVecsRead) {
			delete []wtaVecsRead;
			wtaVecsRead = nullptr;
		}
	}
	if (errCount != 0)
		cout << "\n[processWTAandBinary] ERROR errCount: " << errCount << endl;
	else
		cout << "\n[processWTAandBinary] Succeed!" << endl;
	
}

int main(){
	//generateMatrixAndSaveHashedVectorsAfterProduct(TYPE_GAUSS);
	//generateMatrixAndSaveHashedVectorsAfterProduct(TYPE_FLY);
	processRandom(k_OUTPUT_VEC_DIM);
	processWTAandBinary(k_OUTPUT_VEC_DIM);

	/*
	myMatrix vecsRead = myRead(0, 20, k_OUTPUT_VEC_DIM, 2999, 20, "BinaryVectors/file");
	for (int i = 0; i < k_OUTPUT_VEC_DIM; i++) {
		cout << vecsRead[15][i] << " ";
	}*/
}


