#include <iostream>
#include <cassert>

template <typename ...Args>
void bar(Args... args)
{
   std::cout << "Bar.\n";
   int dummy[sizeof...(Args)] = { (std::cout << args << '\n', 0)... }; 
}

template <int... Is> 
struct foo 
{ 
    foo()
    { 
        bar(2*Is...);
        std::cout << std::endl;
        // bar(2*Is)...; Does not work: http://stackoverflow.com/a/25680524
        // Do the following:
        using expander = int[];
        expander{ (bar(Is),0)... };
    } 
};


auto main(int argc, char**argv) -> int
{
    foo<1,2,3,4,5>();
    return 0;
}
