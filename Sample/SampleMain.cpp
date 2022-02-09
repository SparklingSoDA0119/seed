#include <Common/Seed_String.h>
#include <Sample/SampleApp.h>
#include <iostream>
#include <csignal>

using namespace Seed;

void signalHandler(int32 signum)
{
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

	exit(signum);
}

void signalHanderRegist()
{
	signal(SIGINT, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGBREAK, signalHandler);
}

int32 main(int32 argc, char* argv[])
{
	cString temp = L"Test";

	std::cout << temp.to_string().c_str() << std::endl;

	signalHanderRegist();

	cSampleApp sample;
	sample.initailization();

	sample.join();
	sample.quit();
	
	return 0;
}