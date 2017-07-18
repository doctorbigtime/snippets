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


double* smarter_mmult(double* const A, double* const B, int N)
{
    double* C = new2dMatrix(N, N);
    for(auto col = 0; col < N; ++col)
    {
        for(auto row = 0; row < N; ++row)
            C[row*N + col] = .0;
        for(auto k = 0; k < N; ++k)
            for(auto row = 0; row < N; ++row)
                C[col*N + row] += A[row*N + k] * B[k*N + col];
    }
    return C;
}


double* threaded_mmult(double* const A, double* const B, int N)
{
    /*int nThreads = std::thread::hardware_concurrency();
    double* C = new2dMatrix(N, N);
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
    return C;*/
    return nullptr;
}

double* omp_mmult(double* const A, double* const B, int N)
{
    return nullptr;
}


double* cuda_mmult(double* const A, double* const B, int N)
{
    return nullptr;
}


// static matrix with known outcomes.
void fillMatrices(int matSize, double*& A, double*& B, double& TL, double& TR, double& BL, double& BR )
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
    matSize = atoi(argv[1]);

    #define fun(x) std::make_tuple(x, #x)

    using mmult_fun = std::function<double* (double*, double*, int)>;
    using funspec = std::tuple<mmult_fun, std::string>;

    funspec funs[] = { fun(naive_mmult)
        , fun(smarter_mmult)
        , fun(threaded_mmult)
        , fun(cuda_mmult)
    };

    for(auto& f : funs)
        run_and_time(f);

    return 0;
}

