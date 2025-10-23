#include "SparseMatrix_CSR_Tests.hpp"

using namespace std;

namespace SpMV
{
    template <class fp_type>
    vector<vector<double>> SparseMatrix_CSR_Tests<fp_type>::openMatrixFile(const string &filename)
    {
        // Open the matrix file 
        ifstream file(filename);
        vector<vector<double>> my_arr;

        //If the file is open, read the matrix data
        if (file.is_open())
        {
            string line;
            int rows, cols, nnz;
            int r, c;
            double val;

            while (getline(file, line))
            {
                if (line.empty() || line[0] == '%')
                    continue;
                istringstream iss(line);
                if (iss >> rows >> cols >> nnz)
                    break;
            }

            my_arr.assign(rows, vector<double>(cols, 0));

            while (getline(file, line))
            {
                istringstream iss(line);
                iss >> r >> c >> val;
                my_arr[r - 1][c - 1] = val;
            }

            file.close();
        }
        else
        {
            cerr << "Unable to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        return my_arr;
    }

    template <class fp_type>
    vector<double> SparseMatrix_CSR_Tests<fp_type>::openVectorFile(const string &filename)
    {
        ifstream file(filename);
        vector<double> my_arr;

        if (file.is_open())
        {
            string line;
            double val;
            while (getline(file, line))
            {
                stringstream iss(line);
                iss >> val;
                my_arr.push_back(val);
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        return my_arr;
    }

    template <class fp_type>
    bool SparseMatrix_CSR_Tests<fp_type>::compareVectors(const vector<double> &v1, const vector<double> &v2)
    {
        // Compare vector sizes
        if (v1.size() != v2.size())
        {
            cerr << "Vector size mismatch: " << v1.size() << " vs " << v2.size() <<endl;
            return false;
        }

        // Compare vector elements
        for (size_t i = 0; i < v1.size(); ++i)
        {
            //if not in tolerance, break
            if (abs(v1[i] - v2[i]) > 1e-10)
            {
                cerr << "Mismatch at index " << i << ": " << v1[i] << " vs " << v2[i] << endl;
                return false;
            }
        }

        return true;
    }


    template <class fp_type>
    bool SparseMatrix_CSR_Tests<fp_type>::testAccessors(const string &matrixFile, const string &vecInFile, const string &vecOutFile)
    {
        SparseMatrix_CSR<fp_type> csr;

        csr.setMatrix(matrixFile); //Assuming this is the setter method
        auto csrMatrix = csr.getMatrix(); //Assuming this is the getter method
        auto matrix = SparseMatrix_CSR_Tests<fp_type>::openMatrixFile(matrixFile);

        //check if same size
        if (csrMatrix.size() != matrix.size())
        {
            cerr << "[ERROR] Matrix " << matrixFile << " row size mismatch" << endl;
            return false;
        }

        //check each element
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            //check if same size before checking each element
            if (csrMatrix[i].size() != matrix[i].size())
            {
                cerr << "[ERROR] Matrix " << matrixFile << " column size mismatch in row " << i << endl;
                return false;
            }

            for (size_t j = 0; j < matrix[i].size(); ++j)
            {

                //if not is tolerance, break
                if (abs(csrMatrix[i][j] - denseMatrix[i][j]) > 1e-10)
                {
                    cerr << "[ERROR] Matrix " << matrixFile << " mismatch at (" << i << ", " << j << "): "
                              << csrMatrix[i][j] << " vs " << matrix[i][j] << endl;
                    return false;
                }
            }
        }

        csr.setVector(vecInFile); //assuming this is the setter method
        auto csrVec = csr.getVector(); //assuming this is the getter method
        auto vecIn = SparseMatrix_CSR_Tests<fp_type>::openVectorFile(vecInFile);

        // Compare input vectors
        if (!compareVectors(csrVec, vecIn))
        {
            cerr << "[ERROR] Input vector " << vecInFile << " mismatch" << endl;
            return false;
        }

        // Compute the result vector
        csr.compute();
        auto csrResult = csr.getResult();
        auto realResult = SparseMatrix_CSR_Tests<fp_type>::openVectorFile(vecOutFile);

        // Compare result vectors
        if (!compareVectors(csrResult, realResult))
        {
            cerr << "[ERROR] Result vector mismatch for " << vecOutFile << endl;
            return false;
        }

        cout << "[PASS] " << matrixFile << endl;
        return true;
    }

    template <class fp_type>
    bool SparseMatrix_CSR_Tests<fp_type>::runTests()
    {
        namespace fs = filesystem;
        //get the current path
        fs::path basePath = fs::current_path();

        //name of the test matrices
        vector<string> names = {"fs_183_1", "impcol_e", "lns_3937", "nnc666", "s1rmq4m1"};
        bool passed = true;

        for (const auto &n : names)
        {
            //set file paths
            string matrixFile = (basePath / "mats" / (n + ".mtx")).string();
            string vecInFile = (basePath / "vecs" / (n + ".vecin")).string();
            string vecOutFile = (basePath / "vecs" / (n + ".vecout")).string();

            //check if passed
            bool passed = testAccessors(matrixFile, vecInFile, vecOutFile);

            //if did not pass, break
            if (!passed)
            {
                cerr << "[FAIL] Test failed for matrix: " << matrixFile << endl;
                passed = false;
                break;
            }
        }

        // Final result
        if (passed)
            cout << "All CSR accessor tests passed" << endl;
        else
        {
            cerr << "CSR Accessor Tests Failed." << endl;
            exit(1);
        }

        return passed;
    }

}
