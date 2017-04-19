#include <iostream>

struct foo
{
    template <typename T, typename ...Args> 
    T* create(Args... args)
    {
        return new T(args...);
    }
};


int main(int argc, const char *argv[])
{
    auto f = foo();

    auto* p = f.create<std::string>("Yessir, yessir, three bags full.");
    std::cout << "That is: " << *p << std::endl;
    delete p;

    return 0;
}
