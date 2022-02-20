#include <System/Seed_Disk.h>

using namespace Seed;

cDisk::cDisk()
	: _capacity(1 * 1024 * 1024 * 1024), _usedSize(0)
	, _deviceName(L"/dev/disk0")
{

}

cDisk::~cDisk()
{

}