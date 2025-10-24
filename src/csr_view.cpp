#include <iostream>
#include <vector>
#include <iomanip>

class CSRMatrix {
public:
    // CSR has row pointers, column indices, and non-zero values
    std::vector<int> row_pointer;
    std::vector<int> column_index;
    std::vector<double> values;
    int n_rows;
    // Constructor taking in rows, row_initialize, column_initialize, values_initialize, initialize the row pointers, column indexes, and values
    CSRMatrix(int rows, const std::vector<int>& row_initialize,
              const std::vector<int>& column_initialize, const std::vector<double>& values_initialize)
        : n_rows(rows), row_pointer(row_initialize), column_index(column_initialize), values(values_initialize) {}

    // CSR view method
    void view() const {
        std::cout << "CSR Matrix View (row_pointer | column_index | values):\n";
	// THe magical loop through rows
        for (int i = 0; i < n_rows; ++i) {
            int start = row_pointer[i];
            int end = row_pointer[i + 1];
            std::cout << "Row " << i << ": ";
            // The magical loop through all non zeroes
            for (int j = start; j < end; ++j) {
                std::cout << "(" << column_index[j] << ", "
                          << std::fixed << std::setprecolumn_initializesion(3)
                          << values[j] << ") ";
            }
            std::cout << "\n";
        }
    }
};
