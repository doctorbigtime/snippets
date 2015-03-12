// how to store variadics in a trait, then use them.

#include <iostream>

// thing that takes variadic arguments as mixins.
template <typename... Mixins>
struct Extensible : public Mixins...
{
    Extensible() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

// variadic argument store
template <typename... Args>
struct variadic_typedef {};

// here are the traits, accepting variadic arguments.
template <typename... Mixins>
struct Traits
{
    typedef variadic_typedef<Mixins...> mixins;
};

// here's the solution.
// base case: expand variadic arguments into template T's argument list.
template <template <class ...> class T, typename... Mixins>
struct expander
{
    typedef T<Mixins...> type;
};

// partially specialize this on variadic_typedef
template <template <class...> class T, typename... Args>
struct expander<T, variadic_typedef<Args...>>
{
    // extract the parameter pack here.
    typedef typename expander<T, Args...>::type type;
};

// thing that instantiates an "Extensible" with Traits::mixins.
template <typename Traits>
struct thing
{
    typedef typename expander<Extensible, typename Traits::mixins>::type Extended;
    thing(){}
    Extended e;
};

struct Mixin1 {};
struct Mixin2 {};

int main(int argc, char**argv)
{
    thing<Traits<Mixin1, Mixin2>> t;
    return 0;
}
