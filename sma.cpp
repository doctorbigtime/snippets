#include <array>
#include <vector>
#include <cstring>
#include <iostream>
#include <assert.h>

template < int N, typename T = double, bool use_array = N<=100 >
class SMA;

template < int N, typename T >
class SMA<N,T,true>
{
   public:
   SMA() : pos_(0), size_(0)
   {
      ::memset( buf_.data(), 0, sizeof(buf_) );
   }
   void add( const T& value )
   {
      sum_ -= buf_[ pos_ ];
      buf_[ pos_ ] = value;
      sum_ += value;
      pos_  = (pos_ + 1) % N;
      size_ = size_ < N ? size_+1 : N;
   }
   T get() const
   {
      if( !size_ ) return T();
      return sum_/size_;
   }
   protected:
   int pos_, size_;
   T   sum_;
   std::array< T, N > buf_;
};

template < int N, typename T >
class SMA<N,T,false>
{
   public:
   SMA() : pos_(0), size_(0)
   {
      std::cout << "Booyakasha\n";
      buf_.resize(N);
   }
   void add( const T& value )
   {
      sum_ -= buf_[ pos_ ];
      buf_[ pos_ ] = value;
      sum_ += value;
      pos_  = (pos_ + 1) % N;
      size_ = size_ < N ? size_+1 : N;
   }
   T get() const
   {
      if( !size_ ) return T();
      return sum_/size_;
   }
   protected:
   int pos_, size_;
   T   sum_;
   std::vector< T > buf_;
};

extern int main( int argc, char** argv )
{
   SMA<1000> sma10;
   for( int i = 1; i <= 2000; ++i ) sma10.add( i );
   std::cout << "sma.get: " << sma10.get() << std::endl;
   assert( sma10.get() == 1000+((1000*(1000+1))/2)/1000.0 );
   return 0;
}
