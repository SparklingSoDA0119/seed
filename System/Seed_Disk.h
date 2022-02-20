#ifndef _SEED_DISK_H_
#define _SEED_DISK_H_

#include <Common/Seed_Object.h>
#include <Common/Seed_String.h>

namespace Seed {

	class cDisk : public cObject {
	public :
		cDisk();
		virtual ~cDisk();

	private :
		uint64 _capacity;
		uint64 _usedSize;
		cString _deviceName;

	public :
		uint64 capacity() const { return _capacity; }
		uint64 usedSize() const { return _usedSize; }
		const cString deviceName() const { return _deviceName; }
	};
}

#endif
