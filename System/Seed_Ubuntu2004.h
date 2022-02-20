#ifndef _SEED_UBUNTU_20_04_H_
#define _SEED_UBUNTU_20_04_H_

#include <System/Seed_System.h>

namespace Seed {

	class cUbuntu2004 : public cSystem {
	public :
		cUbuntu2004();
		virtual ~cUbuntu2004();

	public :
		virtual ERR_TYPE_E loadCpu() override;
	};
}

#endif
