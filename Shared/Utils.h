#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>

namespace utils
{
	namespace str
	{
		// convert a number to a string.
		template<typename T>
		std::string ToString(T t)
		{
			std::ostringstream s;
			s << t;
			return s.str();
		}

		// convert string to a number.
		template <class T>
		bool FromString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
		{
			std::istringstream iss(s);
			return !(iss >> f >> t).fail();
		}
	}

	static int IsMachineLittleEndian()
	{
		int flag;
		char magic[4] = {0x01, 0x02, 0x03, 0x04};
		unsigned int *longMagic = (unsigned int *) magic;
	
		if (*longMagic == 0x01020304) 
		{
			flag = 0;
		} 
		else if (*longMagic == 0x04030201) 
		{
			flag = 1;
		} 
		else 
		{
			std::cerr << "Cannot detect machine Endianness, Giving Up." << std::endl;
			return -1;
		}
		return flag;
	}
}

#endif

