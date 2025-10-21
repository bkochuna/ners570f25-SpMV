#include "SparseMatrix_JDS.hpp"

using namespace std;


namespace SpMV
{
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSInputMatrixSummary() const{
        auto jdsMatrix = this->holderGetJDSMatrix();
        cout << "===== JDS Matrix Summary =====" << endl;
        cout << "Rows: " << jdsMatrix.numRows << ", Cols: " << jdsMatrix.numCols << ", NNZ: " << jdsMatrix.nnz << endl;
        cout << "Max row length: " << jdsMatrix.maxRowLength << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSInputVectorSummary() const{
        auto jdsVector = this->holderGetJDSVector();
        cout << "===== JDS Vector Summary =====" << endl;
        cout << "NNZ: " << jdsVector.nnz << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorSummary() const{
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 25" << endl;
            return;
        }
        auto jdsVector = this->holderGetJDSResultVector();
        cout << "===== JDS Result Vector Summary =====" << endl;
        cout << "NNZ: " << jdsVector.nnz << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSMatrixToConsole() const{
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
    void SparseMatrix_JDS<fp_type>::printJDSVectorToConsole() const{
        auto jdsVector = this->holderGetJDSVector(); //Assuming the matrix is stored here
        cout << "===== JDS Vector View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsVector.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        cout << "Val" << endl;
        for (size_t i = 0; i < jdsVector.nnz; ++i) {
            double val = jdsVector.getValue(i); 
            if (val != 0) {
                cout << val << endl;
            }
        }
        
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorToConsole() const{
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 75" << endl;
            return;
        }
        auto jdsVector = this->holderGetJDSResultVector(); //Assuming the matrix is stored here
        cout << "===== JDS Result Vector View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsVector.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        cout << "Val" << endl;
        for (size_t i = 0; i < jdsVector.nnz; ++i) {
            double val = jdsVector.getValue(i); 
            if (val != 0) {
                cout << val << endl;
            }
        }
        
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSMatrixElementToConsole(const size_t row, const size_t column) const{
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here
        double value = jdsMatrix.getValue(row, column); //Assuming a method to get value at (i, j)
        cout << "Value at (" << row << ", " << column << ") = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSVectorElementToConsole(const size_t index) const{
        auto jdsVector = this->holderGetJDSVector();
        double value = jdsVector.getValue(index);
        cout << "Value at index " << index << " = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorElementToConsole(const size_t index) const{
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 110" << endl;
            return;
        }
        auto jdsVector = this->holderGetJDSResultVector();
        double value = jdsVector.getValue(index);
        cout << "Value at index " << index << " = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSMatrixRow(size_t row) const{
        auto jdsMatrix = this->holderGetJDSMatrix();
        cout << "Row " << row << ": ";
        for (size_t j = 0; j < jdsMatrix.numCols; ++j) {
            double val = jdsMatrix.getValue(row, j);
            if (val != 0) cout << "(" << j << ", " << val << ") ";
        }
        cout << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorToLogFile(const std::string& filename) const{
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 132" << endl;
            return;
        }
        auto jdsVector = this->holderGetJDSResultVector(); //Assuming the matrix is stored here

        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: could not open file " << filename << endl;
            return;
        }

        for (size_t i = 0; i < jdsVector.nnz; ++i) {
            double val = jdsVector.getValue(i); 
            if (val != 0) {
                file << val << endl;
            }
        }

        file.close();
    }
    
}
