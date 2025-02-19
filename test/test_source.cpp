#include <gtest/gtest.h>
#include "header.hpp"
    
TEST(TestTopic, TrivialEquality) {
    EXPECT_EQ(get_integer(), 42);
}
    
TEST(TestTopic, MoreEqualityTests) {
  ASSERT_EQ(get_integer(), 0) << "Something went wrong!";
  EXPECT_FLOAT_EQ(23.23F, 23.23F);
}
