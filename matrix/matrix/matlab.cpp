#include "matlab.h"
#include "mymatrix.h"
#include "stdafx.h"
#include <cmath>
// "stdafx.h" is required by VS application project.
// If you deploy the codes in other places, drop it out.

using namespace std;

/*
	Actually we want to design element-wise operations
	with parallelism but our Matrix::iterator only is 
	designed for tranversal. If we want to apply parallelism
	our itrerator has to be redesigned or make it member function
	of the Matrix class.
*/

Matrix Matlab::abs(const Matrix & m)
{
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), [&](double d) {
		return std::abs(d);
	});
	return ret;
}

Matrix Matlab::log(const Matrix & m)
{
	if (!Matlab::isPositive(m)) {
		throw new invalidParamExcep("non-positive value for log");
	}
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), std::logf);
	return ret;
}

Matrix Matlab::exp(const Matrix & m)
{
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), [&](double d) {
		return std::exp(d);
	});
	return ret;
}

Matrix Matlab::square(const Matrix & m)
{
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), [&](double d) {
		return std::pow(d, 2);
	});
	return ret;
}

Matrix Matlab::power(const Matrix & m, double dd)
{
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), [&](double d) {
		return std::pow(d, dd);
	});
	return ret;
}

Matrix Matlab::sqrt(const Matrix & m)
{
	if (!Matlab::isPositive(m)) {
		throw new invalidParamExcep("non-positive value for sqrt");
	}
	Matrix ret(m.size(0), m.size(1));
	transform(m.begin(), m.end(), ret.begin(), [&](double d) {
		return std::sqrt(d);
	});
	return ret;
}

std::vector<double> Matlab::_exact_multiply(const Matrix& left, const Matrix & right) {
	// the two matices are of the same shape: 1xm or mx1
	// return vector for calling appendRow/appendCol
	std::vector<double> ret(left.size(0) * left.size(1));
	transform(left.begin(), left.end(), right.begin(), ret.begin(), multiplies<double>());
	return ret;
}

Matrix Matlab::_one_multiply_many(const Matrix& left, const Matrix & right) {
	Matrix ret;
	if (left.size(0) == 1 && left.size(1) == right.size(1)) {
		for (int t = 0; t < right.size(0); t++) {
			ret.appendRow(_exact_multiply(right.getRow(t), left));
		}
	}
	else if (left.size(1) == 1 && left.size(0) == right.size(0)) {
		for (int t = 0; t < right.size(1); t++) {
			ret.appendCol(_exact_multiply(right.getColumn(t), left));
		}
	}
	else {
		throw new dimenDismatchExcep("Dimensions dismatch.");
	}
	return ret;
}

Matrix Matlab::multiply(const Matrix & left, const Matrix & right)
{
	Matrix ret;
	if (left.size(0) == 1 || left.size(1) == 1) {
		ret = _one_multiply_many(left, right);
	}
	else if (right.size(0) == 1 || right.size(1) == 1)
	{
		ret = _one_multiply_many(right, left);
	}
	else
	{
		throw new dimenDismatchExcep(left.size(0), left.size(1), right.size(0), right.size(1));
	}
	return ret;
}

Matrix Matlab::plus(const Matrix & left, const Matrix & right)
{
	if ((left.size(0) != right.size(0)) || (left.size(1) != right.size(1))) {
		throw new dimenDismatchExcep(left.size(0), left.size(1), right.size(0), right.size(1));
	}
	Matrix ret(left.size(0), left.size(1));
	transform(left.begin(), left.end(), right.begin(), ret.begin(), std::plus<double>());
	return ret;
}

Matrix Matlab::minus(const Matrix & left, const Matrix & right)
{
	if ((left.size(0) != right.size(0)) || (left.size(1) != right.size(1))) {
		throw new dimenDismatchExcep(left.size(0), left.size(1), right.size(0), right.size(1));
	}
	Matrix ret(left.size(0), left.size(1));
	transform(left.begin(), left.end(), right.begin(), ret.begin(), std::minus<double>());
	return ret;
}

bool Matlab::isPositive(const Matrix & m)
{
	Matrix::const_iterator iter = find_if_not(m.begin(), m.end(), [&](double d) {
		return d > 0;
	});
	return (iter == m.end());
}

double Matlab::dotProduct(const Matrix & a, const Matrix & b)
{
	return Matlab::multiply(a, b).sum();
}

Matrix Matlab::max(const Matrix & m, int sign)
{
	return m.max(sign);
}

double Matlab::max(const Matrix & m)
{
	return *(std::max_element(m.begin(), m.end()));
}

Matrix Matlab::min(const Matrix & m, int sign)
{
	return m.min(sign);
}

double Matlab::min(const Matrix & m)
{
	return *(std::min_element(m.begin(), m.end()));
}

Matrix Matlab::median(const Matrix & m, int sign)
{
	if (sign != 0 && sign != 1) {
		throw new invalidParamExcep("invalid param in Matrix::median");
	}
	Matrix sorted = m.sort(sign);
	Matrix ret;
	using namespace concurrency;
	if (sign == 0) {
		int median_index = sorted.size(0) / 2;
		if (sorted.size(0) % 2 == 0) {
			ret = (sorted.getRow(median_index) + sorted.getRow(median_index - 1)) / 2.0;
		}
		else {
			ret = sorted.getRow(median_index);
		}
	}
	else {
		int median_index = sorted.size(1) / 2;
		if (sorted.size(1) % 2 == 0) {
			ret = (sorted.getColumn(median_index) + sorted.getColumn(median_index - 1)) / 2.0;
		}
		else {
			ret = sorted.getColumn(median_index);
		}
	}
	return ret;
}

double Matlab::mean(const Matrix & m)
{
	return m.sum()/(m.size(1)*m.size(0));
}

Matrix Matlab::mean(const Matrix & m, int sign)
{
	return m.sum(sign)/m.size(sign);
}

double Matlab::var(const Matrix & m)
{
	return Matlab::mean(Matlab::square(m)) - std::pow(Matlab::mean(m), 2);
}

Matrix Matlab::var(const Matrix & m, int sign)
{
	return Matlab::mean(Matlab::square(m), sign) - std::pow(Matlab::mean(m), 2);
}

double Matlab::stderror(const Matrix & m)
{
	return std::sqrt(Matlab::var(m));
}

Matrix Matlab::stderror(const Matrix &, int)
{
	return Matrix();
}

Matrix Matlab::covar(const Matrix &)
{
	return Matrix();
}


double Matlab::norm1(const Matrix & m)
{
	return (Matlab::abs(m)).sum();
}

double Matlab::norm2(const Matrix & m)
{
	return Matlab::FrobeniusNorm(m);
}

double Matlab::FrobeniusNorm(const Matrix & m)
{
	using namespace concurrency;
	double ret = 0.0;
	parallel_for_each(m.begin(), m.end(), [&](double d) {
		ret += d * d;
	});
	return std::sqrt(ret);
}

Matrix Matlab::solve(const Matrix & A, const Matrix & b)
{
	// perform dimension check
	if (A.size(0) != b.size(0)) {
		throw new dimenDismatchExcep(A.size(0), A.size(1), b.size(0), b.size(1));
	}
	// needs optimization: compute rank first
	if (A.size(0) > A.size(1)) {
		throw new invalidParamExcep("unsolvable equation.");
	}

	// make expansion a square matrix
	Matrix expansion(Matlab::concatenate(A, b, 1));
	expansion.appendRow(vector<double>(A.size(1)+1, 0));
	expansion = expansion.elemRowOp();
	int row = A.size(0);
	int col = expansion.size(1);
	Matrix X(row, 1);
	for (int t = row - 1; t >= 0; t--) {
		double sum = 0.0;
		for (int r = t; r <= row-2; r++) {
			sum += expansion.get(t, r+1) * X.get(r+1, 0);
		}
		double tmp = (expansion.get(t, col - 1) - sum) / expansion.get(t, t);
		X.set(t, 0, tmp);
	}
	return X;
}

Matrix Matlab::concatenate(const Matrix & A, const Matrix & B, int sign = 0)
{
	if (sign != 0 && sign != 1) {
		throw new invalidParamExcep("invalid param in Matrix::concatenate()");
	}
	Matrix ret(A);
	if (sign == 0) {
		ret.appendRow(B);
	}
	else {
		ret.appendCol(B);
	}
	return ret;
}

std::tuple<Matrix> Matlab::eigenDecompose(const Matrix &)
{
	return std::tuple<Matrix>();
}

std::tuple<Matrix> Matlab::SVD(const Matrix &)
{
	return std::tuple<Matrix>();
}

Matrix Matlab::pseudoInverse(const Matrix &)
{
	return Matrix();
}

Matrix Matlab::read(const char* str)
{
	ifstream file;
	try {
		file.open(str);
	}
	catch(istream::failure e){
		std::cerr << "Exception opening file\n";
		throw new invalidParamExcep("file path error.");
	}
	string row;
	vector<double> vec;
	Matrix ret;

	while (getline(file, row)) {
		stringstream linestream(row);
		string value;
		double d;
		try{
			while (getline(linestream, value, ',')) {
				d = std::stod(value);
				vec.push_back(d);
			}
		}
		catch (std::exception e) {
			throw new MatrixException("failed to convert string to double.");
		}
		if (ret.size(0) == 0) {
		// first input to matrix
			ret = Matrix(vec);
		}
		else {
			ret.appendRow(vec);
		}
		vec.clear();
	}
	file.close();
	return ret;
}

void Matlab::write(const Matrix & m, const char* path)
{
	ofstream file;
	try {
		file.open(path);
	}
	catch (ostream::failure e) {
		std::cerr << "Exception opening file\n";
		throw new invalidParamExcep("file path error.");
	}
	for (int i = 0; i < m.size(0); i++) {
		for (int j = 0; j < m.size(1); j++) {
			file << m.get(i, j) << ',';
		}
		file << '\n';
	}
	file.close();
}


