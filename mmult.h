#pragma once
#include <iostream>
#include <thread>
#include <vector>

template < typename T >
T** new2dMatrix( int rows, int cols )
{
   T** matrix;
   T * elems;
   matrix = new T*[rows];
   elems  = new T[rows*cols];

   for( int r = 0; r < rows; ++r )
      matrix[r] = &elems[r*cols];
   return matrix;
}

template < typename T >
double** mmult( T** const A, T** const B, int N, int nThreads )
{
   T** C = new2dMatrix< T >( N, N );
   int rows = N / nThreads;
   int extra = N % nThreads;
   int start = 0;
   int end   = rows;

   std::vector< std::thread > workers;

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
      workers.push_back( std::thread( loopbody, start, end, N ) );
      start = end;
      end   = start + rows;
   }
   for( auto& t : workers )
      t.join();
   return C;
}
