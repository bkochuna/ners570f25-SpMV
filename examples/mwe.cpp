#include "SpMV.hpp"
#include <iostream>
#include <vector>

int main(/* int argc, char* argv[] */) {
  std::cout << "Hello World!" << std::endl;

  SpMV::SparseMatrix<float> *ptr_A = new SpMV::SparseMatrix_ELL<float>(20, 20);

  ptr_A->setValue(0, 0, 0.0);

  // New scoping unit. This means variables defined in here, stay here.
  {
    SpMV::SparseMatrix_ELL<double> A =
        SpMV::SparseMatrix_ELL<double>(10000, 10);
    std::cout << "Lets do stuff to A!" << std::endl;
    double a = A.getValue(0, 0);
    std::cout << "a(0,0)=" << a << std::endl;
    A.assemble();
  }

  delete (ptr_A);
  ptr_A = NULL;

  // CSR example: minimal 2x2 matrix highlighting constructor validation.
  // The test suite exercises similar shapes, but this shows how to wire one up manually.
  std::vector<size_t> csrRowPtr{0, 2, 3};
  std::vector<size_t> csrColInd{0, 1, 1};
  std::vector<double> csrValues{1.0, 2.0, 3.0};
  SpMV::SparseMatrix_CSR<double> csrExample(2, 3, csrRowPtr, csrColInd, csrValues);
  (void)csrExample; // illustrative snippet; matvec/assemble would work as in the tests.

  return 0;
}
