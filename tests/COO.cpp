
#include "SpMV.hpp"
#include "unit_test_framework.hpp"

using namespace SpMV;

template <typename T>
TEST_CASE(constructor)
{
  SparseMatrix_COO<T> A(2,3);
  
  ASSERT(A.getNumRows() == 2);
  ASSERT(A.getNumCols() == 3);
  ASSERT(A.getNumNonZeros() == 0);
  ASSERT(A.getState() == MatrixState::initialized);
  ASSERT(A.getFormat() == MatrixFormat::COO);
} // constructor

template <typename T>
TEST_CASE(polymorhphic_constructor)
{
  SparseMatrix<T>* ptr_A = new SparseMatrix_COO<T>(2,3);
  
  ASSERT(ptr_A->getNumRows() == 2);
  ASSERT(ptr_A->getNumCols() == 3);
  ASSERT(ptr_A->getNumNonZeros() == 0);
  ASSERT(ptr_A->getState() == MatrixState::initialized);
  ASSERT(ptr_A->getFormat() == MatrixFormat::COO);

  delete(ptr_A);
} // polymorphic constructor

template <typename T>
TEST_CASE(construct_zero)
{
  SparseMatrix_COO<T> A(0,0);
  
  ASSERT(A.getNumRows() == 0);
  ASSERT(A.getNumCols() == 0);
  ASSERT(A.getNumNonZeros() == 0);
  ASSERT(A.getState() == MatrixState::initialized);
  ASSERT(A.getFormat() == MatrixFormat::COO);

} // construct_zero

template <typename T>
TEST_SUITE(constructors)
{
  TEST(constructor<T>);
  TEST(polymorhphic_constructor<T>);
  TEST(construct_zero<T>);
} // constructors

/*****************************************************************************/

template <typename T>
TEST_CASE(get_value_no_set)
{
  SparseMatrix_COO<T> A(2,2);

  ASSERT_NEAR(A.getValue(0,0),static_cast<T>(0.0),TWO_EPS);

} // get_value_no_set

template <typename T>
TEST_CASE(setget_square)
{
  SparseMatrix_COO<T> A(2,2);

  A.setValue(0,0,1.0);
  ASSERT(A.getNumNonZeros() == 1);
  ASSERT(A.getState() == MatrixState::building);
  A.setValue(1,0,2.0);
  ASSERT(A.getNumNonZeros() == 2);
  A.setValue(0,1,3.0);
  ASSERT(A.getNumNonZeros() == 3);
  A.setValue(1,1,4.0);
  ASSERT(A.getNumNonZeros() == 4);

  ASSERT_NEAR(A.getValue(0,0),static_cast<T>(1.0),TWO_EPS);
  ASSERT_NEAR(A.getValue(1,0),static_cast<T>(2.0),TWO_EPS);
  ASSERT_NEAR(A.getValue(0,1),static_cast<T>(3.0),TWO_EPS);
  ASSERT_NEAR(A.getValue(1,1),static_cast<T>(4.0),TWO_EPS);

} // setget_square

template <typename T>
TEST_CASE(set_zero)
{
  SparseMatrix_COO<T> A(2,2);

  A.setValue(0,0,0.0);
  ASSERT(A.getNumNonZeros() == 1);

} // set_zero

template <typename T>
TEST_CASE(double_set)
{
  SparseMatrix_COO<T> A(2,2);

  A.setValue(0,0,1.0);
  ASSERT(A.getNumNonZeros() == 1);
  A.setValue(0,0,2.0);
  ASSERT(A.getNumNonZeros() == 1);
  ASSERT_NEAR(A.getValue(0,0),static_cast<T>(2.0),TWO_EPS);

} // double_set

template <typename T>
TEST_SUITE(setget_values)
{
  TEST(get_value_no_set<T>);
  TEST(setget_square<T>);
  TEST(double_set<T>);
  TEST(set_zero<T>);
} // setget_values

/*****************************************************************************/
// for assemble
template <typename T>
TEST_CASE(assemble_square)
{
    SparseMatrix_COO<T> A(2,2);

    A.setValue(0,0,1.0);
    A.setValue(1,0,2.0);
    A.setValue(0,1,3.0);
    A.setValue(1,1,4.0);
    A.assemble();

    ASSERT(A.getState() == MatrixState::assembled);
    ASSERT_NEAR(A.getValue(0,0),static_cast<T>(1.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,0),static_cast<T>(2.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(0,1),static_cast<T>(3.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,1),static_cast<T>(4.0),TWO_EPS);
}

template <typename T>
TEST_CASE(assemble_diagonal)
{
    SparseMatrix_COO<T> A(3,3);

    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    ASSERT_NEAR(A.getValue(0,0),static_cast<T>(1.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,1),static_cast<T>(2.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(2,2),static_cast<T>(3.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(0,1),static_cast<T>(0.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(0,2),static_cast<T>(0.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,0),static_cast<T>(0.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(2,0),static_cast<T>(0.0),TWO_EPS);
}

template <typename T>
TEST_CASE(reassemble)
{
    SparseMatrix_COO<T> A(2,2);

    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.assemble();

    A.setValue(0,0,3.0);
    ASSERT(A.getState() == MatrixState::building);
    A.setValue(1,0,1.0);
    A.assemble();

    ASSERT(A.getState() == MatrixState::assembled);
    ASSERT_NEAR(A.getValue(0,0),static_cast<T>(3.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,0),static_cast<T>(1.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(0,1),static_cast<T>(0.0),TWO_EPS);
    ASSERT_NEAR(A.getValue(1,1),static_cast<T>(2.0),TWO_EPS);

}

template <typename T>
TEST_SUITE(assemble)
{
  TEST(assemble_square<T>);
  TEST(assemble_diagonal<T>);
  TEST(reassemble<T>);

} // assemble

/*****************************************************************************/
// for matvec

/*****************************************************************************/
auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(constructors<float>);
  RUN_SUITE(constructors<double>);

  RUN_SUITE(setget_values<float>);
  RUN_SUITE(setget_values<double>);

  RUN_SUITE(assemble<float>);
  RUN_SUITE(assemble<double>);

  return 0;
}
