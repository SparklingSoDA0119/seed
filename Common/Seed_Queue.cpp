#include <Common/Seed_Queue.h>

using namespace Seed;

template <typename T>
cQueue<T>::cQueue(const cString& name, cThreadManager* pThreadManager,
    uint32 limitCount, uint32 sleepMs, bool useReset)
    : cLooper(name, pThreadManager)
    , _isRun(false), _useReset(useReset)
    , _limitCount(limitCount), _sleepMs(sleepMs)
{

}

template <typename T>
cQueue<T>::~cQueue()
{
    _isRun = false;

    if (!_queue.empty()) {
        _queue.clear();
    }
}

template <typename T>
int32 cQueue<T>::runQueueThread()
{
    if (isRun()) {
        return SEED_FAILURE;
    }

    if (!isAlive()) {
        run();
    }
    else {
        resume();
    }

    _isRun = true;

    return SEED_SUCCESS;
}

template <typename T>
int32 cQueue<T>::pushData(T& data)
{
    cAutoLock al(_lock);
    if (_queue.size() < _limitCount) {
        _queue.push_back(data);
    }
    return SEED_SUCCESS;
}

template <typename T>
int32 cQueue<T>::popFront()
{
    cAutoLock al(_lock);
    if (!_queue.empty()) {
        _queue.pop_front();
    }

    return SEED_SUCCESS;
}

template <typename T>
int32 cQueue<T>::popBack()
{
    cAutoLock al(_lock);
    if (!_queue.empty()) {
        _queue.pop_back();
    }
}

template <typename T>
void cQueue<T>::clear()
{
    cAutoLock al(_lock);
    if (!_queue.empty()) {
        _queue.clear();
    }
}