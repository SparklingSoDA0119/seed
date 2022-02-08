#ifndef _SEED_QUEUE_H_
#define _SEED_QUEUE_H_

#include <Common/Seed_Looper.h>
#include <deque>

namespace Seed {

    template <typename T>
    class cQueue : public cLooper {
    public :
        cQueue(const cString& name = L"Queue", cThreadManager* pThreadManager = nullptr, 
               uint32 limitCount = 1000, uint32 sleepMs = 30, bool useReset = true)
            : cLooper(name, pThreadManager)
            , _isRun(false), _useReset(useReset)
            , _limitCount(limitCount), _sleepMs(sleepMs)
        {

        }
        virtual ~cQueue()
        {
            _isRun = false;
            if (!_queue.empty()) {
                _queue.clear();
            }
        }

    private :
        bool _isRun;
        bool _useReset;
        uint32 _limitCount;
        uint32 _sleepMs;
        
        std::deque<T> _queue;

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
        int32 pushData(T& data)
        {
            cAutoLock al(_lock);
            if (_queue.size() < _limitCount) {
                _queue.push_back(data);    
            }
            return SEED_SUCCESS;
        }

        T& frontData() { return _queue.front(); }
        int32 popFront()
        {
            cAutoLock al(_lock);
            if (!_queue.empty()) {
                _queue.pop_front();
            }

            return SEED_SUCCESS;
        }

        int32 popBack()
        {
            cAutoLock al(_lock);
            if (!_queue.empty()) {
                _queue.pop_back();
            }
        }

        void clear()
        {
            cAutoLock al(_lock);
            if (!_queue.empty()) {
                _queue.clear();
            }
        }

    protected :
        virtual void looperFunc() override = 0;        
    };
}

#endif