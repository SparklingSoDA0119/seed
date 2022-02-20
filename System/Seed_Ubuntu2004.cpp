#include <System/Seed_Ubuntu2004.h>

using namespace Seed;

cUbuntu2004::cUbuntu2004()
	: cSystem(OS_TYPE_E::UBUNTU2004)
{

}

cUbuntu2004::~cUbuntu2004()
{

}

ERR_TYPE_E cUbuntu2004::loadCpu()
{
	return ERR_TYPE_E::SUCCESS;
}