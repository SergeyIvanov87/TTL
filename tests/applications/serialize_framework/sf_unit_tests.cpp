#include <gtest/gtest.h>


TEST(HelloTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
}
