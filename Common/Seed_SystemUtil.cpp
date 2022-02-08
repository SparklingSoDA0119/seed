#include <Common/Seed_SystemUtil.h>

namespace Seed {

	namespace TickUtil {

		uint64 currentTick()
		{
			std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();

			auto currTick = std::chrono::duration_cast<std::chrono::milliseconds>(currTime.time_since_epoch()).count();

			return static_cast<uint64>(currTick);
		}

		uint64 diffTick(uint64 t1, uint64 t2)
		{
			return (t1 <= t2) ? t2 - t1 : (std::numeric_limits<uint64>::max() - t1) + t2 + 1;
		}
	}
}