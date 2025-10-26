# SparseMatrix_CSR_Tests

**Author:** Sander Katz  
**Date:** 10/21/25  

A No-Pro Pro Industries LLC Production – All Rights Reserved – Glow Blue  

**ChatGPT wrote this ReadMe**
---

## Overview

This module provides a test suite for verifying the functionality of the **Compressed Sparse Row (CSR)** matrix implementation.  
It ensures that:

* Matrix and vector getters/setters work correctly.
* Sparse matrix–vector multiplication (SpMV) produces accurate results.
* Outputs match reference data stored in `.mtx` and `.vec` files.

The tests provide detailed error messages for mismatches and report `[PASS]` for successful checks.

---

## Features

* Reads dense matrices and vectors from files.
* Converts data into CSR structures.
* Compares CSR data against reference dense data.
* Performs SpMV computation and verifies the output.

---

## Functions

### `openMatrixFile(const std::string& filename)`

**Purpose:** Reads a dense matrix from a `.mtx` file.  

**Behavior:**

* Ignores comment lines starting with `%`.
* Reads matrix dimensions and nonzero count.
* Initializes a 2D vector of zeros `[rows][cols]`.
* Fills values at specified `(row, column)` positions.

**Returns:** `std::vector<std::vector<double>>` representing the matrix.

---

### `openVectorFile(const std::string& filename)`

**Purpose:** Reads a vector from `.vecin` or `.vecout` files.  

**Behavior:** Reads each line as a `double` and stores sequentially in a vector.

**Returns:** `std::vector<double>` containing vector elements.

---

### `compareVectors(const std::vector<double>& v1, const std::vector<double>& v2)`

**Purpose:** Compares two vectors element-wise.  

**Behavior:**

* Checks that both vectors have the same length.
* Compares each element using a tolerance of `1e-10`.
* Prints a mismatch message if any difference is detected.

**Returns:** `true` if vectors match within tolerance, `false` otherwise.

---

### `testAccessors(const std::string& matrixFile, const std::string& vecInFile, const std::string& vecOutFile)`

**Purpose:** Tests CSR matrix getters/setters and validates computation.  

**Behavior:**

1. Loads the matrix using `setMatrix()` and retrieves it with `getMatrix()`.
2. Compares CSR data to a dense reference matrix.
3. Loads the input vector with `setVector()` and compares to reference vector.
4. Performs SpMV via `compute()` and compares output to reference.
5. Prints `[PASS]` for successful tests or detailed `[ERROR]` messages for failures.

**Returns:** `true` if all comparisons pass, `false` otherwise.

---

### `runTests()`

**Purpose:** Runs the full CSR accessor and computation test suite.  

**Behavior:**

* Detects the current working directory.
* Constructs file paths for test matrices (`/mats`) and vectors (`/vecs`).
* Iterates over multiple predefined test cases:
  * `fs_183_1`, `impcol_e`, `lns_3937`, `nnc666`, `s1rmq4m1`
* Invokes `testAccessors()` for each test case.
* Reports overall test results and exits on failure.

**Returns:** `true` if all tests pass; exits with code `1` if any test fails.

---

## Example Directory Structure

