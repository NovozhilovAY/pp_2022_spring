// Copyright 2022 Novozhilov Alexander
#include <omp.h>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_mult.h"

TEST(Matrix_Multiplication_STD, get_works) {
    SparseMatrix matrix(3, 3);
    EXPECT_NO_THROW(matrix.get(1, 1));
}

TEST(Matrix_Multiplication_STD, get_throws_exception) {
    SparseMatrix matrix(3, 3);
    EXPECT_ANY_THROW(matrix.get(4, 4));
}

TEST(Matrix_Multiplication_STD, multiplication_works) {
    SparseMatrix matrix1(3, 5);
    SparseMatrix matrix2(5, 6);
    EXPECT_NO_THROW(matrix1.multiply_seq(matrix2));
}

TEST(Matrix_Multiplication_STD, multiplication_throws_exception) {
    SparseMatrix matrix1(3, 6);
    SparseMatrix matrix2(5, 6);
    EXPECT_ANY_THROW(matrix1.multiply_seq(matrix2));
}

TEST(Matrix_Multiplication_STD, result_matrix_has_correct_size) {
    SparseMatrix matrix1(3, 6);
    SparseMatrix matrix2(6, 10);
    SparseMatrix result = matrix1.multiply_seq(matrix2);
    EXPECT_EQ(result.getM(), matrix1.getM());
    EXPECT_EQ(result.getN(), matrix2.getN());
}

TEST(Matrix_Multiplication_STD, STD_multiplication_returns_same_result) {
    SparseMatrix matrix1(3, 6);
    SparseMatrix matrix2(6, 10);
    SparseMatrix result1 = matrix1.multiply_STD(matrix2);
    SparseMatrix result2 = matrix1.multiply_seq(matrix2);
    ASSERT_TRUE(result1 == result2);
}

TEST(Matrix_Multiplication_STD, TBB_multiplication_returns_same_result) {
    SparseMatrix matrix1(3, 6);
    SparseMatrix matrix2(6, 10);
    SparseMatrix result1 = matrix1.multiply_TBB(matrix2);
    SparseMatrix result2 = matrix1.multiply_seq(matrix2);
    ASSERT_TRUE(result1 == result2);
}

TEST(Matrix_Multiplication_STD, parallel_multiplication_works) {
   SparseMatrix matrix1(100, 100);
   SparseMatrix matrix2(100, 100);
   double startSeq = omp_get_wtime();
   SparseMatrix result1 = matrix1.multiply_seq(matrix2);
   double finishSeq = omp_get_wtime();
   double startParallelTBB = omp_get_wtime();
   SparseMatrix result2 = matrix1.multiply_TBB(matrix2);
   double finishParallelTBB = omp_get_wtime();
   double startParallelSTD = omp_get_wtime();
   SparseMatrix result3 = matrix1.multiply_STD(matrix2);
   double finishParallelSTD = omp_get_wtime();
   std::cout << "Sequential time: " << finishSeq - startSeq<< " s." << std::endl;
   std::cout << "       TBB time: " << finishParallelTBB - startParallelTBB << " s." << std::endl;
   std::cout << "std thread time: " << finishParallelSTD - startParallelSTD << " s." << std::endl;
   ASSERT_TRUE(result1 == result2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
