/*
http://www.chesterproductions.net.nz/blogs/it/code/c-valarray-matrix-madness/11/

C++ Valarray Matrix Madness
by admin under c++, code

Numeric STL container valarray began life partially as an attempt to make C++ appealing to the supercomputing community. At the time the big thing in those big machines was, the ironically named, vector processing. However the valarray fell by the wayside as the people driving its development left the STL development group. Perhaps they realised that it didn’t really fit 100% with the STL, or maybe they just got sidetracked; who knows. But it is still useful, and here are a few reasons why:

    Can be used to write faster code for numeric spaces than possible with other STL types like the ubiquitous vector template.
    Offers the coder the possibility of staying within the comfort zone of the STL and familiar object oriented concepts with a small speed sacrifice over hand carved C.
    Allows library developers a way of writing optimized libraries for different environments so that coder can concentrate on the development at hand and not loose track in the complexity of the target environment.

So I’ve been playing around with valarray. It seemed that the best example to play with is that old classic the matrix. So here it is. Yes I know that there are some particularly hairy things wrong with it, but its not meant as a copy and paste solution. Its here as the results of a learning exercise and an example of what’s possible with valarray. There are one or two places which are not implemented or have not been tested, but you should be able to complete or test these by just looking at the rest of the examples. It should compile and run as is.

Have a look and have fun.

*/
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <valarray>
#include <vector>

/*------------------------------------------------------------------------------
  matrix2d interface

  This is the template for the matrix class.  Its not that fancy and there could
  be a lot more added to it but that was not the point of the exercise.  It is
  also limited by the number of types supported by the underlying valarray data
  container.  From an OO point of view its really not that good either given
  that a lot of the manipulation or generator methods really don't need to have
  access directly to the data part of the construct.  However on a practical
  basis including those methods in this class provide small benefits in speed
  and allow things to be a little more obvious.  There are also problems in the
  way that the generators return new matrix2d objects.  But I've no intention to
  fix them as this was just a learning exercise.  I've tossed the implementation
  of the methods into seperate compilation objects to make the interface cleaner
  for inspection purposes.  In keeping with the valarray perspective there is no
  bounds checking anywhere - you have been warned.
  ----------------------------------------------------------------------------*/

template<typename T>

class matrix2d {
public:

  // creates based on the rows and data size
  matrix2d(std::size_t rows, std::valarray<T> data);
  // creates an empty rows x size matrix
  matrix2d(std::size_t rows, std::size_t cols);
  // direct initialisation - beware that rows x cols must equal data.size()
  matrix2d(std::size_t rows, std::size_t cols, std::valarray<T> data);

  // get the number of rows in the matrix2d
  std::size_t rows() const;
  // get the number of columns in the matrix2d
  std::size_t cols() const;
  // get a copy of the data in the matrix2d
  std::valarray<T> array() const;

  // retrieve the data from row r of the matrix
  std::valarray<T> row(std::size_t r) const;
  // retrieve the data from col c of the matrix
  std::valarray<T> col(std::size_t c) const;

  // retrieve refernce to the data from row r of the matrix
  std::slice_array<T> row(std::size_t r);
  // retrieve refernce to the data from col c of the matrix
  std::slice_array<T> col(std::size_t c);

  // basic item reference
  T & operator()(std::size_t r, std::size_t c);
  // basic item retrieval
  T operator()(std::size_t r, std::size_t c) const;

  // generate a matrix sort each row then sort the rows - UNIMPLEMENTED
  matrix2d<T> sort();
  // genetate a new matrix that is the transposition of this one
  matrix2d<T> transpose();
  // generate a new matrix with this matrix's data and sort each row
  matrix2d<T> rowItmSort();
  // generate a new matrix with this matrix's data and sort each row in reverse
  matrix2d<T> rowItmSortRev();
  // generate a new matrix with this matrix's data and sort each col
  matrix2d<T> colItmSort();
  // generate a new matrix with this matrix's data and sort each col in reverse
  matrix2d<T> colItmSortRev();

  // generate a new matrix of this one with m appended below
  matrix2d<T> appendRows(matrix2d<T> &m);
  // generate a new matrix of this one with m appended to the right
  matrix2d<T> appendCols(matrix2d<T> &m);
  // generate a matrix of this one, upper left corner at row t col l - UNTESTED
  matrix2d<T> extractMatrix2d(size_t t, size_t l, size_t w, size_t h);

protected:

  std::size_t rows_;
  std::size_t cols_;
  std::valarray<T> data_;

};

/*------------------------------------------------------------------------------
  matrix2d implementation
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  matrix2d constructors
  ----------------------------------------------------------------------------*/

template<class T>
matrix2d<T>::matrix2d(std::size_t rows, std::valarray<T> data) :
rows_(rows), cols_(data.size() / rows), data_(data) {}

template<class T>
matrix2d<T>::matrix2d(std::size_t rows, std::size_t columns) :
rows_(rows), cols_(columns), data_(rows * columns) {}

template<class T>
matrix2d<T>::matrix2d(std::size_t rows, std::size_t columns,
std::valarray<T> data) :
rows_(rows), cols_(columns), data_(data) {}

/*------------------------------------------------------------------------------
  matrix2d operations
  ----------------------------------------------------------------------------*/

template<class T>
std::size_t matrix2d<T>::rows() const {
  return rows_;
}

template<class T>
std::size_t matrix2d<T>::cols() const {
  return cols_;
}

template<class T>
std::valarray<T> matrix2d<T>::array() const {
  return data_;
}

template<class T>
std::valarray<T> matrix2d<T>::row(std::size_t r) const {
  return data_[std::slice(r * cols(), cols(), 1)];
}

template<class T>
std::valarray<T> matrix2d<T>::col(std::size_t c) const {
  return data_[std::slice(c, rows(), cols())];
}

template<class T>
std::slice_array<T> matrix2d<T>::row(std::size_t r) {
  return data_[std::slice(r * cols(), cols(), 1)];
}

template<class T>
std::slice_array<T> matrix2d<T>::col(std::size_t c) {
  return data_[std::slice(c, rows(), cols())];
}

template<class T>
T& matrix2d<T>::operator()(std::size_t r, std::size_t c) {
  return data_[r * cols() + c];
}

template<class T>
T matrix2d<T>::operator()(std::size_t r, std::size_t c) const {
  return row(r)[c];
}

/*------------------------------------------------------------------------------
  matrix2d generators
  ----------------------------------------------------------------------------*/

template<class T>
matrix2d<T> matrix2d<T>::sort() {

  matrix2d<T> result(rows_, cols_);

  /* TO DO TO DO TO DO TO DO TO DO TO DO TO DO TO DO TO DO TO DO TO DO TO DO */

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::transpose() {

  matrix2d<T> result(cols_, rows_);

  for (std::size_t i = 0; i < rows_; ++i)
    result.col(i) = static_cast<std::valarray<T> > (row(i));

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::rowItmSort() {

  matrix2d<T> result(rows_, cols_);

  for (std::size_t i = 0; i < rows_; ++i) {

    std::valarray<T> x = static_cast<std::valarray<T> > (row(i));
    std::sort(&x[0], &x[cols_]);
    result.row(i) = x;
  }

  return result;
}

template<class T> bool rev (const T & a, const T & b) { return a > b; }

template<class T>
matrix2d<T> matrix2d<T>::rowItmSortRev() {

  matrix2d<T> result(rows_, cols_);

  for (std::size_t i = 0; i < rows_; ++i) {

    std::valarray<T> x = static_cast<std::valarray<T> > (row(i));
    std::sort(&x[0], &x[cols_], rev<T>);
    result.row(i) = x;
  }

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::colItmSort() {

  matrix2d<T> result(rows_, cols_);

  for (std::size_t i = 0; i < cols_; ++i) {

    std::valarray<T> x = static_cast<std::valarray<T> > (col(i));
    std::sort(&x[0], &x[rows_]);
    result.col(i) = x;
  }

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::colItmSortRev() {

  matrix2d<T> result(rows_, cols_);

  for (std::size_t i = 0; i < cols_; ++i) {

    std::valarray<T> x = static_cast<std::valarray<T> > (col(i));
    std::sort(&x[0], &x[rows_], rev<T>);
    result.col(i) = x;
  }

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::appendRows(matrix2d<T> &m) {

  matrix2d<T> result(rows_ + m.rows_, cols_);

  result.data_[std::slice(0, rows_ * cols_, 1)] = data_;
  result.data_[std::slice(rows_ * cols_, m.rows_ * m.cols_, 1)] = m.data_;

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::appendCols(matrix2d<T> &m) {

  matrix2d<T> result(rows_, cols_ + m.cols_);

  std::size_t s1[] = {rows_,cols_}; // shape of left matrix
  std::size_t p1[] = {result.cols_,1}; // position of left matrix in result
  std::size_t s2[] = {m.rows_,m.cols_}; // shape of right matrix
  std::size_t p2[] = {result.cols_,1}; // position or right matrix in result

  std::valarray<std::size_t> sv1(s1, 2);
  std::valarray<std::size_t> pv1(p1, 2);
  std::valarray<std::size_t> sv2(s2, 2);
  std::valarray<std::size_t> pv2(p2, 2);

  result.data_[std::gslice(0, sv1, pv1)] = data_; // copy left matrix into place
  result.data_[std::gslice(cols_, sv2, pv2)] = m.data_; // repeat for m

  return result;
}

template<class T>
matrix2d<T> matrix2d<T>::extractMatrix2d(size_t x, size_t y, size_t w,
size_t h) {

  /* TEST ME TEST ME TEST ME TEST ME TEST ME TEST ME TEST ME TEST ME TEST ME */

  matrix2d<T> result(h, w);

  size_t x2[] = {h, w}, s[] = {rows_, 1};
  std::valarray<size_t> xa(x2, 2), sa(s, 2);

  result.data_ = data_[(const std::gslice)std::gslice(y * rows_ + x, xa, sa)];

  return result;
}

/*------------------------------------------------------------------------------
  matrix2d general input output

  This is a simple class to help collect methods of serialising the matrix2d
  data forms.  Really they should be done another way but once again I don't
  really care as they are throw away code for the purpoese of demonstration
  only.
  ----------------------------------------------------------------------------*/

template<typename T>

class matrix2dio {
public:

  matrix2d<T> textToM2d(std::istream & is, size_t w);
  matrix2d<T> fileToM2d(std::string file, size_t w);
  void m2dToText(std::ostream & os, const matrix2d<T> & m);
  void printValarray(std::ostream & os, const std::valarray<int> & va);
};

/*------------------------------------------------------------------------------
  matrix2dio operations
  ----------------------------------------------------------------------------*/

typedef std::istream_iterator<int> int_istrm_iter;

template<class T>
matrix2d<T> matrix2dio<T>::textToM2d(std::istream & is, size_t w) {

  std::vector<T> v;
  std::valarray<T> a;

  if (!is.good() || is.eof())
    return;

  copy(int_istrm_iter(is), int_istrm_iter(), back_inserter(v));
  a.resize(v.size(), sizeof ( int));
  copy(v.begin(), v.end(), &a[0]);

  return new matrix2d<T > (w, a);
}

template<class T>
matrix2d<T> matrix2dio<T>::fileToM2d(std::string file, size_t w) {

  std::filebuf fb;
  std::istream is(&fb);

  fb.open(file.c_str(), std::ios::in);
  matrix2d<T> m = textToM2d(is, w);
  fb.close();

  return m;
}

template<class T>
void matrix2dio<T>::m2dToText(std::ostream & os, const matrix2d<T> & m) {

  size_t i = 0, j = 0;

  for (i = 0; i < m.rows(); i++) {

    std::valarray<T> r = m.row(i);
    os << r[0];

    for (j = 1; j < m.cols(); j++)
      std::cout << ' ' << r[j];

    os << '\n';
  }

  os << std::flush;
}

template<class T>
void matrix2dio<T>::printValarray(std::ostream & os,
const std::valarray<int> & va) {

  copy(&va[0], &va[va.size() - 1], std::ostream_iterator<T > (os, " "));
  os << va[va.size() - 1] << "\n";
}

/*------------------------------------------------------------------------------
  matrix2d tests
  ----------------------------------------------------------------------------*/

void testConstructors() {

  std::cout << "\n\n\nRunning Constructor Tests\n\n";

  int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  std::valarray<int> v( a, 12 );
  std::size_t x = 3, y = 4;
  matrix2dio<int> i;

  std::cout <<
    "\nTesting: matrix2d(std::size_t rows, std::size_t columns, "
    "std::valarray<T> data);\n";
  matrix2d<int> m1( x, y, v );

  std::cout << "number of rows: " << m1.rows() << '\n'
  << "number of cols: " << m1.cols() << '\n'
  << "matrix content:\n";
  i.m2dToText( std::cout, m1 );

  std::cout << "\nTesting: matrix2d(std::size_t rows, std::size_t columns);\n";
  matrix2d<int> m2( x, y );

  std::cout << "number of rows: " << m2.rows() << '\n'
  << "number of cols: " << m2.cols() << '\n'
  << "matrix content:\n";
  i.m2dToText( std::cout, m2 );

  std::cout <<
    "\nTesting: matrix2d(std::size_t rows, std::valarray<T> data);\n";
  matrix2d<int> m3( x, v );

  std::cout << "number of rows: " << m3.rows() << '\n'
  << "number of cols: " << m3.cols() << '\n'
  << "matrix content:\n";
  i.m2dToText( std::cout, m3 );
}

void testRowsAndCols() {

  std::cout << "\n\n\nRunning Row/Col Accessor Tests\n\n";

  int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  std::valarray<int> v( a, 12 );
  std::size_t x = 3, y = 4;
  matrix2dio<int> i;

  matrix2d<int> m1( x, y, v );

  std::cout << "\nTesting: std::valarray<T> row(std::size_t r) const;\n";

  std::valarray<int> r1 = m1.row(0);
  std::valarray<int> r2 = m1.row(1);
  std::valarray<int> r3 = m1.row(2); 

  std::cout << "row 1:\n";
  i.printValarray( std::cout, r1 );
  std::cout << "row 2:\n";
  i.printValarray( std::cout, r2 );
  std::cout << "row 3:\n";
  i.printValarray( std::cout, r3 );

  std::cout << "\nTesting: std::valarray<T> col(std::size_t r) const;\n";

  std::valarray<int> c1 = m1.col(0);
  std::valarray<int> c2 = m1.col(1);
  std::valarray<int> c3 = m1.col(2);
  std::valarray<int> c4 = m1.col(3); 

  std::cout << "col 1:\n";
  i.printValarray( std::cout, c1 );
  std::cout << "col 2:\n";
  i.printValarray( std::cout, c2 );
  std::cout << "col 3:\n";
  i.printValarray( std::cout, c3 );
  std::cout << "col 4:\n";
  i.printValarray( std::cout, c4 );

  std::cout << "\nTesting: std::slice_array<T> row(std::size_t r);\n";

  std::slice_array<int> rs1 = m1.row(0);
  std::slice_array<int> rs2 = m1.row(1);
  std::slice_array<int> rs3 = m1.row(2); 

  std::cout << "row 1:\n";
  i.printValarray( std::cout, rs1 );
  std::cout << "row 2:\n";
  i.printValarray( std::cout, rs2 );
  std::cout << "row 3:\n";
  i.printValarray( std::cout, rs3 );

  std::cout << "\nTesting: std::slice_array<T> col(std::size_t r);\n";

  std::slice_array<int> cs1 = m1.col(0);
  std::slice_array<int> cs2 = m1.col(1);
  std::slice_array<int> cs3 = m1.col(2);
  std::slice_array<int> cs4 = m1.col(3); 

  std::cout << "col 1:\n";
  i.printValarray( std::cout, cs1 );
  std::cout << "col 2:\n";
  i.printValarray( std::cout, cs2 );
  std::cout << "col 3:\n";
  i.printValarray( std::cout, cs3 );
  std::cout << "col 4:\n";
  i.printValarray( std::cout, cs4 );
}

void testGenerators() {

  std::cout << "\n\n\nRunning Generator Tests\n\n";

  int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  int b[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 };
  std::valarray<int> v1( a, 12 );
  std::valarray<int> v2( b, 12 );
  std::size_t x = 3, y = 4;
  matrix2dio<int> i;

  matrix2d<int> m1( x, y, v1 );
  matrix2d<int> m2( x, y, v2 );

  std::cout << "\nTesting: matrix2d<T> transpose();\noriginal:\n";

  matrix2d<int> m3 = m1.transpose();
  i.m2dToText( std::cout, m1 );
  std::cout << "transposed:\n";
  i.m2dToText( std::cout, m3 );

  std::cout << "\nTesting: matrix2d<T> appendRows(matrix2d<T> &v);\n";

  matrix2d<int> m4 = m2.appendRows(m1);
  i.m2dToText( std::cout, m4 );

  std::cout << "\nTesting: matrix2d<T> appendCols(matrix2d<T> &m);\n";

  matrix2d<int> m5 = m2.appendCols(m1);
  i.m2dToText( std::cout, m5 );

  std::cout << "\nTesting: matrix2d<T> rowItmSort();\n";

  matrix2d<int> m6 = m5.rowItmSort();
  i.m2dToText( std::cout, m6 );

  std::cout << "\nTesting: matrix2d<T> rowItmSortRev();\n";

  matrix2d<int> m7 = m5.rowItmSortRev();
  i.m2dToText( std::cout, m7 );

  std::cout << "\nTesting: matrix2d<T> colItmSort();\n";

  matrix2d<int> m8 = m4.colItmSort();
  i.m2dToText( std::cout, m8 );

  std::cout << "\nTesting: matrix2d<T> colItmSortRev();\n";

  matrix2d<int> m9 = m4.colItmSortRev();
  i.m2dToText( std::cout, m9 );
}

void testMatrix2d() {

    testConstructors();
    testRowsAndCols();
    testGenerators();
}

/*------------------------------------------------------------------------------
  matrix2d test entry point
  ----------------------------------------------------------------------------*/

int main( int argc, char** argv ) {

  testMatrix2d();
  return (EXIT_SUCCESS);
}
