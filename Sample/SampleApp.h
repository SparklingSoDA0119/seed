#ifndef _SAMPLE_APP_H_
#define _SAMPLE_APP_H_

#include <Common/Seed_Queue.h>
#include <Sample/SampleQueue.h>
#include <deque>

namespace Seed {

	
	class cSampleApp : public cLooper {
	public :
		cSampleApp();
		virtual ~cSampleApp();

	private :
		bool _isRun;
		cSampleQueue _queue;

	public :
		int32 initailization();

	protected :
		void looperFunc();

	};
}

#endif