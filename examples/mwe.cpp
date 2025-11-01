// examples/mwe.cpp

#include <iostream>
#include <vector>

// Header layout in this repo:
//   - ELL: include/SparseMatrix_ELL.hpp
//   - CSR/COO/JDS: include/spmv/SparseMatrix_*.hpp
#include "SparseMatrix_ELL.hpp"
#include "spmv/SparseMatrix_CSR.hpp"

int main() {
  std::cout << "Hello World!\n";

  // ELL: construct and assemble (no matvec here; assemble is a no-op placeholder)
  SpMV::SparseMatrix<float>* ptr_A = new SpMV::SparseMatrix_ELL<float>(20, 20);
  ptr_A->assemble();
  delete ptr_A;
  ptr_A = nullptr;

  // Minimal CSR example showing constructor wiring.
  std::vector<size_t> csrRowPtr{0, 2, 3};     // 2 rows, 3 nnz total
  std::vector<size_t> csrColInd{0, 1, 1};
  std::vector<double> csrValues{1.0, 2.0, 3.0};
  SpMV::SparseMatrix_CSR<double> csrExample(2, 3, csrRowPtr, csrColInd, csrValues);
  // (Optional) csrExample.assemble();  // if needed before matvec in your API

  std::cout << "mwe done.\n";
  return 0;
}
