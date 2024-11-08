#include <gtest/gtest.h>
#include "../src/core/DataTypes.h"
#include <cmath>

TEST(PointTestSuite, PointZeroInitialization) {

    Point pnt;
    ASSERT_FLOAT_EQ(pnt.x, 0.0);
    ASSERT_FLOAT_EQ(pnt.y, 0.0);
}

TEST(PointTestSuite, PointPositiveInitialization) {

    Point pnt{100.123f, 200.456f};
    ASSERT_FLOAT_EQ(pnt.x, 100.123f);
    ASSERT_FLOAT_EQ(pnt.y, 200.456f);
}

TEST(PointTestSuite, PointNegativeInitialization) {

    Point pnt{-100.123f, -200.456f};
    ASSERT_FLOAT_EQ(pnt.x, -100.123f);
    ASSERT_FLOAT_EQ(pnt.y, -200.456f);
}

TEST(PointTestSuite, TwoDefaultInitPointsEqual) {

    Point pnt1, pnt2;
    ASSERT_TRUE(pnt1 == pnt2);
}

TEST(PointTestSuite, TwoInitPointsEqual) {

    Point pnt1{1.23f}, pnt2{1.23f};
    ASSERT_TRUE(pnt1 == pnt2);
}

TEST(PointTestSuite, TwoPointsNotEqual) {

    Point pnt1{1.23f}, pnt2{1.24f};
    ASSERT_FALSE(pnt1 == pnt2);
}

 TEST(LootInfoTestSuite, LootInfoDefaultInitialization) {

    LootInfo off;
    ASSERT_EQ(off.id, 0);
    ASSERT_EQ(off.type, 0);
    ASSERT_FLOAT_EQ(off.x, 0.0f);
    ASSERT_FLOAT_EQ(off.y, 0.0f);
}

TEST(LootInfoTestSuite, LootInfoPositiveInitialization) {

    LootInfo off{1, 2, 3.123f, 4.678f};
    ASSERT_EQ(off.id, 1);
    ASSERT_EQ(off.type, 2);
    ASSERT_FLOAT_EQ(off.x, 3.123f);
    ASSERT_FLOAT_EQ(off.y, 4.678f);
}

TEST(LootInfoTestSuite, LootInfoNegativeInitialization) {

    LootInfo off{-1, -2, -3.123f, -4.678f};
    ASSERT_EQ(off.id, -1);
    ASSERT_EQ(off.type, -2);
    ASSERT_FLOAT_EQ(off.x, -3.123f);
    ASSERT_FLOAT_EQ(off.y, -4.678f);
}

TEST(RoadInfoTestSuite, LootInfoDefaultInitialization) {

    RoadInfo info;
    ASSERT_EQ(info.road_index, 0);
    ASSERT_EQ(info.road_type, RoadMutualPositionType::Parallel);
}

TEST(RoadInfoTestSuite, LootInfoAdjacentInitialization) {

    RoadInfo info{1, RoadMutualPositionType::Adjacent};
    ASSERT_EQ(info.road_index, 1);
    ASSERT_EQ(info.road_type, RoadMutualPositionType::Adjacent);
}

TEST(RoadInfoTestSuite, LootInfoCrossedInitialization) {

    RoadInfo info{1000, RoadMutualPositionType::Crossed};
    ASSERT_EQ(info.road_index, 1000);
    ASSERT_EQ(info.road_type, RoadMutualPositionType::Crossed);
}


TEST(OffsetTestSuite, OffsetZeroInitialization) {

    Offset pnt;
    ASSERT_FLOAT_EQ(pnt.dx, 0.0);
    ASSERT_FLOAT_EQ(pnt.dy, 0.0);
}

TEST(OffsetTestSuite, OffsetPositiveInitialization) {

    Offset pnt{110.123f, 220.456f};
    ASSERT_FLOAT_EQ(pnt.dx, 110.123f);
    ASSERT_FLOAT_EQ(pnt.dy, 220.456f);
}

TEST(OffsetTestSuite, OffsetNegativeInitialization) {

    Offset pnt{-1100.123f, -1200.456f};
    ASSERT_FLOAT_EQ(pnt.dx, -1100.123f);
    ASSERT_FLOAT_EQ(pnt.dy, -1200.456f);
}

