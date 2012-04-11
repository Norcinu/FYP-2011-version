#include "TimerTests.h"

TEST(Timer, ResetSeconds)
{
	Timer t;
	EXPECT_EQ(0, t.Seconds());
}

