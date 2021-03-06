#include "stdafx.h"
#include "specialmat.h"
#include <algorithm>

diagMatrix::diagMatrix(int r, double d):Matrix(r, r){
	for (int i = 0; i < r; i++) {
		this->set(i, i, d);
	}
}

diagMatrix::diagMatrix(std::vector<double> vec):Matrix(vec.size(), vec.size()){
	for (size_t i = 0; i < vec.size(); i++) {
		this->set(i, i, vec[i]);
	}
}


onesMatrix::onesMatrix(int r, int c):Matrix(r, c, 1.0){
}

zerosMatrix::zerosMatrix(int r, int c):Matrix(r, c, 0.0){
}

unitMatrix::unitMatrix(int r): diagMatrix(r, 1.0){
}

randomMatrix::randomMatrix(int r, int c):Matrix(r, c){
	// C++11 new feature
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	std::for_each(this->begin(), this->end(), [&](double& d) {
		d = dist(mt);
	});
}
