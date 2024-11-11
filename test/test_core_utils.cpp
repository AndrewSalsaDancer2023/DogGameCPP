#include <gtest/gtest.h>
#include "../src/core/Utils.h"
#include "./utils/test_utilities.h"
#include <cmath>

TEST(RandomIntTestSuite, ZeroOneRangeTest) {
    int minValue = 0, maxValue = 1;
    for(int i = 0; i < 10; ++i)
    {
        int res = utils::GetRandomIntNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomIntTestSuite, MinusOneZeroRangeTest) {
    int minValue = -1, maxValue = 0;
    for(int i = 0; i < 10; ++i)
    {
        int res = utils::GetRandomIntNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomIntTestSuite, ZeroRangeTest) {
    int minValue = 0, maxValue = 0;
    for(int i = 0; i < 10; ++i)
    {
        int res = utils::GetRandomIntNumber(minValue, maxValue);
        ASSERT_LE(res,minValue);
        ASSERT_GE(res,maxValue);
    }
}

TEST(RandomIntTestSuite, ZeroHundredRangeTest) {
    int minValue = 0, maxValue = 100;
    for(int i = 0; i < 20; ++i)
    {
        int res = utils::GetRandomIntNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomIntTestSuite, MinusHundredZeroRangeTest) {
    int minValue = -100, maxValue = 0;
    for(int i = 0; i < 20; ++i)
    {
        int res = utils::GetRandomIntNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

////////////////////////

TEST(RandomFloatTestSuite1, ZeroOneFloatRangeTest) {
    float minValue = 0.0;
    float maxValue = 1.0;
    for(int i = 0; i < 10; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomFloatTestSuite2, MinusOneZeroFloatRangeTest) {
    float minValue = -1.0; 
    float maxValue = 0.0;
    for(int i = 0; i < 10; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomFloatTestSuite3, ZeroRangeFloatTest) {
    float minValue = 0.0;
    float maxValue = 0.0;
    for(int i = 0; i < 10; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_LE(res,minValue);
        ASSERT_GE(res,maxValue);
    }
}

TEST(RandomFloatTestSuite4, ZeroHundredFloatRangeTest) {
    float minValue = 0.0; 
    float maxValue = 100.123;

    for(int i = 0; i < 20; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomFloatTestSuite5, MinusHundredZeroFloatRangeTest) {
    float minValue = -100.123;
    float maxValue = 0.0;
    for(int i = 0; i < 20; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(RandomFloatTestSuite6, ZeroHundredFloatRangeTest) {
    float minValue = 0.0;
    float maxValue = 100.123;
    for(int i = 0; i < 20; ++i)
    {
        float res = utils::GetRandomFloatNumber(minValue, maxValue);
        ASSERT_GE(res,minValue);        
        ASSERT_LE(res,maxValue);
    }
}

TEST(GenerateRandomTokenTestSuite, ValidationTest) {
    utils::PlayerToken generator;

    for(int i = 0; i < 50; ++i)
    {
        auto res = generator.GetToken();
        ASSERT_EQ(res.length(), utils::default_token_size_);
        ASSERT_TRUE(test_utils::isValidToken(res));
    }
}


TEST(GenerateUniqueTokenTestSuite, UniqueTokensTest) {
    utils::PlayerToken generator;
    std::set<std::string> tokens;
    for(int i = 0; i < 50; ++i)
        tokens.insert(generator.GetToken());
    
    ASSERT_EQ(tokens.size(), 50);
}