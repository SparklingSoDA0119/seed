#ifndef _SEED_CPU_H_
#define _SEED_CPU_H_

#include <Common/Seed_Object.h>

namespace Seed {

	enum class CPU_TYPE_E {
		AMD_X86 = 1,
		AMD_X84_64,
		AMD64,
		MAX_CPU_TYPE,
	};

	class cCpu : public cObject {
	public :
		cCpu(CPU_TYPE_E eType = CPU_TYPE_E::MAX_CPU_TYPE)
			: _eCpuType(eType)
		{

		}

		virtual ~cCpu() {}

	private :
		CPU_TYPE_E _eCpuType;

	public :
		CPU_TYPE_E cpuType() { return _eCpuType; }
	};
}

#endif