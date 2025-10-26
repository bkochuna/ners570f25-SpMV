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
        spmv::JDS B;

        spmv::JDS coo = spmv::disassemble_toCOO(B);
        //access jdsMatrix properties and print summary
        cout << "===== JDS Matrix Summary =====" << endl;
        int nnz = static_cast<int>(coo.row.size());
        cout << "Rows: " << coo.nrows << ", Cols: " << coo.ncols << ", NNZ: " << nnz << endl;
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
    void SparseMatrix_JDS<fp_type>::printJDSMatrixToConsole() const {
        spmv::JDS B;


        spmv::COO coo = spmv::disassemble_toCOO(B);

        int nnz = static_cast<int>(coo.row.size());

        std::cout << "===== JDS Matrix View =====" << std::endl;
        std::cout << "Num rows: " << coo.nrows << std::endl;
        std::cout << "Num cols: " << coo.ncols << std::endl;
        std::cout << "Nonzero elements: " << nnz << std::endl << std::endl;

        // Print matrix entries
        std::cout << "Row  Col  Val" << std::endl;
        for (int i = 0; i < nnz; ++i) {
            std::cout << coo.row[i] << "   "
                    << coo.col[i] << "   "
                    << coo.val[i] << std::endl;
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
    void SparseMatrix_JDS<fp_type>::printJDSMatrixElementToConsole(const int row, const int column) const {
        // Access stored JDS matrix
        spmv::JDS B;

        spmv::COO coo = spmv::disassemble_toCOO(B);

        // Search for the element at (row, column)
        double value = 0;
        for (size_t i = 0; i < coo.row.size(); ++i) {
            if (coo.row[i] == row && coo.col[i] == column) {
                value = coo.val[i];
                break;
            }
        }

        std::cout << "Value at (" << row << ", " << column << ") = " << value << std::endl;
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