#ifndef BUFFER_TESTS_H
#define BUFFER_TESTS_H

#include <gtest/gtest.h>

class StringBufferTest :  public ::testing::Test
{
protected:
	StringBufferTest() {}
	virtual ~StringBufferTest() {}

	virtual void SetUp() {}
		virtual void TearDown() {}
};

class MessageBufferTest : public ::testing::Test
{
protected:
	MessageBufferTest() {}
	virtual ~MessageBufferTest() {}

	virtual void SetUp() {}
	virtual void TearDown() {}
};

#endif
