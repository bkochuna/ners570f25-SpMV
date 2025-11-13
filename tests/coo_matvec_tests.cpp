#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "SparseMatrix_COO.h"

using namespace std;
using namespace SpMV;

bool approx_equal(double a, double b, double tol=1e-12) {
    return fabs(a - b) < tol;
}

void print_vector(const vector<double>& v) {
    cout << "[";
    for(size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i != v.size() -1) cout << ", ";
    }
    cout << "]\n";
}

bool test_matvec(const string& name, SparseMatrix_COO<double>& mat, const vector<double>& x, const vector<double>& expected) {
    try {
        vector<double> b = mat.matvec(x);
        if (b.size() != expected.size()) {
            cout << name << " failed: output size mismatch.\n";
            return false;
        }
        for (size_t i = 0; i < b.size(); ++i) {
            if (!approx_equal(b[i], expected[i])) {
                cout << name << " failed at index " << i << ". Expected " << expected[i] << ", got " << b[i] << "\n";
                return false;
            }
        }
    } catch (...) {
        cout << name << " threw exception\n";
        return false;
    }
    cout << name << " passed.\n";
    return true;
}

// Create random square COO matrix
SparseMatrix_COO<double> random_square_matrix(int nrows, int ncols, int nnz) {
    SparseMatrix_COO<double> mat;
    mat._nrows = nrows;
    mat._ncols = ncols;
    mat._numnz = nnz;
    mat._state = mat.assembled;
    mat.idx_row_.resize(nnz);
    mat.idx_col_.resize(nnz);
    mat.aij_.resize(nnz);

    for (int i = 0; i < nnz; ++i) {
        mat.idx_row_[i] = rand() % nrows;
        mat.idx_col_[i] = rand() % ncols;
        mat.aij_[i] = ((rand() % 200) - 100) / 10.0; // random -10 to 10
    }
    return mat;
}

// Create identity matrix of size n
SparseMatrix_COO<double> identity_matrix(int n) {
    SparseMatrix_COO<double> mat;
    mat._nrows = n;
    mat._ncols = n;
    mat._numnz = n;
    mat._state = mat.assembled;
    mat.idx_row_.resize(n);
    mat.idx_col_.resize(n);
    mat.aij_.resize(n,1.0);
    for (int i = 0; i < n; ++i) {
        mat.idx_row_[i] = i;
        mat.idx_col_[i] = i;
    }
    return mat;
}

// Compute expected matvec for a COO matrix
vector<double> compute_expected(const SparseMatrix_COO<double>& mat, const vector<double>& x) {
    vector<double> b(mat._nrows, 0.0);
    for (size_t i = 0; i < mat._numnz; ++i)
        b[mat.idx_row_[i]] += mat.aij_[i] * x[mat.idx_col_[i]];
    return b;
}

int main() {
    srand(time(nullptr));
    int pass_count = 0, fail_count = 0;
    int n = 4;
    int nnz = 6;

    cout << "===== Random square matrix tests =====\n";
    auto square_mat = random_square_matrix(n,n,nnz);
    vector<double> x_square(n,1.0);
    auto expected_square = compute_expected(square_mat, x_square);
    if (test_matvec("Random square matrix", square_mat, x_square, expected_square)) pass_count++; else fail_count++;

    cout << "===== Identity matrix tests =====\n";
    auto id_mat = identity_matrix(n);
    vector<double> x_id(n);
    for (int i = 0; i < n; ++i) x_id[i] = rand()%10+1;
    auto expected_id = compute_expected(id_mat, x_id);
    if (test_matvec("Identity matrix", id_mat, x_id, expected_id)) pass_count++; else fail_count++;

    cout << "===== Over-defined matrix tests =====\n";
    auto over_mat = square_mat;
    over_mat.idx_row_.push_back(rand()%n);
    over_mat.idx_col_.push_back(rand()%n);
    over_mat.aij_.push_back(0.0);
    over_mat._numnz = over_mat.aij_.size();
    if (test_matvec("Over-defined matrix", over_mat, x_square, expected_square)) pass_count++; else fail_count++;

    cout << "===== Under-defined matrix tests =====\n";
    auto under_mat = random_square_matrix(n,n,nnz-2); // fewer entries
    auto expected_under = compute_expected(under_mat, x_square);
    if (test_matvec("Under-defined matrix", under_mat, x_square, expected_under)) pass_count++; else fail_count++;

    cout << "===== Unassembled matrix tests =====\n";
    auto unassembled_mat = random_square_matrix(n,n,nnz);
    unassembled_mat._state = unassembled_mat.not_assembled;
    try {
        auto b5 = unassembled_mat.matvec(x_square);
        cout << "Unassembled matrix test failed: no exception thrown.\n";
        fail_count++;
    } catch (...) {
        cout << "Unassembled matrix test passed (exception caught).\n";
        pass_count++;
    }

    cout << "===== Dimension mismatch tests =====\n";
    vector<double> x_wrong(n-1);
    for(int i=0;i<n-1;i++) x_wrong[i]=1.0;
    try {
        auto b6 = square_mat.matvec(x_wrong);
        cout << "Dimension mismatch test failed.\n";
        fail_count++;
    } catch (...) {
        cout << "Dimension mismatch test passed.\n";
        pass_count++;
    }

    cout << "Testing complete: " << pass_count << " passed, " << fail_count << " failed.\n";
    return 0;
}
