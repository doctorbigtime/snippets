#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>

namespace Fields
{
   struct PriceLevel
   {
      //enum { Buy, Sell } Side;
      //virtual Side side() = 0;
      virtual int  depth() { return -1; }
   };
   struct CFETF_BID_1 : public PriceLevel
   {
      static const int value = 0x0001;
      static const char* name;
      CFETF_BID_1( int arg ) : data(arg) {}
      int data;
   };
   struct CFETF_BID_2 : public PriceLevel
   {
      static const int value = 0x0002;
      static const char* name;
      CFETF_BID_2( int arg ) : data(arg) {}
      int data;
   };
   struct CFETF_ASK_1 : public PriceLevel
   {
      static const int value = 0x0003;
      static const char* name;
      CFETF_ASK_1( int arg ) : data(arg) {}
      int data;
   };
   struct CFETF_ASK_2 : public PriceLevel
   {
      static const int value = 0x0004;
      static const char* name;
      CFETF_ASK_2( int arg ) : data(arg) {}
      int data;
   };

   const char* CFETF_BID_1::name = "CFETF_BID_1";
   const char* CFETF_BID_2::name = "CFETF_BID_2";
   const char* CFETF_ASK_1::name = "CFETF_ASK_1";
   const char* CFETF_ASK_2::name = "CFETF_ASK_2";
};

namespace mpl = boost::mpl;

typedef mpl::vector< long, float, int, double, long > types;
typedef mpl::fold<
         types
         , mpl::int_<0>
         , mpl::if_< boost::is_float<mpl::_2>, mpl::next<mpl::_1>, mpl::_1 >
         >::type num_floats;

template < typename T >
struct UpdateReaderMap
{
   template < typename READER_MAP, typename KEY >
   struct apply
   {
      typedef typename mpl::has_key< READER_MAP, KEY >::type hasKey;
      BOOST_MPL_ASSERT(( boost::is_same< mpl::false_, hasKey > ));
      //typedef typename mpl::insert< READER_MAP, mpl::begin< READER_MAP >, mpl::pair< KEY, T > >::type;
      typedef typename mpl::insert< READER_MAP, mpl::begin< READER_MAP >, mpl::pair< KEY, T > >::type type;
   };
};

template < typename MESSAGE_TYPES = mpl::vector<>
         , typename READER_MAP = mpl::map<> >
struct foo
{
   typedef typename mpl::fold<
      MESSAGE_TYPES
      , READER_MAP
      , UpdateReaderMap< int >
   >::type map_t;
};

struct dispatcher_t
{
   #if 0
   typedef mpl::map<
      mpl::pair< mpl::int_< Fields::CFETF_BID_1::value >, Fields::CFETF_BID_1 >
         , mpl::pair< mpl::int_< Fields::CFETF_BID_2::value >, Fields::CFETF_BID_2 >
         , mpl::pair< mpl::int_< Fields::CFETF_ASK_1::value >, Fields::CFETF_ASK_1 >
         , mpl::pair< mpl::int_< Fields::CFETF_ASK_2::value >, Fields::CFETF_ASK_2 >
         > map_t;
   #endif
   typedef mpl::vector<
      Fields::CFETF_BID_1
      , Fields::CFETF_BID_2
      , Fields::CFETF_ASK_1
      , Fields::CFETF_ASK_2
   > map_t;

   template < typename PROCESSOR >
   void dispatch( PROCESSOR& p, int i )
   {
      find( p, i, typename mpl::begin<map_t>::type() );
   }

   template < typename PROCESSOR, typename ITER >
   void find( PROCESSOR& p, int i, ITER )
   {
      typedef typename mpl::deref<ITER>::type object_t;
      if( object_t::value == i )
      {
         callback( p, object_t(i*666) );
      }
      else
      {
         find( p, i, typename mpl::next<ITER>::type() );
      }
   }

   template < typename PROCESSOR >
   void find( PROCESSOR& p, int i, mpl::end<map_t>::type )
   {
      //BOOST_STATIC_ASSERT(( false ));
      //static_assert( false, "Should not be instantiated" );
      std::cout << "Type " << i << " not found." << std::endl;
   }

   template < typename PROCESSOR, typename T >
   void callback( PROCESSOR& p, T t )
   {
      p.process( t );
   }
};

struct processor_t
{
   dispatcher_t dispatch;
   void go( int i )
   {
      dispatch.dispatch( *this, i );
   }
   template < typename T >
   void process( const T& t )
   {
      std::cout << __PRETTY_FUNCTION__ 
         << " value: " << T::value
         << " data: " << t.data
         << std::endl;
   }
};

int main( int argc, char** argv )
{
   //BOOST_MPL_ASSERT(( boost::is_same< mpl::at<map_t, mpl::int_<0x0001> >::type, Fields::CFETF_BID_1 > ));
   //std::cout << "number of floats: " << num_floats::value << '\n';

   //typedef foo< mpl::vector< Fields::CFETF_ASK_1, Fields::CFETF_ASK_2 > > foo_t;
   //BOOST_MPL_ASSERT(( boost::is_same< mpl::at< foo_t::map_t, Fields::CFETF_ASK_1 >::type, int > ));

   processor_t p;
   while( --argc )
   {
      int i( atoi(*++argv) );
      p.go( i );
   }
   return 0;
}

