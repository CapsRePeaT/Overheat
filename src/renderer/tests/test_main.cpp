#include <gtest/gtest.h>

TEST(TestTest, BasicAssertions) {
	EXPECT_STRNE("hello", "world");
	EXPECT_EQ(7 * 6, 42);
}
