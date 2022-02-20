#ifndef _SEED_GPU_H_
#define _SEED_GPU_H_

#include <Common/Seed_String.h>

namespace Seed {

	enum class GPU_TYPE_E {
		NONE = 0,
		RTX10XX,
		RTX30XX,
	};

	class cGpu : public cObject {
	public :
		cGpu(GPU_TYPE_E eType = GPU_TYPE_E::NONE);
		virtual ~cGpu();

	private :
		GPU_TYPE_E _eType;

	protected :
		cString _deviceName;
		uint32 _deviceId;
		uint64 _totalMemSize;
		uint64 _usedMemSize;
		uint32 _temperature;

	public :
		GPU_TYPE_E type() const { return _eType; }
		const cString deviceName() const { return _deviceName; }
		uint32 deviceId() const { return _deviceId; }
		uint64 totalMemSize() const { return _totalMemSize; }
		uint64 usedMemSize() const { return _usedMemSize; }
		uint32 temperature() const { return _temperature; }
	};
}

#endif
