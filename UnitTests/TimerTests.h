#ifndef TIMER_TESTS_H
#define TIMER_TESTS_H

#include <gtest/gtest.h>
#include "../Shared/Timer.h"

class TimerTest : public ::testing::Test
{
protected:
	TimerTest() {}
	virtual ~TimerTest() {}
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#endif
