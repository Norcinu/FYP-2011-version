#include <iostream>
#include "../Shared/InBuffer.h"
#include "../Shared/Utils.h"
#include "../Shared/Message.h"

int main(int argc, char *argv[])
{
	INIT_IN_BUFFER;
	if (IN_BUFFER.Empty())
	{
		std::cout << "buffer is empty" << std::endl;
		for (int i = 0; i < 10; ++i)
		{
			IN_BUFFER.PushMessage(std::string("value: " + utils::str::ToString<int>(i)));
		}
	}

	while (!IN_BUFFER.Empty())
	{
		Message msg = IN_BUFFER.Front();
		std::cout << msg << std::endl;
	}

	DESTROY_IN_BUFFER;
}
