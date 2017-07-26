#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <tuple>
#include <cstring>
#include <cassert>


double* new2dMatrix(int rows, int cols)
{
   auto matrix  = new double[rows*cols];
   ::memset(matrix, 0, sizeof(double)*rows*cols);
   return matrix;
}


/**
 * plain algorithm
 */
double* naive_mmult(double* const A, double* const B, int N)
{
    double* C = new2dMatrix(N, N);
    for(auto row = 0; row < N; ++row)
    {
        for(auto col = 0; col < N; ++col)
        {
            double& dot = C[row*N + col];
            dot = .0;
            for(auto i = 0; i < N; ++i)
                dot += A[row*N + i] * B[i*N + row];
        }
    }
    return C;
}


/**
 * we change the access pattern here to traverse A, B, and C
 * in a linear fashion.
 */
double* smarter_mmult(double* const A, double* const B, int N)
{
    double* C = new2dMatrix(N, N);
    for(auto i = 0; i < N; ++i)
        for(auto k = 0; k < N; ++k)
            for(auto j = 0; j < N; ++j)
            {
                /*std::cout << "C[" << (i*N +j) << "] += A["
                    << (i*N + k) << "] * B["
                    << (k*N + i) << "]" << std::endl;*/
                C[i*N + j] += A[i*N + k] * B[k*N + i];
            }
return C;
}


/**
 * hand rolled threads.
 */
double* threaded_mmult(double* const A, double* const B, int N)
{
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "num_threads: " << num_threads << std::endl;
    double* C = new2dMatrix(N, N);
    int rows = N / num_threads;
    int extra = N % num_threads;
    int start = 0;
    int end   = rows;

    std::vector<std::thread> workers;

    for( int t = 1; t <= num_threads; ++t )
    {
        if( num_threads == t ) end += extra;
        auto loopbody = [ C, A, B ]( int start, int end, int N )
        {
            for( int i = start; i < end; ++i )
                for( int k = 0; k < N; ++k )
                    for( int j = 0; j < N; ++j )
                        C[i*N + j] += (A[i*N + k] * B[k*N + i]);
        };
        workers.push_back(std::thread(loopbody, start, end, N));
        start = end;
        end   = start + rows;
    }
    for(auto& t : workers)
        t.join();
    return C;
}

/**
 * uses OpenMP pragmas to spawn threads.
 */
double* omp_mmult(double* const A, double* const B, int N)
{
    double* C = new2dMatrix(N, N);
    #pragma omp parallel for schedule(dynamic)
    for(auto row = 0; row < N; ++row)
    {
        for(auto col = 0; col < N; ++col)
        {
            double& dot = C[row*N + col];
            dot = .0;
            for(auto i = 0; i < N; ++i)
                dot += A[row*N + i] * B[i*N + row];
        }
    }
    return C;
    return nullptr;
}

#ifdef CUDA
#include "cuda_mmult.h"

/**
 * Do the multiplication on the graphics card.
 */
double* cuda_mmult(double* const A, double* const B, int N)
{
    double* C = new2dMatrix(N, N);
    if(do_cuda_mmult(A, B, C, N))
        return C;
    return nullptr;
}
#endif /* defined(CUDA) */


// static matrix with known outcomes.
void fillMatrices(int matSize, double*& A, double*& B, double& TL, double& TR, double& BL, double& BR)
{
   A = new2dMatrix(matSize, matSize);
   B = new2dMatrix(matSize, matSize);

   for(int r = 0; r < matSize; ++r)
      for(int c = 0; c < matSize; ++c)
         A[r*matSize + c] = r+1;

   for(int r = 0; r < matSize; ++r)
      for(int c = 0; c < matSize; ++c)
         B[r*matSize + c] = c+1;
   
   double dN = static_cast<double>(matSize);

   TL = dN;
   TR = dN*dN;
   BL = dN*dN;
   BR = dN*dN*dN;
}

// global.
int matSize = 0;


template <typename T> 
void run_and_time(T& funspec)
{
    using namespace std::chrono;
    std::cout << "Running: " << std::get<1>(funspec) << std::endl;

    double* A, *B, TL, TR, BL, BR;
    fillMatrices(matSize, A, B, TL, TR, BL, BR);

    time_point<system_clock> start, end;
    start = system_clock::now();
    auto result = std::get<0>(funspec)(A, B, matSize);
    if(!result)
    {
        std::cout << "Fail." << std::endl;
    }
    else
    {
        end = system_clock::now();
        assert(TL == result[0]);
        assert(BR == result[matSize * (matSize-1) + matSize-1]);

        duration<double> elapsed_time(end - start);
        std::cout << std::get<1>(funspec) << " took " 
                  << duration_cast<milliseconds>(elapsed_time).count()
                  << " milliseconds" << std::endl;
    }
}


extern int main( int argc, char** argv )
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <matrix_size>"
                  << std::endl;
        return -1;
    }
    matSize = atoi(argv[1]);

    #define fun(x) std::make_tuple(x, #x)

    using mmult_fun = std::function<double* (double*, double*, int)>;
    using funspec = std::tuple<mmult_fun, std::string>;

    funspec funs[] = { fun(naive_mmult)
        , fun(smarter_mmult)
        , fun(threaded_mmult)
        , fun(omp_mmult)
        #ifdef CUDA
        , fun(cuda_mmult)
        #endif
    };

    for(auto& f : funs)
        run_and_time(f);

    return 0;
}

