#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

class MylibInterface {
public:
    virtual ~MylibInterface() {}
    virtual int func(int n) = 0;
};


class Mylib : public MylibInterface {
public:
    virtual ~Mylib() {}
    int func(int n) override {
        return 123;
    }
};


class MylibMock : public MylibInterface {
public:
    virtual ~MylibMock() {}
    MOCK_METHOD(int, func, (int), (override));
};


class Myapp {
    // this pointer will be injected by the injector either with pointing
    // to the real object or to the mock object. The interface ensures that both
    // objects have the same method calls.
    MylibInterface* m_mylib;

public:
    Myapp(MylibInterface* mylib) : m_mylib(mylib) {}

    bool func(int n) {
        int ret = m_mylib->func(n);
        std::cout << "mylib.func returns: '" << ret << "'\n";
        return true;
    }
};


TEST(MylibTestSuite, mock_mylib_func)
// this test macro can be seen as the injector. It determins what object
// is injected to myapp.
{
    using ::testing::Return;

    // inject a real mylib object to myapp and exersize it
    Mylib mylib;
    Myapp myapp(&mylib);
    std::cout << "  real ";
    EXPECT_TRUE(myapp.func(10));

    // inject a mocked mylib object to myapp
    MylibMock mylib_mock;
    Myapp myapp_mock(&mylib_mock);
    // EXPECT_CALL(mylib_mock, func(100));
    EXPECT_CALL(mylib_mock, func(::testing::Eq(100)))
        .WillOnce(Return(456));

    // and exersize it
    std::cout << "mocked ";
    EXPECT_TRUE(myapp_mock.func(100));
}