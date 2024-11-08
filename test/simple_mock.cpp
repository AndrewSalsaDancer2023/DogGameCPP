#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;

class Turtle {
public:    
  virtual ~Turtle() {}
  virtual void PenUp() = 0;
  virtual void PenDown() = 0;
  virtual void Forward(int distance) = 0;
  virtual void Turn(int degrees) = 0;
  virtual void GoTo(int x, int y) = 0;
  virtual int GetX() const = 0;
  virtual int GetY() const = 0;
};

class MockTurtle : public Turtle {
 public:
  MOCK_METHOD(void, PenUp, (), (override));
  MOCK_METHOD(void, PenDown, (), (override));
  MOCK_METHOD(void, Forward, (int distance), (override));
  MOCK_METHOD(void, Turn, (int degrees), (override));
  MOCK_METHOD(void, GoTo, (int x, int y), (override));
  MOCK_METHOD(int, GetX, (), (const, override));
  MOCK_METHOD(int, GetY, (), (const, override));
};

class Painter{
public:
    Painter(Turtle* turtle)
    : m_pturtle{turtle}
    {}
    bool DrawCircle(int x, int y, int Radius)
    {
        m_pturtle->PenDown();
        m_pturtle->GoTo(x, y);

        return true;
    }

    Turtle* m_pturtle{};
};

class Foo {
 private:
  FRIEND_TEST(FooTest, BarReturnsZeroOnNull);

  int Bar(void* x) { return 0;}
};

TEST(FooTest, BarReturnsZeroOnNull) {
  Foo foo;
  EXPECT_EQ(foo.Bar(NULL), 0);  // Uses Foo's private member Bar().
}

TEST(PainterTest, CanDrawSomething) {
  MockTurtle turtle;                              // #2
  EXPECT_CALL(turtle, PenDown())                  // #3
      .Times(AtLeast(1));

  Painter painter(&turtle);                       // #4
  painter.DrawCircle(0, 0, 10);
//   EXPECT_TRUE(painter.DrawCircle(0, 0, 10));      // #5
}