#include "mAP.h"

//file name
#define ORIGNAL_FILE "files/file"
#define PROCESSED_FILE "pmnistFiles/file"
#define Gaussian_FILE "GaussHashedVectors/file"
#define FLY_FILE "FlyHashedVectors/file"
#define BINARY_FILE "BinaryVectors/file"
#define WTA_FILE "WTAVectors/file"
#define RANDOM_FILE "RandomVectors/file"
#define MNIST_FILE "mnist/mnist"

int main(void) {

	printf("the mAP of orignal file and Gaussian file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_GAUSS, ORIGNAL_FILE, Gaussian_FILE));
	printf("the mAP of orignal file and Fly file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_FLY, ORIGNAL_FILE, FLY_FILE));
	printf("the mAP of orignal file and random file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_RANDOM, ORIGNAL_FILE, RANDOM_FILE));
	printf("the mAP of orignal file and WTA file: %lf\n", mAP(DIMENSION_OF_ORIGNAL,  DIMENSION_OF_RANDOM, ORIGNAL_FILE, WTA_FILE));
	printf("the mAP of orignal file and binary file: %lf\n", mAP(DIMENSION_OF_ORIGNAL,  DIMENSION_OF_RANDOM, ORIGNAL_FILE, BINARY_FILE));

	printf("\n");

	printf("the mAP of preprocess file and Gaussian file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_GAUSS, PROCESSED_FILE, Gaussian_FILE));
	printf("the mAP of preprocess file and Fly file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_FLY, PROCESSED_FILE, FLY_FILE));
	printf("the mAP of preprocess file and random file: %lf\n", mAP(DIMENSION_OF_ORIGNAL, DIMENSION_OF_RANDOM, PROCESSED_FILE, RANDOM_FILE));
	printf("the mAP of preprocess file and WTA file: %lf\n", mAP(DIMENSION_OF_ORIGNAL,  DIMENSION_OF_RANDOM, PROCESSED_FILE, WTA_FILE));
	printf("the mAP of preprocess file and binary file: %lf\n", mAP(DIMENSION_OF_ORIGNAL,  DIMENSION_OF_RANDOM, PROCESSED_FILE, BINARY_FILE));

	return 0;
}
