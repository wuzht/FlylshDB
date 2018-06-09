#ifndef MYPAGE_H
#define MYPAGE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "myVector.h"

#define BUFFER_SIZE 50

using namespace std;

class myBuffer {
	public:
		myBuffer() {
			vec = new myVector[20];
			for (int j = 0; j < 20; j ++) {
				vec[j].newForPoint(784);
			}
			slot = new int[20];
			numOfSlots = 20;
			dimension = 784;
			indexOfFile = 0;
		}
		
		myBuffer(int n, int d) {
			
			vec= new myVector[n];
	
			for (int j = 0; j < n; j ++) {
				vec[j].newForPoint(d);
			}
			
			slot = new int[n];
			numOfSlots = n;
			dimension = d;
			indexOfFile = 0;
		}
		
		~myBuffer() {
			delete[] slot;
			
		}
		
		int getNumOfSlots() {
			return numOfSlots;
		}
		
		

		int getIndexOfFile() {
			return indexOfFile;
		}
		
		void setIndexOfFile(int indexOfFile) {
			this -> indexOfFile = indexOfFile;
		}
		
		int getDimension() {
			return dimension;
		}
		
		void setDimension(int d) {
			dimension = d;
		}
		
		void setnumOfSlots(int n) {
			numOfSlots = n;
		}
		
		void setSlots(int *slot, int n) {
			for (int i = 0; i < n; i ++) {
				this -> slot[i] = slot[i];
			}
		}
		
		myVector &getMyVector(int i) {
			return this -> vec[i];
		}
		
		bool setMyVectors(myVector *v) {
			for (int i = 0; i < numOfSlots; i ++) {
				vec[i] = v[i];
			}
			return true;
		}
		
		myBuffer& operator=(myBuffer &src) {
			int len = src.getNumOfSlots();
			
			vec= new myVector[len];
	
			for (int j = 0; j < len; j ++) {
				vec[j].newForPoint(src.getDimension());
			}
			
			slot = new int[len];
			numOfSlots = len;
			indexOfFile = 0;
			
			for (int i = 0; i < len; i ++) {
				vec[i] = src.getMyVector(i);
			}
		}
		
	private:

		myVector *vec;
		int *slot;
		int numOfSlots;
		int indexOfFile;
		int dimension;
};

#endif
