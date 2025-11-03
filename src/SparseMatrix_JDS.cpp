#include "SparseMatrix_JDS.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from JDS Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::assemble()
    {
        cout << "Hello from JDS assemble" << endl;

        // This routine will later convert _buildCoeff into JDS storage format.
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y) const
    {
        cout << "Hello from JDS matvec" << endl;

        y.assign(this->_nrows, static_cast<fp_type>(0));

        // Perform y = A * x in JDS format
        for (size_t jd = 0; jd + 1 < jd_ptr.size(); ++jd)
        {
            size_t start = jd_ptr[jd];
            size_t end = jd_ptr[jd + 1];
            size_t diag_len = end - start;

            for (size_t i = 0; i < diag_len; ++i)
            {
                size_t row = perm[i];
                size_t col = col_idx[start + i];
                y[row] += val[start + i] * x[col];
            }
        }
    }


    // Accessor method implementations
    template <class fp_type>
    const std::vector<fp_type>& SpMV::SparseMatrix_JDS<fp_type>::get_jdiag() const {
        return val.data();
    }

    template <class fp_type>
    const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_col_ind() const {
        return col_idx.data();
    }

    template <class fp_type>
    const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_perm() const {
        return perm.data();
    }

    template <class fp_type>
    const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_jd_ptr() const {
        return jd_ptr.data();
    }

    template <class fp_type>
    size_t SpMV::SparseMatrix_JDS<fp_type>::get_nz() const {
        return val.size();
    }

    template <class fp_type>
    size_t SpMV::SparseMatrix_JDS<fp_type>::get_ncol() const {
        return this->_ncols;
    }

    template <class fp_type>
    size_t SpMV::SparseMatrix_JDS<fp_type>::get_nrow() const {
        return this->_nrows;
    }

    template <class fp_type>
    size_t SpMV::SparseMatrix_JDS<fp_type>::get_maxnz_row() const { // Calculate the maximum number of non-zeroes per row
        if (jd_ptr.empty()) return 0;
        size_t max_len = 0;
        for (size_t i = 0; i < jd_ptr.size() - 1; i++) {
            size_t len = jd_ptr[i+1] - jd_ptr[i];
            if (len > max_len) max_len = len;
        }
        return max_len;
    }



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

// Need to declare the concrete templates within the library
// for use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;
