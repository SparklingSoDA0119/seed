#include <System/Seed_Windows10.h>

using namespace Seed;

cWin10System::cWin10System()
	//: cSystem(OS_TYPE_E::WINDOWS10)
	: cSystem()
{

}

cWin10System::~cWin10System()
{

}

ERR_TYPE_E cWin10System::loadSystem()
{
	ERR_TYPE_E eErr = ERR_TYPE_E::SUCCESS;
	
	/* Load Cpu Information */
	eErr = loadCpu();
	if (eErr != ERR_TYPE_E::SUCCESS) {
		return eErr;
	}

	return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cWin10System::loadCpu()
{
	return ERR_TYPE_E::SUCCESS;
}

