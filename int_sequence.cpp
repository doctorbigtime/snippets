// how to generate a monotonically increasing integer sequence as template parameters.
#include <iostream>

// here's the integer sequence.
template <int...>
struct int_sequence
{

};

// basic template that gets called like this make_int_sequence<5>
template <int N, int... Is>
struct make_int_sequence : make_int_sequence<N - 1, N - 1, Is...>
{
};

// specialization to stop the iteration.
template <int... Is>
struct make_int_sequence<0, Is...> : int_sequence<Is...>
{
};

template <int...Is> 
void print(int_sequence<Is...>)
{
    std::cout << "print:\n";
    int dummy[] = { (std::cout << Is << '\n', 0)... };
}

template <typename ...Args> 
void foo(Args... args)
{
    print(make_int_sequence<sizeof...(Args)>{});
}


auto main(int argc, char**argv) -> int
{
    foo("bar","baz","qux");
    return 0;
}
