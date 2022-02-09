#ifndef _SEED_QUEUE_H_
#define _SEED_QUEUE_H_

#include <Common/Seed_Looper.h>
#include <queue>

namespace Seed {

    template <typename T>
    class cQueue : public cLooper {
    public :
        cQueue(const cString& name = L"Queue", cThreadManager* pThreadManager = nullptr, 
               uint32 limitCount = 1000, uint32 sleepMs = 30, bool useReset = true);

        virtual ~cQueue();

    private :
        bool _isRun;
        bool _useReset;
        uint32 _limitCount;
        uint32 _sleepMs;
        
        std::queue<T> _queue;

        cLock _lock;

    public :
        bool isRun() const { return _isRun; }
        bool useReset() const { return _useReset; }
        uint32 limitCount() const { return _limitCount; }
        uint32 sleepMs() const { return _sleepMs; }

    public :
        void setUseReset(bool use) { _useReset = useReset; }
        void setLimitCount(uint32 count) { _limitCount = count; }
        void setSleepMs(uint32 sleepMs) { _sleepMs = sleepMs; }

    public :
        int32 pushData(T& data);

        T& frontData() { return _queue.front(); }

    protected :
        int32 runQueueThread();
        int32 popFront();
        int32 popBack();
        void clear();

    public :
        virtual int32 initialization() = 0;

    protected :
        virtual void looperFunc() override = 0;        
    };
    
}

#endif