#include <Common/Seed_String.h>
#include <Sample/SampleApp.h>
#include <iostream>

using namespace Seed;


int32 main(int32 argc, char* argv[])
{
	cString temp = L"Test";

	std::cout << temp.to_string().c_str() << std::endl;

	cSampleApp sample;
	sample.initailization();

	sample.join();
	sample.quit();
	
	return 0;
}