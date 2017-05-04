#include <iostream>
#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

template < typename T >
typename boost::enable_if< boost::is_arithmetic<T>, bool >::type
foo( const T& t ) { std::cout << t+1 << " - is t+1." << std::endl; return true; }

template <typename T >
typename boost::disable_if< boost::is_arithmetic<T>, bool >::type
foo( const T& t ) { std::cout << t << " is not an arithmetic type." << std::endl; return false; }

int main( int argc, char** argv )
{
   foo( 666 );
   foo( "johnny" );
   return 0;
}
