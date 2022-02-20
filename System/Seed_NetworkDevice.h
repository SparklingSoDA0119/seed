#ifndef _SEED_NETWORK_DEVICE_H_
#define _SEED_NETWORK_DEVICE_H_

#include <Common/Seed_String.h>

namespace Seed {

	class cNetworkDevice : public cObject {
	public :
		cNetworkDevice();
		virtual ~cNetworkDevice();

	private :
		cString _macAddr;
		cString _ipAddr;
		uint64 _rxDataSizePerSec;
		uint64 _txDataSizePerSec;

	public :
		const cString macAddr() const { return _macAddr; }
		const cString ipAddr() const { return _ipAddr; }

		uint64 rxDataSizePerSec() const { return _rxDataSizePerSec; }
		uint64 txDataSizePerSec() const { return _txDataSizePerSec; }
	};
}

#endif
