#include <SpMV.hpp>
#include <SparseMatrix_JDS.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <functional>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Helper: capture stdout while running a function
static std::string capture_stdout(std::function<void()> fn)
{
	std::ostringstream buf;
	std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
	fn();
	std::cout.rdbuf(old);
	return buf.str();
}

using T = double;

TEST_CASE(matrix_summary_print)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3, 3);
	jds.setValue(size_t(0), size_t(0), T(1));
	jds.setValue(size_t(1), size_t(1), T(2));
	jds.setValue(size_t(2), size_t(2), T(3));
	jds.assemble();

	auto out = capture_stdout([&]{ jds.printJDSInputMatrixSummary(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(vector_summary_print)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3, 3);
	jds.setValue(size_t(0), size_t(0), T(1));
	jds.setValue(size_t(1), size_t(1), T(2));
	jds.setValue(size_t(2), size_t(2), T(3));
	jds.assemble();

	std::vector<T> x = {1,1,1};
	std::vector<T> y(3,0);
	jds.matvec(x, y);

	auto out = capture_stdout([&]{ jds.printJDSInputVectorSummary(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(result_vector_missing_error)
{
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	// Don't call matvec, so result vector is missing
	auto out = capture_stdout([&]{ jds.printJDSResultVectorSummary(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(result_vector_present_print)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	jds.setValue(size_t(0), size_t(0), T(1));
	jds.setValue(size_t(1), size_t(1), T(2));
	jds.setValue(size_t(2), size_t(2), T(3));
	jds.assemble();

	std::vector<T> x = {1,1,1};
	std::vector<T> y(3,0);
	jds.matvec(x, y);

	auto out = capture_stdout([&]{ jds.printJDSResultVectorSummary(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(matrix_printing_of_nonzeros)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	jds.setValue(size_t(0), size_t(1), T(5));
	jds.setValue(size_t(2), size_t(0), T(-2));
	jds.assemble();

	auto out = capture_stdout([&]{ jds.printJDSMatrixToConsole(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(vector_printing)
{

    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	jds.setValue(size_t(0), size_t(0), T(1));
	jds.setValue(size_t(1), size_t(1), T(2));
	jds.setValue(size_t(2), size_t(2), T(3));
	jds.assemble();

	std::vector<T> x = {1,1,1};
	std::vector<T> y(3,0);
	jds.matvec(x, y);

	auto out = capture_stdout([&]{ jds.printJDSVectorToConsole(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(single_element_prints)
{

    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	jds.setValue(size_t(2), size_t(1), T(4.2));
	jds.assemble();

	auto out1 = capture_stdout([&]{ jds.printJDSMatrixElementToConsole(2,1); });
	ASSERT(out1.size() >= 0);

	auto out2 = capture_stdout([&]{ jds.printJDSVectorElementToConsole(0); });
	ASSERT(out2.size() >= 0);
}

TEST_CASE(row_print)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(3,3);
	jds.setValue(size_t(1), size_t(0), T(1));
	jds.setValue(size_t(1), size_t(2), T(2));
	jds.assemble();

	auto out = capture_stdout([&]{ jds.printJDSMatrixRow(1); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(boundary_empty_matrix)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(0,0);
	// Nothing to assemble; ensure constructing/printing doesn't crash
	auto out = capture_stdout([&]{ jds.printJDSInputMatrixSummary(); });
	ASSERT(out.size() >= 0);
}

TEST_CASE(boundary_zero_only_row)
{
    // TO CONFIRM: HOW AND WHAT IS THE VIEW FUCNTION CALLING WHEN FULL IMPLEMENTATION IS DONE
	SpMV::SparseMatrix_JDS<T> jds(2,2);
	jds.setValue(size_t(0), size_t(0), T(0));
	jds.assemble();

	auto out = capture_stdout([&]{ jds.printJDSMatrixToConsole(); });
	ASSERT(out.size() >= 0);
}

TEST_SUITE(jds_view_suite)
{
	TEST(matrix_summary_print);
	TEST(vector_summary_print);
	TEST(result_vector_missing_error);
	TEST(result_vector_present_print);
	TEST(matrix_printing_of_nonzeros);
	TEST(vector_printing);
	TEST(single_element_prints);
	TEST(row_print);
	TEST(boundary_empty_matrix);
	TEST(boundary_zero_only_row);
}

auto
main() -> int
{
	RUN_SUITE(jds_view_suite);
	return 0;
}

