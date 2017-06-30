#include <memory>
#include <stdexcept>
#include <map>
#include <utility>
#include <functional>
#include <iostream>
#include <cassert>

template <typename>  struct table_maker;
template <typename R, typename ...Args>
struct table_maker<R(*)(Args...)>
{
    using ArgT = std::tuple<Args...>;
    using Table = std::map<ArgT, R>;
};

template <typename Derived>
struct SingleInstance
{
    using MemoizeTable = typename table_maker<typename Derived::create>::Table;
    using Map = std::map<Derived*, std::shared_ptr<Derived>>;

    static MemoizeTable& getTable()
    {
        static MemoizeTable* p = new MemoizeTable;
        return *p;
    }
    static void resetMemoizeTable()
    {
        getTable().clear();
    }

    static Map& get()
    {
        static Map* p = new Map;
        return *p;
    }
    static void resetMap()
    {
        get().clear();
    }
    static void reset()
    {
        resetMemoizeTable();
        resetMap();
    }

    template <typename R, typename ...Args>
    static auto memoize_instance_impl(std::function<R(Args...)> fun)
    {
        return [fun](Args... args) mutable -> R {
            auto argt = std::forward_as_tuple(args...);
            auto& table = SingleInstance::getTable();
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

    template <typename R, typename ...Args>
    static auto memoize_instance(R(*fun)(Args...))
    {
        std::function<R(Args...)> obj(fun);
        return memoize_instance_impl(obj);
    }

    template <typename ... Args> 
    static std::shared_ptr<Derived> get_or_create(Args&&... args)
    {
        auto item = memoize_instance(&Derived::create)(std::forward<Args&&>(args)...);
        assert(item);
        auto& map = get();
        auto iter = map.find(item);
        if(iter != map.end())
            return iter->second;
        return map[item] = std::shared_ptr<Derived>(item);
    }
};


struct TestObject : public SingleInstance<TestObject>
{
    static TestObject* create(int i, double& d)
    {
        return new TestObject(i, d);
    }
    private:
    TestObject(int i, double& d)
        : i_(i), d_(d)
    {
        std::cout << "TestObject created: " << i << " " << d << std::endl;
    }
    int i_;
    double d_;
};

auto main(int argc, char**argv) -> int
{
    double d1 = 666.66, d2 = 999.99;
    auto p1 = TestObject::get_or_create(1, d1);
    auto p2 = TestObject::get_or_create(1, d2);
    auto p3 = TestObject::get_or_create(2, d1);
    auto p4 = TestObject::get_or_create(1, d1);
    assert(p4 == p1);
    return 0;
}
