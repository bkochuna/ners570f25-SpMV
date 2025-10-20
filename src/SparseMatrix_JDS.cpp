#include "SparseMatrix_JDS.hpp"

using namespace std;


namespace SpMV
{

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSToCounsole(){
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here
        cout << "===== JDS Matrix View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Num rows: " << jdsMatrix.numRows << endl; //Again making assumptions about how the data is stored
        cout << "Num columns: " << jdsMatrix.numCols << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsMatrix.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        cout << "Row Col Val" << endl;
        for (size_t i = 0; i < jdsMatrix.numRows; ++i) {
            for (size_t j = 0; j < jdsMatrix.numCols; ++j) {
                double val = jdsMatrix.getValue(i, j); //Assuming a method to get value at (i, j)
                if (val != 0) {
                    cout << i << " " << j << " " << val << endl;
                }
            }
        }
        
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSElementToConsole(const size_t row, const size_t column) {
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here
        double value = jdsMatrix.getValue(row, column); //Assuming a method to get value at (i, j)
        cout << "Value at (" << row << ", " << column << ") = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSToLogFile(const std::string& filename) {
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here

        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: could not open file " << filename << endl;
            return;
        }

        file << "===== JDS Matrix Log =====\n"; 
        file << "Num rows: " << jdsMatrix.numRows << "\n";//Again making assumptions about how the data is stored
        file << "Num columns: " << jdsMatrix.numCols << "\n";//Again making assumptions about how the data is stored
        file << "Nonzero elements: " << jdsMatrix.nnz << "\n\n";//Again making assumptions about how the data is stored

        file << "Row Col Val" << endl;
        for (size_t i = 0; i < jdsMatrix.numRows; ++i) {
            for (size_t j = 0; j < jdsMatrix.numCols; ++j) {
                double val = jdsMatrix.getValue(i, j); //Assuming a method to get value at (i, j)
                if (val != 0) {
                    file << i << " " << j << " " << val << endl;
                }
            }
        }

        file.close();
    }
}
