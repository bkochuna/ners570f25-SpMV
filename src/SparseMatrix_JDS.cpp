#include "SparseMatrix_JDS.hpp"

using namespace std;

//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!

namespace SpMV
{
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSInputMatrixSummary() const{
        auto jdsMatrix = this->holderGetJDSMatrix();
        //access jdsMatrix properties and print summary
        cout << "===== JDS Matrix Summary =====" << endl;
        cout << "Rows: " << jdsMatrix.numRows << ", Cols: " << jdsMatrix.numCols << ", NNZ: " << jdsMatrix.nnz << endl;
        cout << "Max row length: " << jdsMatrix.maxRowLength << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSInputVectorSummary() const{
        //access jdsVector properties and print summary
        auto jdsVector = this->holderGetJDSVector();
        cout << "===== JDS Vector Summary =====" << endl;
        cout << "NNZ: " << jdsVector.nnz << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorSummary() const{
        //access jdsResultVector properties and print summary
        //check if result vector exists
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
        //access jdsMatrix properties and print summary
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here
        cout << "===== JDS Matrix View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Num rows: " << jdsMatrix.numRows << endl; //Again making assumptions about how the data is stored
        cout << "Num columns: " << jdsMatrix.numCols << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsMatrix.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        //Print matrix with row, column, value format
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
        //access jdsVector properties and print summary
        auto jdsVector = this->holderGetJDSVector(); //Assuming the matrix is stored here
        cout << "===== JDS Vector View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsVector.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        //print vector values
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
        //access jdsResultVector properties and print summary
        //check if result vector exists
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 75" << endl;
            return;
        }
        //
        auto jdsVector = this->holderGetJDSResultVector(); //Assuming the matrix is stored here
        cout << "===== JDS Result Vector View =====" << endl; //Again making assumptions about how the data is stored
        cout << "Nonzero elements: " << jdsVector.nnz << endl; //Again making assumptions about how the data is stored
        cout << endl;

        //print vector values
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
        //access jdsMatrix properties and print single element
        auto jdsMatrix = this->holderGetJDSMatrix(); //Assuming the matrix is stored here
        double value = jdsMatrix.getValue(row, column); //Assuming a method to get value at (i, j)
        cout << "Value at (" << row << ", " << column << ") = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSVectorElementToConsole(const size_t index) const{
        //access jdsVector properties and print single element
        //
        auto jdsVector = this->holderGetJDSVector();
        double value = jdsVector.getValue(index);
        cout << "Value at index " << index << " = " << value << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorElementToConsole(const size_t index) const{
        //access jdsResultVector properties and print single element
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
        //access jdsMatrix properties and print single row
        auto jdsMatrix = this->holderGetJDSMatrix();
        cout << "Row " << row << ": ";
        //print all non-zero elements in the row
        for (size_t j = 0; j < jdsMatrix.numCols; ++j) {
            double val = jdsMatrix.getValue(row, j);
            if (val != 0) cout << "(" << j << ", " << val << ") ";
        }
        cout << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::printJDSResultVectorToLogFile(const std::string& filename) const{
        //access jdsResultVector properties and print to log file
        //check if result vector exists
        if (!this->hasResultVector()) { // assume you add a helper function
            cerr << "Error: result vector not computed yet. SparseMatrix_JDS.cpp, Line 132" << endl;
            return;
        }
        auto jdsVector = this->holderGetJDSResultVector(); //Assuming the matrix is stored here

        //open file
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: could not open file " << filename << endl;
            return;
        }

        //print vector values
        for (size_t i = 0; i < jdsVector.nnz; ++i) {
            double val = jdsVector.getValue(i); 
            if (val != 0) {
                file << val << endl;
            }
        }


        //close file
        file.close();
    }
    
}