#include <System/Seed_NetworkDevice.h>

using namespace Seed;

cNetworkDevice::cNetworkDevice()
	: _macAddr(L""), _ipAddr(L"")
	, _rxDataSizePerSec(0), _txDataSizePerSec(0)
{

}

cNetworkDevice::~cNetworkDevice()
{

}