#include "SparseMatrix_CSR_Tests.hpp"

using namespace std;

namespace SpMV
{
    template <class fp_type>
    vector<vector<double>> SparseMatrix_CSR_Tests<fp_type>::openMatrixFile(const string &filename)
    {
        ifstream file(filename);
        vector<vector<double>> my_arr;

        if (file.is_open())
        {
            string line;
            int rows, cols, nnz;
            int r, c;
            double val;

            // Skip header lines
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
        if (v1.size() != v2.size())
        {
            cerr << "Vector size mismatch: " << v1.size() << " vs " << v2.size() <<endl;
            return false;
        }

        for (size_t i = 0; i < v1.size(); ++i)
        {
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

        if (csrMatrix.size() != matrix.size())
        {
            cerr << "[ERROR] Matrix " << matrixFile << " row size mismatch" << endl;
            return false;
        }

        for (size_t i = 0; i < matrix.size(); ++i)
        {
            if (csrMatrix[i].size() != matrix[i].size())
            {
                cerr << "[ERROR] Matrix " << matrixFile << " column size mismatch in row " << i << endl;
                return false;
            }

            for (size_t j = 0; j < matrix[i].size(); ++j)
            {

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

        if (!compareVectors(csrVec, vecIn))
        {
            cerr << "[ERROR] Input vector " << vecInFile << " mismatch" << endl;
            return false;
        }

        csr.compute();
        auto csrResult = csr.getResult();
        auto realResult = SparseMatrix_CSR_Tests<fp_type>::openVectorFile(vecOutFile);

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
        fs::path basePath = fs::current_path();

        vector<string> names = {"fs_183_1", "impcol_e", "lns_3937", "nnc666", "s1rmq4m1"};
        bool passed = true;

        for (const auto &n : names)
        {
            string matrixFile = (basePath / "mats" / (n + ".mtx")).string();
            string vecInFile = (basePath / "vecs" / (n + ".vecin")).string();
            string vecOutFile = (basePath / "vecs" / (n + ".vecout")).string();

            bool passed = testAccessors(matrixFile, vecInFile, vecOutFile);

            if (!passed)
            {
                cerr << "[FAIL] Test failed for matrix: " << matrixFile << endl;
                passed = false;
                break;
            }
        }

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
