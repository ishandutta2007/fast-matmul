#ifndef _strassen_HPP_
#define _strassen_HPP_


// This is an automatically generated file from gen.py.
#include "common.hpp"


namespace strassen {
template <typename Scalar>
void FastMatmul(Matrix<Scalar>& A, Matrix<Scalar>& B, Matrix<Scalar>& C, int numsteps, double x=1e-8) {
    // Base case for recursion
    if (numsteps == 0) {
        Gemm(A, B, C);
        return;
    }

    int A_row_step = A.m() / 2;
    int A_col_step = A.n() / 2;
    Matrix<Scalar> A11 = A.Subblock(2, 2, 1, 1);
    Matrix<Scalar> A12 = A.Subblock(2, 2, 1, 2);
    Matrix<Scalar> A21 = A.Subblock(2, 2, 2, 1);
    Matrix<Scalar> A22 = A.Subblock(2, 2, 2, 2);
    int B_row_step = B.m() / 2;
    int B_col_step = B.n() / 2;
    Matrix<Scalar> B11 = B.Subblock(2, 2, 1, 1);
    Matrix<Scalar> B12 = B.Subblock(2, 2, 1, 2);
    Matrix<Scalar> B21 = B.Subblock(2, 2, 2, 1);
    Matrix<Scalar> B22 = B.Subblock(2, 2, 2, 2);
    int C_row_step = C.m() / 2;
    int C_col_step = C.n() / 2;
    Matrix<Scalar> C11 = C.Subblock(2, 2, 1, 1);
    Matrix<Scalar> C12 = C.Subblock(2, 2, 1, 2);
    Matrix<Scalar> C21 = C.Subblock(2, 2, 2, 1);
    Matrix<Scalar> C22 = C.Subblock(2, 2, 2, 2);


    // These are the intermediate matrices.
    // We define them here so that they can be used
    // inside the lambda functions for Cilk.
    Matrix<Scalar> M1(C_row_step, C_col_step);
    Matrix<Scalar> M2(C_row_step, C_col_step);
    Matrix<Scalar> M3(C_row_step, C_col_step);
    Matrix<Scalar> M4(C_row_step, C_col_step);
    Matrix<Scalar> M5(C_row_step, C_col_step);
    Matrix<Scalar> M6(C_row_step, C_col_step);
    Matrix<Scalar> M7(C_row_step, C_col_step);


#ifdef _OPEN_MP_
    #pragma omp parallel
    {
    #pragma omp single
        {
#endif
    // M1 = (1.0 * A11 + 1.0 * A22) * (1.0 * B11 + 1.0 * B22)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M1A(A11.m(), A11.n());
    Add(A11, A22, Scalar(1.0), Scalar(1.0), M1A);
    Matrix<Scalar> M1B(B11.m(), B11.n());
    Add(B11, B22, Scalar(1.0), Scalar(1.0), M1B);
    FastMatmul(M1A, M1B, M1, numsteps - 1, x);
    M1A.deallocate();
    M1B.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M2 = (1.0 * A21 + 1.0 * A22) * (1.0 * B11)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M2A(A11.m(), A11.n());
    Add(A21, A22, Scalar(1.0), Scalar(1.0), M2A);
    FastMatmul(M2A, B11, M2, numsteps - 1, x);
    M2A.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M3 = (1.0 * A11) * (1.0 * B12 + -1.0 * B22)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M3B(B11.m(), B11.n());
    Add(B12, B22, Scalar(1.0), Scalar(-1.0), M3B);
    FastMatmul(A11, M3B, M3, numsteps - 1, x);
    M3B.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M4 = (1.0 * A22) * (-1.0 * B11 + 1.0 * B21)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M4B(B11.m(), B11.n());
    Add(B11, B21, Scalar(-1.0), Scalar(1.0), M4B);
    FastMatmul(A22, M4B, M4, numsteps - 1, x);
    M4B.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M5 = (1.0 * A11 + 1.0 * A12) * (1.0 * B22)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M5A(A11.m(), A11.n());
    Add(A11, A12, Scalar(1.0), Scalar(1.0), M5A);
    FastMatmul(M5A, B22, M5, numsteps - 1, x);
    M5A.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M6 = (-1.0 * A11 + 1.0 * A21) * (1.0 * B11 + 1.0 * B12)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M6A(A11.m(), A11.n());
    Add(A11, A21, Scalar(-1.0), Scalar(1.0), M6A);
    Matrix<Scalar> M6B(B11.m(), B11.n());
    Add(B11, B12, Scalar(1.0), Scalar(1.0), M6B);
    FastMatmul(M6A, M6B, M6, numsteps - 1, x);
    M6A.deallocate();
    M6B.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

    // M7 = (1.0 * A12 + -1.0 * A22) * (1.0 * B21 + 1.0 * B22)
#ifdef _CILK_
    cilk_spawn [&] {
#elif defined _OPEN_MP_
# pragma omp task
    {
#endif
    Matrix<Scalar> M7A(A11.m(), A11.n());
    Add(A12, A22, Scalar(1.0), Scalar(-1.0), M7A);
    Matrix<Scalar> M7B(B11.m(), B11.n());
    Add(B21, B22, Scalar(1.0), Scalar(1.0), M7B);
    FastMatmul(M7A, M7B, M7, numsteps - 1, x);
    M7A.deallocate();
    M7B.deallocate();
#ifdef _CILK_
    }();
#elif defined _OPEN_MP_
    }
#endif

#ifdef _CILK_
    cilk_sync;
#elif defined _OPEN_MP_
        }  // End omp single region
    }  // End omp parallel region
#endif
    Add(M1, M4, M5, M7, Scalar(1.0), Scalar(1.0), Scalar(-1.0), Scalar(1.0), C11);
    Add(M3, M5, Scalar(1.0), Scalar(1.0), C12);
    Add(M2, M4, Scalar(1.0), Scalar(1.0), C21);
    Add(M1, M2, M3, M6, Scalar(1.0), Scalar(-1.0), Scalar(1.0), Scalar(1.0), C22);
}

}
  // namespace strassen
#endif  // _strassen_HPP_
