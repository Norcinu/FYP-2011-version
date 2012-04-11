#include <gtest/gtest.h>

class StringMessageBufferTest :  public ::testing::Test
{
protected:
	StringMessageBufferTest() {}
	virtual ~StringMessageBufferTest() {}

	virtual void SetUp() 
	{
	}

	virtual void TearDown()
	{
	}
};

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

