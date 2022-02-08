#include <Common/Seed_String.h>

#include <iostream>

using namespace Seed;


int32 main(int32 argc, char* argv[])
{
	cString temp = L"Test";
	std::cout << temp.to_string().c_str() << std::endl;
}