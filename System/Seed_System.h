#ifndef _SEED_SYSTEM_H_
#define _SEED_SYSTEM_H_

#include <Common/Seed_Object.h>
#include <System/Seed_Cpu.h>

#include <vector>

namespace Seed {

	enum class OS_TYPE_E {
		WINDOWS10 = 1,
		WINDOWS11,
		UBUNTU1804,
		UBUNTU2004,
		MAX_OS_TYPE,
	};

	class cSystem : public cObject {
	public :
		cSystem(OS_TYPE_E eOsType = OS_TYPE_E::MAX_OS_TYPE)
			: _eOsType(eOsType)
		{

		}

		virtual ~cSystem() {}

	private :
		OS_TYPE_E _eOsType;

	protected :
		std::vector<std::unique_ptr<cCpu>> _cpus;

	public :
		OS_TYPE_E osType() { return _eOsType; }

	public :
		virtual ERR_TYPE_E loadSystem() = 0;
		virtual ERR_TYPE_E loadCpu() = 0;

	};
}

#endif