#pragma once
#include "mymatrix.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <ppl.h>

namespace Matlab{

	/*
		Element-wise Operations
	*/
	Matrix abs(const Matrix&);
	Matrix log(const Matrix&);
	Matrix exp(const Matrix&);
	Matrix square(const Matrix&);
	Matrix power(const Matrix&, double);
	Matrix sqrt(const Matrix&);
	std::vector<double> _exact_multiply(const Matrix & left, const Matrix & right);
	Matrix _one_multiply_many(const Matrix & left, const Matrix & right);
	Matrix multiply(const Matrix&, const Matrix&);
	Matrix plus(const Matrix&, const Matrix&);
	Matrix minus(const Matrix&, const Matrix&);
	Matrix divide(const Matrix&, const Matrix&);
	bool isPositive(const Matrix&);
	double dotProduct(const Matrix&, const Matrix&);

	/*
		Statistic Operations
	*/
	Matrix max(const Matrix&, int);
	double max(const Matrix&);
	Matrix min(const Matrix&, int);
	double min(const Matrix&);
	Matrix median(const Matrix&, int);

	double mean(const Matrix&);
	Matrix mean(const Matrix&, int);
	double var(const Matrix&);
	Matrix var(const Matrix&, int);
	// standard error
	double stderror(const Matrix&);
	Matrix stderror(const Matrix&, int);
	// covariance matrix
	Matrix covar(const Matrix&);
	
	/*
		Linear Algebra 
	*/
	double norm1(const Matrix&);
	double norm2(const Matrix&);
	double FrobeniusNorm(const Matrix&);

	// Solve matrix equation
	Matrix solve(const Matrix&, const Matrix&);
	Matrix concatenate(const Matrix&, const Matrix&, int);
	
	// Eigen decomposition
	std::tuple<Matrix> eigenDecompose(const Matrix&);

	// Single value decomposition
	std::tuple<Matrix> SVD(const Matrix&);

	// MP-Inverse
	Matrix pseudoInverse(const Matrix&);

	/*
		File I/O
	*/
	Matrix read(const char*);
	void write(const Matrix&, const char*);
}