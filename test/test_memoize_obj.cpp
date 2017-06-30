#include <gtest/gtest.h>

#include "../memoize_obj.h"


struct TestObjectImpl
{
    static TestObjectImpl* create(int i, double& d)
    {
        return new TestObjectImpl(i, d);
    }
    private:
    TestObjectImpl(int i, double& d)
        : i_(i), d_(d)
    {
        std::cout << "TestObjectImpl created: " << i << " " << d << std::endl;
    }
    int i_;
    double d_;
};
using TestObject = SingleInstance<TestObjectImpl>;


TEST(test_memoize_obj, test_basic)
{
    double d1 = 666.66, d2 = 999.99;
    auto p1 = TestObject::get_or_create(1, d1);
    auto p2 = TestObject::get_or_create(1, d2);
    auto p3 = TestObject::get_or_create(2, d1);

    auto p4 = TestObject::get_or_create(1, d1);
    EXPECT_EQ(p4, p1);
    auto p5 = TestObject::get_or_create(1, d2);
    EXPECT_EQ(p5, p2);
    auto p6 = TestObject::get_or_create(2, d1);
    EXPECT_EQ(p6, p3);

    // same arg combo as p2, but d2 has changed.
    d2 = 123.456;
    auto p7 = TestObject::get_or_create(1, d2);
    EXPECT_NE(p7, p2);

    TestObject::reset_memoization();
    auto p8 = TestObject::get_or_create(1, d1);
    EXPECT_NE(p8, p1);
}

