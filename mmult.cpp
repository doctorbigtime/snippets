#include <iostream>
#include <chrono>
#include <thread>
#include "mmult.h"

template < typename T >
void fillMatrices( int matSize, T**& A, T**& B, T& TL, T& TR, T& BL, T& BR )
{
   A = new2dMatrix<T>( matSize, matSize );
   B = new2dMatrix<T>( matSize, matSize );

   for( int r = 0; r < matSize; ++r )
      for( int c = 0; c < matSize; ++c )
         A[r][c] = r+1;

   for( int r = 0; r < matSize; ++r )
      for( int c = 0; c < matSize; ++c )
         B[r][c] = c+1;
   
   T dN = static_cast<T>(matSize);

   TL = dN;
   TR = dN*dN;
   BL = dN*dN;
   BR = dN*dN*dN;
}

extern int main( int argc, char** argv )
{
   int matSize = atoi( argv[1] );
   int nThread = std::thread::hardware_concurrency();
   int blockSize = matSize;

   std::cout << "ll matmult!" << std::endl;

   double** A, **B, TL, TR, BL, BR;
   fillMatrices( matSize, A, B, TL, TR, BL, BR );

   std::chrono::time_point< std::chrono::system_clock > start, end;
   start = std::chrono::system_clock::now();
   double** C = mmult( A, B, matSize, nThread );
   end = std::chrono::system_clock::now();
   std::chrono::duration< double > elapsed_time( end - start );
   std::cout << "mmult took " << std::chrono::duration_cast< std::chrono::milliseconds >( elapsed_time ).count()
         << " milliseconds" << std::endl;

   return 0;
}

