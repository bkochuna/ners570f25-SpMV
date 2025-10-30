# SparseMatrix_CSR Getter Tests

**Author:** Sander Katz  
**Date:** 10/21/25  

A No-Pro Pro Industries LLC Production – All Rights Reserved – Glow Blue  

**ChatGPT wrote this ReadMe from source code**

---

## Overview

This module provides unit tests for validating the **Compressed Sparse Row (CSR)** matrix class's **basic accessor and initialization behavior**.  
It specifically checks:

* Correct reporting of matrix dimensions and non-zero count
* Correct storage of CSR arrays (`rowPtr`, `colInd`, `values`)
* Proper exception behavior when accessing an *uninitialized* CSR object
* Template correctness for multiple numeric types (`float`, `double`)

These tests do **not** perform SpMV or file-based matrix loading.  
They verify only the foundational CSR functionality.

---

## Scope of Tests

| Feature | Description |
|--------|-------------|
Matrix metadata access | `getNumRows()`, `getNumCols()`, `getNumNonzeros()`  
CSR storage arrays | `getRowPtr()`, `getColInd()`, `getValues()`  
Template flexibility | Tests run for `float` and `double`  
Error handling | Uninitialized CSR getters throw `std::runtime_error`  