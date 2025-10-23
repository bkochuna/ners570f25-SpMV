JDS View Method  
Author: Sander Katz  
Date: 10/21/25  

A No-Pro Pro Industries LLC Production  
All Rights Reserved  
Glow Blue  

# ChatGPT Wrote This ReadMe

# SparseMatrix_CSR_Tests

This module provides a test suite for verifying the functionality of the **Compressed Sparse Row (CSR)** matrix implementation.
It ensures that matrix and vector accessors, as well as the sparse matrix–vector multiplication (SpMV) computation, work correctly by comparing against reference data stored in `.mtx` and `.vec` files.

---

## Overview

The test suite:

* Loads dense matrix and vector data from text files.
* Converts and sets them into CSR structures using setter functions (`setMatrix`, `setVector`).
* Retrieves data using getters and compares the results to ensure correctness.
* Performs the SpMV computation using `compute()` and verifies the output.
* Reports detailed error messages if any mismatch occurs.

---

## Methods

### **openMatrixFile(const std::string& filename)**

**Purpose:**
Reads a dense matrix from a `.mtx` file and returns it as a 2D `std::vector`.

**Description:**

* Skips comment lines starting with `%`.
* Reads the matrix dimensions and nonzero count.
* Initializes a zero-filled 2D vector of size `[rows][cols]`.
* Populates matrix values at given `(row, column)` positions.

**Returns:**
`std::vector<std::vector<double>>` representing the dense matrix.

---

### **openVectorFile(const std::string& filename)**

**Purpose:**
Reads a vector from a `.vecin` or `.vecout` file.

**Description:**

* Opens the specified file and reads each line as a `double`.
* Stores all values sequentially in a 1D vector.

**Returns:**
`std::vector<double>` containing all vector elements.

---

### **compareVectors(const std::vector<double>& v1, const std::vector<double>& v2)**

**Purpose:**
Compares two vectors element-wise to ensure numerical equality.

**Description:**

* Checks that both vectors are the same length.
* Compares each element using a tolerance of `1e-10`.
* Prints a mismatch message if any difference is detected.

**Returns:**
`true` if vectors are identical within tolerance, `false` otherwise.

---

### **testAccessors(const std::string& matrixFile, const std::string& vecInFile, const std::string& vecOutFile)**

**Purpose:**
Tests the CSR matrix’s getter/setter functions and validates the computation result.

**Description:**

1. Uses `csr.setMatrix()` and `csr.getMatrix()` to load and retrieve the matrix.
2. Reads the same matrix using `openMatrixFile()` and compares them element-wise.
3. Uses `csr.setVector()` and `csr.getVector()` to test vector input.
4. Reads the reference vector using `openVectorFile()` and compares results.
5. Calls `csr.compute()` to perform SpMV and compares `csr.getResult()` to the reference output vector.
6. Prints `[PASS]` for successful tests or detailed `[ERROR]` messages otherwise.

**Returns:**
`true` if all comparisons pass, `false` otherwise.

---

### **runTests()**

**Purpose:**
Runs the complete CSR accessor and computation validation suite.

**Description:**

* Automatically detects the current working directory (`std::filesystem::current_path()`).
* Constructs file paths for test matrices and vectors in `/mats` and `/vecs`.
* Iterates over multiple predefined test cases (`fs_183_1`, `impcol_e`, `lns_3937`, `nnc666`, `s1rmq4m1`).
* Invokes `testAccessors()` for each test.
* Reports overall test results and exits with an error if any test fails.

**Returns:**
`true` if all tests pass, otherwise exits with status code `1`.

---

## Example Directory Structure

```
project/
├── mats/
│   ├── fs_183_1.mtx
│   ├── impcol_e.mtx
│   ├── ...
├── vecs/
│   ├── fs_183_1.vecin
│   ├── fs_183_1.vecout
│   ├── ...
├── SparseMatrix_CSR_Tests.hpp
├── SparseMatrix_CSR_Tests.cpp
└── main.cpp
```

---

## Summary

| Function         | Description                                         |
| ---------------- | --------------------------------------------------- |
| `openMatrixFile` | Reads and returns a dense matrix from a `.mtx` file |
| `openVectorFile` | Reads and returns a vector from a `.vec` file       |
| `compareVectors` | Compares two vectors element-by-element             |
| `testAccessors`  | Validates CSR getters, setters, and compute results |
| `runTests`       | Runs all test cases automatically                   |
