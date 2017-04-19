#include <iostream>

template<typename... Ts> void func(Ts... args)
{
    const int size = sizeof...(args) + 2;
    int res[size] = {1,args...,2};
    // since initializer lists guarantee sequencing, this can be used to
    // call a function on each element of a pack, in order:
    int dummy[sizeof...(Ts)] = { (std::cout << args, 0)... };
    std::cout << std::endl;
    for(auto i = 0; i < size; ++i)
        std::cout << "res[" << i << "] = " << res[i] << std::endl;
}

int main(int argc, const char *argv[])
{
    func(1, 2, 3, 4);
    return 0;
}
