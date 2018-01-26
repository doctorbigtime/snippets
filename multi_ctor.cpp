#include <iostream>

struct Foo
{
    static Foo* create(int i) { return new Foo(i); }
    static Foo* create(std::string i) { return new Foo(i); }
    private:
    Foo(int i) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    Foo(std::string const& i) { std::cout << i << std::endl; }
};

template <typename... Args>
Foo* make_it(Args... args)
{
    auto create = static_cast<Foo*(*)(Args...)>(&Foo::create);
    create(args...);
}

auto main(int argc, char**argv) -> int
{
    std::string hw = "hello world";
    make_it(1);
    //make_it((std::string const&)hw);
    make_it(hw);
    return 0;
}
