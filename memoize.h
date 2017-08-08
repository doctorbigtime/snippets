#pragma once
#include <iostream>
#include <functional>
#include <map>

// static below is kinda ugly, but I don't know where else to store the map...
template <typename R, typename ...Args>
using memoize_table_t = std::map<std::tuple<typename std::decay<Args>::type...>, R>;

template <typename R, typename... Args>
memoize_table_t<R, Args...> memoize_table = memoize_table_t<R, Args...>{};

template <typename R, typename ...Args> 
auto memoize_impl(std::function<R(Args...)> fun)
{
    return [fun](Args... args) mutable -> R {
        auto argt = std::make_tuple(args...);
        auto& table = memoize_table<R, Args...>;
        auto memoized = table.find(argt);
        if(memoized == table.end())
        {
            auto result = fun(args...);
            table[argt] = result;
            return result;
        }
        else
        {
            return memoized->second;
        }
    };
}

// @returns a function that wraps the passed
// function by memoizing outputs.
template <typename R, typename ...Args>
auto memoize(R(*fun)(Args...))
{
    std::function<R(Args...)> obj(fun);
    return memoize_impl(obj);
}

#if 0
int do_something(int i)
{
    std::cout << "do_something called.\n";
    return i*10;
}

int main(int argc, const char *argv[])
{
    auto a1 = memoize(do_something)(1234);
    auto a2 = memoize(do_something)(1234);
    auto a3 = memoize(do_something)(4567);
    auto a4 = memoize(do_something)(4567);
    std::cout << "got: " << a1 << ' ' << a2 << ' ' << a3 << ' ' << a4 << std::endl;
    return 0;
}
#endif
