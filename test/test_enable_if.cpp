#include <gtest/gtest.h>

#include <iostream>
#include <type_traits>

template < typename T >
typename std::enable_if< std::is_arithmetic<T>::value, bool >::type
foo( const T& t ) { std::cout << t+1 << " - is t+1." << std::endl; return true; }

template <typename T >
typename std::enable_if< !std::is_arithmetic<T>::value, bool >::type
foo( const T& t ) { std::cout << t << " is not an arithmetic type." << std::endl; return false; }



TEST(test_enable_if, test_call)
{
    EXPECT_TRUE(foo(1234));
    EXPECT_FALSE(foo("goodbye, cruel world"));
}
