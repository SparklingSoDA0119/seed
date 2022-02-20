#ifndef _SAMPLE_APP_H_
#define _SAMPLE_APP_H_

#include <Common/Seed_Queue.h>
#include <deque>
#include <System/Seed_System.h>

namespace Seed {

	
	class cSampleApp : public cLooper {
	public :
		cSampleApp();
		virtual ~cSampleApp();

	private :
		bool _isRun;
		std::unique_ptr<cSystem> _pSystem;

	public :
		int32 initailization();

	protected :
		void looperFunc();

	};
}

#endif