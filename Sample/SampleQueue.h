#ifndef _SAMPLE_QUEUE_H_
#define _SAMPLE_QUEUE_H_

#include <Common/Seed_Lock.h>
#include <Common/Seed_Looper.h>
#include <deque>

namespace Seed {

    typedef struct sample_data_s {
        int32 _data;
        cString _dataStr;

        sample_data_s() : _data(0), _dataStr(L"") {}
    } SAMPLE_DATA_S;

    class cSampleQueue : public cLooper {
    public :
        cSampleQueue();
        virtual ~cSampleQueue();

    private :
        bool _isRun;
        std::deque<SAMPLE_DATA_S> _dataQue;
        cLock _lock;
        
    public :
        int32 initialization();
        int32 pushData(SAMPLE_DATA_S stData);

    protected :
        virtual void looperFunc();

    private :
        int32 checkQueue();
    };
}
#endif