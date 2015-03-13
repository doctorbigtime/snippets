#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/comparison.hpp>

#include "util.h"

struct foo1
{
   static const int val = 10;
};
struct foo2
{
   static const int val = 20;
};
struct foo3
{
   static const int val = 30;
};
struct foo4
{
   static const int val = 40;
};
struct foo5
{
   static const int val = 50;
};
struct foo6
{
   static const int val = 60;
};
struct foo7
{
   static const int val = 70;
};
struct foo8
{
   static const int val = 80;
};
struct foo9
{
   static const int val = 90;
};

namespace mpl = boost::mpl;

typedef mpl::vector< foo1
   , foo2
   , foo3
   , foo4
   , foo5
   , foo6
   , foo7
   , foo8
   , foo9
> type_vec_t;


template < typename BITER, typename EITER >
void __attribute__((always_inline)) find( int i, BITER, EITER )
{
   typedef typename mpl::distance< BITER, EITER >::type dist_t;
   typedef typename mpl::divides< dist_t, mpl::int_<2> >::type mid_dist_t;
   typedef typename mpl::advance< BITER, mid_dist_t >::type midpoint_t;
   typedef typename mpl::deref< midpoint_t >::type object_t;

   //std::cout << __PRETTY_FUNCTION__ 
   //   << "\nexamining object::val: " << object_t::val 
   //   << " dist: " << dist_t::value
   //   << " midpoint: " << mid_dist_t::value
   //   << std::endl;
   if( object_t::val == i )
   {
      std::cout << "Found: " << demangle(typeid(object_t))
         << " for " << i
         << std::endl;
   }
   else if( i > object_t::val )
   {
      //std::cout << "i > object_t::val\n";
      return find( i, midpoint_t(), typename mpl::if_< mpl::equal_to< mid_dist_t, mpl::int_<0> >, midpoint_t, EITER >::type() );
   }
   else //< i < object_t::val
   {
      //std::cout << "i < object_t::val\n";
      return find( i, typename mpl::if_< mpl::equal_to< mid_dist_t, mpl::int_<0> >, midpoint_t, BITER >::type(), midpoint_t() );
   }
}

template < typename END >
void __attribute__((always_inline)) find( int i, END, END )
{
   std::cout << __PRETTY_FUNCTION__ 
      << "\nNothing found for " << i << std::endl;
}

void __attribute__((always_inline)) go( int i )
{
   return find( i, mpl::begin< type_vec_t >::type(), mpl::end< type_vec_t >::type() );
}

int main( int argc, char** argv )
{
   /*
   typedef mpl::divides< mpl::size< type_vec_t >, mpl::int_<2> >::type half_size;
   std::cout << "half_size: " << half_size::value << '\n';
   typedef mpl::deref< mpl::advance< mpl::begin< type_vec_t >::type, half_size >::type >::type half_point;
   std::cout << "half_point value: " << half_point::val << '\n';
   std::cout << "equal 4?: " << mpl::equal_to< half_size, mpl::int_<4> >::value << '\n';
   std::cout << "equal 2?: " << mpl::equal_to< half_size, mpl::int_<2> >::value << '\n';
   */
   //while( --argc )
   //{
      int i = atoi(*++argv);
      go( i );
   //}
   return 0;
}

