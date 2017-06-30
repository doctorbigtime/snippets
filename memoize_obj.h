#pragma once
#include <memory>
#include <stdexcept>
#include <map>
#include <utility>
#include <functional>
#include <iostream>
#include <cassert>

template <typename fun>
struct fun_decomposer;

template <typename R, typename ...Args>
struct fun_decomposer<R(Args...)>
{
    using Product = R;
    using ArgT = std::tuple<Args...>;
};


template <typename Base>
struct SingleInstance : public Base
{
    using ThisType = SingleInstance<Base>;
    using CreateType = decltype(Base::create);
    using Decomposer = fun_decomposer<CreateType>;

    using CreateArgs = typename Decomposer::ArgT;
    using CreateProduct = typename Decomposer::Product;
    static_assert(std::is_same<CreateProduct, Base*>::value
        , "create() function does not return expected type.");

    using MemoizeTable = std::map<CreateArgs, CreateProduct>;
    using Product = std::shared_ptr<Base>;
    using Map = std::map<CreateProduct, Product>;

    static MemoizeTable& get_memoize_table()
    {
        static MemoizeTable* p = new MemoizeTable;
        return *p;
    }
    static void reset_memoize_table()
    {
        get_memoize_table().clear();
    }

    static Map& get()
    {
        static Map* p = new Map;
        return *p;
    }
    static void reset_ptrmap()
    {
        get().clear();
    }
    static void reset_memoization()
    {
        reset_memoize_table();
        reset_ptrmap();
    }

    template <typename R, typename ...Args>
    static auto memoize_instance_impl(std::function<R(Args...)> fun)
    {
        return [fun](Args... args) mutable -> R {
            auto argt = std::forward_as_tuple(args...);
            auto& table = ThisType::get_memoize_table();
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
    static std::shared_ptr<Base> get_or_create(Args&&... args)
    {
        auto item = memoize_instance(&Base::create)(std::forward<Args&&>(args)...);
        assert(item);
        auto& map = get();
        auto iter = map.find(item);
        if(iter != map.end())
            return iter->second;
        auto ptr = std::shared_ptr<Base>(item);
        map[item] = ptr;
        return ptr;
    }
};

