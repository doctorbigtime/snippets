//#include <boost/mpl/vector.hpp>
#include <assert.h>
#include <boost/any.hpp>
#include <boost/hana.hpp>

using namespace boost::hana;

int main(int argc, const char *argv[])
{
    boost::any a = 'x';
    std::string r = switch_(a)(
        case_<int>([](auto i) { return "int: "s + std::to_string(i); }),
        case_<char>([](auto c) { return "char: "s + std::string{c}; }),
        default_([] { return "unknown"s; })
    );

    assert(r == "char: x"s);

    return 0;
}

