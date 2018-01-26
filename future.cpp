#include <iostream>
#include <iomanip>
#include <vector>
#include <future>

using namespace std::chrono_literals;


auto main(int argc, char**argv) -> int
{
    using vec_t = std::vector<std::string>;
    vec_t v;
    auto fun = [&v]() -> bool {
        for(int i = 0; i < 1e6; ++i)
            v.push_back(std::to_string(i));
        //throw std::logic_error("blah blah");
        return true;
    };

    std::future<decltype(fun())> future = std::async(std::launch::async, fun);
    
    do
    {
        std::cout << "Doing something...." << std::endl;
        std::this_thread::sleep_for(10ms);
        if(std::future_status::ready == future.wait_for(0ms))
        {
            std::cout << "Thread done.\nResult: " << std::boolalpha << future.get()
                << "\nv.size(): " << v.size() << '\n'
                << "v.back(): " << v.back() << std::endl;
            break;
        }
        else
        {
            std::cout << "Thread still working." << std::endl;
        }
    } while(true);

    return 0;
}
