#include <System/Seed_Gpu.h>

using namespace Seed;

cGpu::cGpu(GPU_TYPE_E eType)
	: _eType(eType), _deviceName(L""), _deviceId(0)
	, _totalMemSize(0), _usedMemSize(0), _temperature(0)
{

}

cGpu::~cGpu()
{

}