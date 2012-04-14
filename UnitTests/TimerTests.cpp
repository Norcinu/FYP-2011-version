#include "TimerTests.h"

TEST(TimerTest, ResetSeconds)
{
	Timer t;
	EXPECT_EQ(0, t.Seconds());
}

