#include "SpMV.hpp"

#include <iostream>

// Traditionally, main takes two arguments: argc and argv.
// These are the number of command line arguments and the arguments themselves.
// int main(int argc, char* argv[])
// However, we don't need them for this example.
int main()
{
  std::cout << "Hello World!\n";

  SpMV::SparseMatrix* ptr_A = new SpMV::SparseMatrix_COO();

  // New scoping unit. This means variables defined in here, stay here.
  {
    SpMV::SparseMatrix_COO A = SpMV::SparseMatrix_COO();
  }

  delete(ptr_A);

  return 0;
}
