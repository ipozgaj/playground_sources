// Matrica
// (C) 2006 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cmath>

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// glavna Matrix klasa
class Matrix 
{
	protected:
		int rows, columns;
		double *elements;
		void clear_matrix();
		
	public:
		Matrix() : rows(0), columns(0), elements(NULL) {};
		Matrix(unsigned int, unsigned int);
		Matrix(const Matrix&);
		Matrix(string) throw(string);
		~Matrix();

		Matrix transposed();
		Matrix lu_decomposition() throw(string);
		Matrix lup_decomposition(Matrix& b) throw(string);
		Matrix& forward_substitution(Matrix&) throw(string);
		Matrix& backward_substitution(Matrix&) throw(string);
		
		friend ostream& operator<<(ostream&, const Matrix&);
		friend Matrix operator*(const Matrix&, const double);
		friend Matrix operator*(const double, const Matrix&);
		friend Matrix operator*(const Matrix&, const Matrix&) throw(string);
		friend Matrix operator+(const Matrix&, const Matrix&) throw(string);
		friend Matrix operator-(const Matrix&, const Matrix&) throw(string);
		friend Matrix& operator+=(Matrix&, const Matrix&) throw(string);
		friend Matrix& operator-=(Matrix&, const Matrix&) throw(string);
		friend Matrix& operator*=(Matrix&, const double);
		
		double& operator()(int i, int j) const throw(string)
		{
			if (i >= rows || j >= columns || i < 0 || j < 0) {
				throw string("Matrix index out of bound");
				exit(EXIT_FAILURE);
			}
			
			return elements[i * columns + j];
		}
		
		Matrix& operator=(const Matrix& m)
		{
			if (&m != this) {
				rows = m.rows;
				columns = m.columns;
				elements = new double[rows * columns];
				memcpy(elements, m.elements, rows * columns * sizeof(double));
			}

			return *this;
		}
};

// klasa za rjesavanje linearnih sustava
class LinearSystem
{
	Matrix A, b;

	public:
		LinearSystem(Matrix& A, Matrix& b) : A(A), b(b) {};
		Matrix lu_solve();
		Matrix lup_solve();
};

// kreira nul matrixu dimenzija MxN
Matrix::Matrix(unsigned int m, unsigned int n)
{
	rows = m;
	columns = n;
	elements = new double[rows * columns];
	clear_matrix();
}

// copy konstruktor
Matrix::Matrix(const Matrix& m)
{
	rows = m.rows;
	columns = m.columns;
	elements = new double[rows * columns];
	memcpy(elements, m.elements, rows * columns * sizeof(double));
}

// kreira i inicijalizira matricu brojevima iz datoteke
Matrix::Matrix(string filename) throw(string)
{
	rows = columns = 0;
	elements = NULL;
		
	ifstream ifs(filename.c_str());
	
	if (!ifs) {
		throw string("No such file: " + filename);
		exit(EXIT_FAILURE);
	}

	string s;
	while (getline(ifs, s)) 
		++rows;

	ifs.close();
	ifs.open(filename.c_str());

	double num;
	vector<double> v;
	
	while(ifs) {
		ifs >> num;
		v.push_back(num);
	}
	
	columns = (v.size() - 1) / rows;
	elements = new double[rows * columns];
	
	for (unsigned int i = 0 ; i < v.size() - 1 ; ++i)
		elements[i] = v[i];
}

// destruktor
Matrix::~Matrix()
{
	if (elements)
		delete[] elements;
}

// postavlja sve elemente matrice na nulu
void Matrix::clear_matrix()
{
	for (int i = 0 ; i < rows ; ++i)
		for (int j = 0 ; j < columns ; ++j)
			(*this)(i, j) = 0;
}

// << operator
ostream& operator<<(ostream& os, const Matrix& m)
{
	for (int i = 0 ; i < m.rows ; ++i) {
		for (int j = 0 ; j < m.columns ; ++j)
			os << m.elements[i * m.columns + j] << " ";
		os << endl;
	}
		
	return os;
}

// += operator
Matrix& operator+=(Matrix& left, const Matrix& right) throw(string)
{
	if (left.rows != right.rows || left.columns != right.columns) {
		throw string("Matrices of incompatible type");
		exit(EXIT_FAILURE);
	}

	Matrix result(left.rows, left.columns);
	for (int i = 0 ; i < left.rows ; ++i)
		for (int j = 0 ; j < left.columns ; ++j)
			left(i, j) += right(i, j);

	return left;
}

// -= operator
Matrix& operator-=(Matrix& left, const Matrix& right) throw(string)
{
	if (left.rows != right.rows || left.columns != right.columns) {
		throw string("Matrices of incompatible type");
		exit(EXIT_FAILURE);
	}

	Matrix result(left.rows, left.columns);
	for (int i = 0 ; i < left.rows ; ++i)
		for (int j = 0 ; j < left.columns ; ++j)
			left(i, j) -= right(i, j);

	return left;
}

// *= operator
Matrix& operator*=(Matrix& left, const double right)
{
	Matrix result(left.rows, left.columns);
	for (int i = 0 ; i < left.rows ; ++i)
		for (int j = 0 ; j < left.columns ; ++j)
			left(i, j) *= right;

	return left;
}

// zbrajanje
Matrix operator+(const Matrix& left, const Matrix& right) throw(string)
{
	if (left.rows != right.rows || left.columns != right.columns) {
		throw string("Matrices of incompatible type");
		exit(EXIT_FAILURE);
	}

	Matrix result(left.rows, left.columns);
	for (int i = 0 ; i < left.rows ; ++i)
		for (int j = 0 ; j < left.columns ; ++j)
			result(i, j) = left(i, j) + right(i, j);

	return result;
}

// oduzimanje
Matrix operator-(const Matrix& left, const Matrix& right) throw(string)
{
	if (left.rows != right.rows || left.columns != right.columns) {
		throw string("Matrices of incompatible type");
		exit(EXIT_FAILURE);
	}

	Matrix result(left.rows, left.columns);
	for (int i = 0 ; i < left.rows ; ++i)
		for (int j = 0 ; j < left.columns ; ++j)
			result(i, j) = left(i, j) - right(i, j);

	return result;
}

// mnozenje matrice skalarom s desna
Matrix operator*(const Matrix& matrix, const double scalar)
{
	Matrix result(matrix);
	for (int i = 0 ; i < result.rows ; ++i)
		for (int j = 0 ; j < result.columns ; ++j)
			result(i, j) *= scalar;

	return result;
}

// mnozenje matrice skalarom s lijeva
Matrix operator*(const double scalar, const Matrix& matrix)
{
	Matrix result(matrix);
	for (int i = 0 ; i < result.rows ; ++i)
		for (int j = 0 ; j < result.columns ; ++j)
			result(i, j) *= scalar;

	return result;
}

// transponiranje
Matrix Matrix::transposed()
{
	Matrix result(columns, rows);

	for (int i = 0 ; i < rows ; ++i)
		for (int j = 0 ; j < columns ; ++j)
			result(j, i) = (*this)(i, j);
	
	return result;
}

// unaprijedna supstitucija
Matrix& Matrix::forward_substitution(Matrix& b) throw(string)
{
	if (rows != columns) {
		throw string("Operation can be done only on quadratic matrices");
		exit(EXIT_FAILURE);
	}

	if (b.columns != 1) {
		throw string("Substitution can be done only with vectors");
		exit(EXIT_FAILURE);
	}

	if (b.rows != rows) {
		throw string("Matrix and vector are not compatible type for substitution");
		exit(EXIT_FAILURE);
	}

	for (int i = 0 ; i < b.rows - 1 ; ++i)
		for (int j = i + 1 ; j < b.rows ; ++j)
			b(j, 0) -= (*this)(j, i) * b(i, 0);

	return b;
}

// unazadnja supstitucija
Matrix& Matrix::backward_substitution(Matrix& b) throw(string)
{
	if (rows != columns) {
		throw string("Operation can be done only on quadratic matrices");
		exit(EXIT_FAILURE);
	}

	if (b.columns != 1) {
		throw string("Substitution can be done only with vectors");
		exit(EXIT_FAILURE);
	}

	if (b.rows != rows) {
		throw string("Matrix and vector are not compatible type for substitution");
		exit(EXIT_FAILURE);
	}
	
	for (int i = b.rows - 1 ; i >= 0 ; --i) {
		b(i, 0) /= (*this)(i, i);
		for (int j = 0 ; j < i ; ++j)
			b(j, 0) -= (*this)(j, i) * b(i, 0);
	}
	
	return b;
}

// mnozenje matrice s matricom
Matrix operator*(const Matrix& left, const Matrix& right) throw(string)
{
	if (left.columns != right.rows) {
		throw string("Matrices of incompatible type");
		exit(EXIT_FAILURE);
	}

	Matrix result(left.rows, right.columns);
	
	for (int i = 0 ; i < result.rows ; ++i)
		for (int j = 0 ; j < result.columns ; ++j)
			for (int k = 0 ; k < left.columns ; ++k)
				result(i, j) += left(i, k) * right(k, j);

	return result;
}

// lu dekompozicija
Matrix Matrix::lu_decomposition() throw(string)
{
	if (rows != columns) {
		throw string("LU decomposition can be done only on quadratic matrices");
		exit(EXIT_FAILURE);
	}
	
	Matrix result(*this);

	for (int i = 0 ; i < rows - 1 ; ++i)
		for (int j = i + 1 ; j < rows ; ++j) {
			result(j, i) /= result(i, i);
			for (int k = i + 1 ; k < rows ; ++k)
				result(j, k) -= result(j, i) * result(i, k);
		}
	
	return result;
}

// lup dekompozicija
Matrix Matrix::lup_decomposition(Matrix& b) throw(string)
{
	if (rows != columns) {
		throw string("LU decomposition can be done only on quadratic matrices");
		exit(EXIT_FAILURE);
	}
	
	Matrix result(*this);
	int p[rows];

	for (int i = 0 ; i < rows ; ++i)
		p[i] = i;
	
	for (int i = 0 ; i < rows - 1; ++i) {
		int pivot = i;
		for (int j = i + 1 ; j < rows ; ++j)
			if (fabs(result(p[j], i)) > fabs(result(p[pivot], i)))
				pivot = j;
		swap(p[i], p[pivot]);
		swap(b(i, 0), b(pivot, 0));
		for (int j = i + 1 ; j < rows ; ++j) {
			result(p[j], i) /= result(p[i], i);
			for (int k = i + 1 ; k < rows ; ++k)
				result(p[j], k) -= result(p[j], i) * result(p[i], k);
		}
	}
	
	return result;
}

// rjesavanje sustava LU dekompozicijom
Matrix LinearSystem::lu_solve()
{
	Matrix lu, old_b(b);
	
	lu = A.lu_decomposition();
	lu.forward_substitution(b);
	lu = lu.backward_substitution(b);
	b = old_b;
	
	return lu;
}

// rjesavanje sustava LUP dekompozicijom
Matrix LinearSystem::lup_solve()
{
	Matrix lup, old_b(b);
	
	lup = A.lu_decomposition();
	lup.forward_substitution(b);
	lup = lup.backward_substitution(b);
	b = old_b;
	
	return lup;
}

// main
int main()
{
	try {
		Matrix A("m.txt"), b("v.txt");
		LinearSystem ls(A, b);

		cout << "Rjesenje LU dekompozicijom: " << endl;
		cout << ls.lup_solve();
	} catch (string s) {
		cerr << s << endl;
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
