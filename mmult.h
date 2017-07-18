#pragma once
#include <iostream>
#include <thread>
#include <vector>

double** new2dMatrix( int rows, int cols )
{
   double** matrix;
   double * elems;
   matrix = new T*[rows];
   elems  = new T[rows*cols];

   for( int r = 0; r < rows; ++r )
      matrix[r] = &elems[r*cols];
   return matrix;
}


double** naive_mmult(double** const A, double** const B, int N)
{
    double** C = new2dMatrix(N, N);
    for(auto row = 0; row < N; ++row)
    {
        for(auto col = 0; col < N; ++col)
        {
            T dot = .0;
            for(auto i = 0; i < N; ++i)
                dot += A[row][i] * B[i][row];
            C[row][col] = acc;
        }
    }
    return C;
}


double** smart_mmult(double** const A, double** const B, int N)
{
    return nullptr
}


double** threaded_mmult(double** const A, double** const B, int N)
{
    int nThreads = std::thread::hardware_concurrency();
    double** C = new2dMatrix< T >( N, N );
    int rows = N / nThreads;
    int extra = N % nThreads;
    int start = 0;
    int end   = rows;

    std::vector<std::thread> workers;

    for( int t = 1; t <= nThreads; ++t )
    {
        if( nThreads == t ) end += extra;
        auto loopbody = [ C, A, B ]( int start, int end, int N )
        {
            for( int i = start; i < end; ++i )
                for( int j = 0; j < N; ++j )
                    C[i][j] = .0;
            for( int i = start; i < end; ++i )
                for( int k = 0; k < N; ++k )
                    for( int j = 0; j < N; ++j )
                        C[i][j] += (A[i][k] * B[k][i]);
        };
        workers.push_back(std::thread(loopbody, start, end, N));
        start = end;
        end   = start + rows;
    }
    for(auto& t : workers)
        t.join();
    return C;
}

double** omp_mmult(double** const A, double** const B, int N)
{
    return nullptr
}


double** cuda_mmult(double** const A, double** const B, int N)
{
    return nullptr
}
