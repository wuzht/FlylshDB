#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <iostream>
using namespace std;
//a class used to store the vector
class myVector {
	public:
		myVector(){
			point = nullptr;
			dimension = 0;
			id = 0;
		}
		
		myVector(int dim) {
			dimension = dim;
			point = new float[dim];
		}
		
		myVector(int id, int dim) {
			this->id = id;
			dimension = dim;
			point = new float[dim];
		}
	
		myVector(myVector& other) {
			id = other.id;
			dimension = other.dimension;
			if(other.point != nullptr) {
				point = new float[dimension];
				for(int i = 0; i < dimension; i++) {
					point[i] = other.point[i];
				}
			}
		}

		~myVector() {
			if(point != nullptr) {
				delete[] point;
			}
		}
	
		void setId(int id){
			this->id = id;
		}
		
		int getId() {
			return id;
		}
		
		int getDimension() {
			return dimension;
		}
		
		bool newForPoint(const int dim) {
			dimension = dim;
			if (point == nullptr) {
				
				point = new float[dim];
							
				return true;
			}
			else {
				delete[] point;
				point = new float[dim];
				return true;
			}
			return false;
		}
		
		bool setVector(float* vec) {
			if(vec == nullptr) return false;
			for(int i = 0; i < dimension; i++) {
				point[i] = vec[i];
			}
			return true;
		}
		
		float* getVector() {
			return point;
		}
		
		float& operator[](int i) {
			return point[i];
		}

		myVector& operator=(myVector &src) {
			if (point != nullptr) delete[] point;
			
			this -> id = src.getId();
			this -> dimension = src.getDimension();
			
			point = new float[dimension];
			for (int i = 0; i < dimension; i ++) {
				point[i] = src.getVector()[i];
			}
		}

		void printVec() {
			cout << "VEC " << id << ": ";
			for (int i = 0; i < dimension; i++) {
				cout << point[i] << " ";
			}
			cout << endl << endl;
		}
		
	private:
		int id;
		int dimension;
		float* point;
};

#endif
