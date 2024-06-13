#include <gtest/gtest.h>
#include <rad/Core/RefCounted.h>

struct A : public rad::RefCounted<A>
{
    A(int data) : m_data(data) { s_instanceCount += 1; }
    ~A() { s_instanceCount -= 1; }

    static std::atomic<size_t> s_instanceCount;
    int m_data;
};

std::atomic<size_t> A::s_instanceCount = 0;

TEST(Core, RefCounted)
{
    EXPECT_EQ(A::s_instanceCount, 0);
    {
        rad::Ref<A> a1 = rad::MakeRefCounted<A>(1);
        EXPECT_EQ(A::s_instanceCount, 1);
        rad::Ref<A> a2 = rad::MakeRefCounted<A>(2);
        EXPECT_EQ(A::s_instanceCount, 2);
        a1 = a2;
        EXPECT_EQ(A::s_instanceCount, 1);
    }
    EXPECT_EQ(A::s_instanceCount, 0);
}
