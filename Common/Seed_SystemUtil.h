#ifndef _SEED_SYSTEM_UTIL_H_
#define _SEED_SYSTEM_UTIL_H_

#include <Common/Seed_String.h>

#include <vector>
#include <list>
#include <chrono>
#include <unistd.h>

namespace Seed {

	namespace TickUtil {
		uint64 currentTick();
		uint64 diffTick(uint64 t1, uint64 t2);
	}

	namespace FileUtil {
		ERR_TYPE_E isExistFile(const cString& fileName);
	}
}

#endif