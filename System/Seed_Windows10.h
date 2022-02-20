#ifndef _SEED_WINDOWS_10_H_
#define _SEED_WINDOWS_10_H_

#include <System/Seed_System.h>

namespace Seed {

	class cWin10System : public cSystem {
	public :
		cWin10System();
		virtual ~cWin10System();

	public :
		virtual ERR_TYPE_E loadSystem() override;
		virtual ERR_TYPE_E loadCpu() override;

	};
}

#endif
