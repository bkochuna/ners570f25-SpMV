JDS View Method

Author: Sander Katz

Date: 10/21/25

A No-Pro Pro Industries LLC Production

All Rights Reserved

Glow Blue


# SparseMatrix_JDS Print and View Methods
# ChatGPT Wrote This ReadMe
---

**Method:**  
`printJDSInputMatrixSummary() const`  
**Purpose:**  
Prints a summary of the input matrix, including the number of rows, columns, nonzero elements (NNZ), and maximum row length.

---

**Method:**  
`printJDSInputVectorSummary() const`  
**Purpose:**  
Prints information about the input vector, including its length and number of nonzero elements.

---

**Method:**  
`printJDSResultVectorSummary() const`  
**Purpose:**  
Prints a summary of the result vector generated after a SpMV operation.  
If the result vector has not yet been computed, an error message is printed.

---

**Method:**  
`printJDSMatrixToConsole() const`  
**Purpose:**  
Prints the entire JDS matrix in a `Row  Col  Val` format.  
Only nonzero elements are displayed, along with basic matrix information such as dimensions and NNZ.

---

**Method:**  
`printJDSVectorToConsole() const`  
**Purpose:**  
Prints the input vector values in an `Index  Val` format.  
Only nonzero entries are displayed for clarity.

---

**Method:**  
`printJDSResultVectorToConsole() const`  
**Purpose:**  
Prints the computed result vector in an `Index  Val` format.  
If the result vector does not exist, an error message is printed.

---

**Method:**  
`printJDSMatrixElementToConsole(const size_t row, const size_t column) const`  
**Purpose:**  
Prints the value of a single matrix element located at the specified `(row, column)` index.

---

**Method:**  
`printJDSVectorElementToConsole(const size_t index) const`  
**Purpose:**  
Prints the value of the input vector element at the given index.

---

**Method:**  
`printJDSResultVectorElementToConsole(const size_t index) const`  
**Purpose:**  
Prints the value of the result vector element at the given index.  
If the result vector does not exist, an error message is printed.

---

**Method:**  
`printJDSMatrixRow(size_t row) const`  
**Purpose:**  
Prints all nonzero elements from the specified matrix row in the format `(column, value)`.

---

**Method:**  
`printJDSResultVectorToLogFile(const std::string& filename) const`  
**Purpose:**  
Writes all nonzero elements of the result vector to a log file.  
If the result vector has not been computed or the file cannot be opened, an error message is printed.

---
