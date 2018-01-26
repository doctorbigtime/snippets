#include <iostream>
#include <type_traits>
#include <boost/variant.hpp>

// lambda_visitor
// https://stackoverflow.com/a/7870614

template <typename ReturnType, typename... Lambdas> 
struct lambda_visitor;

template <typename ReturnType, typename Lambda1, typename... Lambdas> 
struct lambda_visitor<ReturnType, Lambda1, Lambdas...> 
    : public lambda_visitor<ReturnType, Lambdas...>
    , public Lambda1
{
    using Lambda1::operator();
    using lambda_visitor<ReturnType, Lambdas...>::operator();
    lambda_visitor(Lambda1 l1, Lambdas... lambdas)
        : lambda_visitor<ReturnType, Lambdas...>(lambdas...), Lambda1(l1)
    {}
};

// Specialization for 1 lambda.
template <typename ReturnType, typename Lambda1> 
struct lambda_visitor<ReturnType, Lambda1> 
    : public boost::static_visitor<ReturnType>
    , public Lambda1
{
    using Lambda1::operator();
    lambda_visitor(Lambda1 l1)
        : boost::static_visitor<ReturnType>(), Lambda1(l1)
    {}
};

// Specialization for to end recursion
template <typename ReturnType>
struct lambda_visitor<ReturnType>
    : public boost::static_visitor<ReturnType>
{
    lambda_visitor()
        : boost::static_visitor<ReturnType>()
    {}
};

template <typename ReturnType, typename... Lambdas> 
lambda_visitor<ReturnType, Lambdas...> make_lambda_visitor(Lambdas... lambdas)
{
    return { lambdas... };
}

#include <string>

struct Widget 
{
    explicit Widget(int x) : x_(x) {}
    int x_;
};

auto main(int argc, char**argv) -> int
{
    using Variant = boost::variant<std::string, double, Widget>;

    Variant v(Widget(1234));
    auto visitor = make_lambda_visitor<void>(
        [&](std::string const& s) { std::cout << "String: " << s << std::endl; },
        [&](Widget const& w) { std::cout << "Widget: " << w.x_ << std::endl; },
        [&](double const& d) { std::cout << "Double: " << d << std::endl; }
    );
    boost::apply_visitor(visitor, v);

    v = 666.999;
    boost::apply_visitor(visitor, v);

    v = "this bytes";
    boost::apply_visitor(visitor, v);

    v = 1234;
    boost::apply_visitor(visitor, v);

    return 0;
}
