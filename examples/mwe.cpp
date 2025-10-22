#include <iostream>
#include "SpMV.hpp"

int main(/* int argc, char* argv[] */)
{
    std::cout << "Hello World!" << std::endl;

    SpMV::SparseMatrix<float>* ptr_A = new SpMV::SparseMatrix_COO<float>(20,20);

    ptr_A->setValue(0,0,0.0);

    // New scoping unit. This means variables defined in here, stay here.
    {
        SpMV::SparseMatrix_COO<double> A = SpMV::SparseMatrix_COO<double>(10000,10);
        std::cout << "Lets do stuff to A!" << std::endl;
        A.setValue(1,1,5.6);
        A.setValue(4,6, -10.5);
        A.view();
        double a = A.getValue(0,0);
        std::cout << "a(0,0)=" << a << std::endl;
        A.assemble();
    }

    delete(ptr_A);
    ptr_A = NULL;

    return 0;
}
