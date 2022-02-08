#include <Common/Seed_Thread.h>
#include <Common/Seed_Exception.h>
#include <Common/Seed_SystemUtil.h>
#include <Common/Seed_StringUtil.h>
#include <Common/Seed_Assert.h>

#include <stdexcept>
#include <exception>    // bad_exception
#include <typeinfo>     // bad_cast, bad_typeid
#include <new>          // bad_alloc
#include <cstdlib>      // abort()
#include <stdio.h>

#if defined(_MSC_VER)
#include <process.h>
#else
#include <linux/unistd.h>
#include <sys/prctl.h>

#if !defined(__ETI__)
#include <syscall.h>
namespace { // unnamed
    inline pid_t gettid() { return (pid_t)syscall(__NR_gettid); }
}   // unnamed namespace
#else
#define gettid  getpid
#endif
#endif

using namespace Seed;

uint32 cThread::DEFAULT_THREAD_AFFINITY_MASK_VALUE = 0;
const uint32& cThread::DEFAULT_THREAD_AFFINITY_MASK = cThread::DEFAULT_THREAD_AFFINITY_MASK_VALUE;

cThread::cThread(const cString& name, cThreadManager* manager)
    : _id(INVALID_ID), _handle(0),
    _name(name), _priority(PRIORITY_NORMAL), _policy(POLICY_NORMAL), _threadAffinityMask(0),
    _manager(manager ? manager : &cThreadManager::defaultManager()),
    _status(STATUS_READY)
{
}

cThread::~cThread()
{
}

void cThread::setPriority(uint32 priority, Policy policy)
{
    assert(PRIORITY_LOWEST <= priority && priority <= PRIORITY_HIGHEST);
    _priority = priority;
    _policy = policy;

    if (!_handle) {
        return;
    }

    if (!isAlive()) {
        return;
    }
#if defined(_MSC_VER)
    int32 pri;
    if (_policy == POLICY_REALTIME) {
        pri = (PRIORITY_HIGHEST <= _priority) ? THREAD_PRIORITY_TIME_CRITICAL :
            (PRIORITY_NORMAL <= _priority) ? THREAD_PRIORITY_HIGHEST :
            (PRIORITY_LOWEST <= _priority) ? THREAD_PRIORITY_ABOVE_NORMAL :
            THREAD_PRIORITY_NORMAL;
    }
    else {  // POLICY_NORMAL
        pri = (PRIORITY_HIGHEST <= _priority) ? THREAD_PRIORITY_ABOVE_NORMAL :
            (PRIORITY_NORMAL <= _priority) ? THREAD_PRIORITY_NORMAL :
            (PRIORITY_LOWEST <= _priority) ? THREAD_PRIORITY_BELOW_NORMAL :
            THREAD_PRIORITY_LOWEST;
    }
    ::SetThreadPriority(_handle, pri);
#else
    int32 pol;
    struct sched_param param;
    ::pthread_getschedparam(_handle, &pol, &param);
    if (_policy == POLICY_REALTIME) {
        param.sched_priority = _priority;
        pol = SCHED_FIFO;
    }
    else if (_policy == POLICY_REALTIME_RR) {
        param.sched_priority = _priority;
        pol = SCHED_RR;
    }
    else {
        param.sched_priority = 0;
        pol = SCHED_OTHER;
    }

    ::pthread_setschedparam(_handle, pol, &param);
#endif
}

bool cThread::setAffinityMask(uint32 mask)
{
    _threadAffinityMask = mask;

    if (!_handle) {
        return false;
    }

    if (!isAlive()) {
        return false;
    }

#if defined(_MSC_VER)
    if (0 == ::SetThreadAffinityMask(_handle, static_cast<DWORD_PTR>(mask))) {
        ////cTRACE(L"G2Thread[\"%@\"] have failed to set thread affinity mask.", &name());
        return false;
    }
    return true;
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    int32 maxCores = ::sysconf(_SC_NPROCESSORS_ONLN);
    maxCores = 32 < maxCores ? 32 : maxCores;
    for (int32 i = 0; i < maxCores; ++i) {
        if (mask & (0x1 << i)) {
            CPU_SET(i, &cpuset);
        }
    }

    // bind thread
    if (::pthread_setaffinity_np(_handle, sizeof(cpu_set_t), &cpuset) < 0) {
        //cTRACE(L"cThread[\"%s\"] have failed to set thread affinity mask.", &name());
        return false;
    }
#endif
    return true;
}

void cThread::readyThread()
{
    //cTRACE(L"cThread[\"%s\"] is starting.", &name());
    setStatus(STATUS_STARTING);
    starting();
#if 0
    cNotificationCenter& nc = cNotificationCenter::defaultCenter();
    nc.postNotification(cNotification(cTHREAD_STARTING_NOTIFICATION, this));
#endif
}

void cThread::initThread()
{
#if defined(_MSC_VER)
    _id = static_cast<uint32>(::GetCurrentThreadId());
#else
    _id = static_cast<uint32>(::gettid());
#endif

    _manager->registerThread(this);

    if (_threadAffinityMask == 0 &&
        DEFAULT_THREAD_AFFINITY_MASK > 0) {
        _threadAffinityMask = DEFAULT_THREAD_AFFINITY_MASK;
    }

    if (_threadAffinityMask > 0) {
        setAffinityMask(_threadAffinityMask);
    }

    setPriority(_priority, _policy);
}

void cThread::runThread()
{
#if 0
    cNotificationCenter& nc = cNotificationCenter::defaultCenter();
#endif

#if defined(__linux__)
    ::prctl(PR_SET_NAME, _name.to_string().c_str(), 0, 0, 0);
#endif

    //cTRACE(L"cThread[%s] is started.", _name);
    started();

    //nc.postNotification(cNotification(cTHREAD_STARTED_NOTIFICATION, this));

    setStatus(STATUS_WORKING);
    threadFunc();

    //cTRACE(L"cThread[%s] is finished.", &name());
    setStatus(STATUS_FINISHING);
    finished();
    //nc.postNotification(cNotification(cTHREAD_FINISHED_NOTIFICATION, this));
}

void cThread::cleanupThread()
{
    setStatus(STATUS_READY);

    _manager->unregisterThread(this);

#if defined(_MSC_VER)
    if (_handle) {
        ::CloseHandle(_handle);
    }
#else
    _id = INVALID_ID;
#endif

    //cTRACE(L"cThread[\"%s\"] is terminated.", &name());
    terminated();
    //cNotificationCenter& nc = cNotificationCenter::defaultCenter();
    //nc.postNotification(cNotification(cTHREAD_TERMINATED_NOTIFICATION, this));
}

void cThread::setStatus(Status status)
{
    cAutoLock al(_statusLock);
    _status = status;
}

bool cThread::startingThread()
{
    if (!performWithoutException(&cThread::readyThread)) {
        setStatus(STATUS_READY);
        return false;
    }
    return true;
}

void cThread::_coreThreadFunc()
{
    if (performWithoutException(&cThread::initThread)) {
        performWithoutException(&cThread::runThread);
    }
    performWithoutException(&cThread::cleanupThread);
}

#if defined(_MSC_VER)
unsigned int WINAPI cThread::coreThreadFunc(void* param)
{
    cThread* thread = reinterpret_cast<cThread*>(param);
    thread->_coreThreadFunc();
    return 0;
}
#else
void* cThread::coreThreadFunc(void* param)
{
    cThread* thread = reinterpret_cast<cThread*>(param);

    thread->_coreThreadFunc();
    return 0;
}
#endif

bool cThread::run(bool waitUntilStarted)
{
    assert(!isAlive());
    assert(!_handle);

    if (!startingThread()) {
        return false;
    }

    bool success;
#if defined(_MSC_VER)
    DWORD id;
    _handle = (HANDLE)(ULONG_PTR)
        ::_beginthreadex(0, 0, coreThreadFunc, this, 0, reinterpret_cast<UINT*>(&id));
    int32 errorCode = GetLastError();
    success = (_handle != 0);
#else
    int32 errorCode = ::pthread_create(&_handle, 0, coreThreadFunc, this);
    success = (0 == errorCode);
#endif


    if (!success) {
        printf("cThread::run Failed %s(%d)\n", _name.to_string().c_str(), errorCode);
        setStatus(STATUS_READY);
        return false;
    }

    if (waitUntilStarted) {
        while (_status == STATUS_STARTING) {
            ////cTRACE(L"cThread[%@]::run() is waiting for starting.", this);
            cThread::yield();
        }
    }
    return true;
}

void cThread::quit()
{
    if (isAlive()) {
        //cTRACE(L"cThread[%s] is quit-requested", &name());
        quitRequested();
    }
}

void cThread::join()
{
    if (!_handle) {
        return;
    }

    //cTRACE(L"cThread[%s] is joining...", &name());
#if defined(_MSC_VER)
    while (isAlive()) {
        cThread::sleep(10);
    }
#else
    void* p = 0;
    ::pthread_join(_handle, &p);
#endif

    _handle = 0;

    //cTRACE(L"cThread[\"%s\"] is joined", &name());
}

#if defined(_MSC_VER)
void cThread::yield() { ::SwitchToThread(); }
void cThread::sleep(uint32 msec) { ::Sleep(msec); }
#else
void cThread::yield() { ::sched_yield(); }
void cThread::sleep(uint32 msec)
{
    struct timespec ts_sleep, ts_remaining;
    ts_sleep.tv_sec = msec / 1000;
    ts_sleep.tv_nsec = (msec % 1000) * 1000000;
    ::nanosleep(&ts_sleep, &ts_remaining);
}
#endif

void cThread::checkTimers()
{
    cAutoLock al(_timersLock);

    uint64 tick = TickUtil::currentTick();
    std::list<TIMER_S>::iterator i = _timers.begin();
    while (i != _timers.end()) {
        cTimer* timer = (*i).timer.get();
        if (timer->interval() <= TickUtil::diffTick(i->lastCheckTick, tick)) {
            i->lastCheckTick = tick;
            (timer->target()->*timer->action())(timer);
            if (!timer->isRepeat()) {
                _timers.erase(i++);
            }
            else {
                ++i;
            }
        }
        else {
            ++i;
        }
    }
}

#undef addTimer
cTimer* cThread::addTimer(uint32 interval, bool repeat,
    cObject* target, cTimerAction action)
{
    cAutoLock al(_timersLock);

    if (0 < interval && target && action) {
        _timers.push_back(TIMER_S());
        TIMER_S& t = _timers.back();
        t.timer.reset(new cTimer(interval, repeat, target, action));
        t.lastCheckTick = TickUtil::currentTick();
        return t.timer.get();
    }
    return 0;
}

void cThread::restartTimer(cTimer* timer)
{
    cAutoLock al(_timersLock);

    std::list<TIMER_S>::iterator i;
    for (i = _timers.begin(); i != _timers.end(); ++i) {
        if (i->timer.get() == timer) {
            i->lastCheckTick = TickUtil::currentTick();
            break;
        }
    }
}

void cThread::removeTimer(cTimer* timer)
{
    cAutoLock al(_timersLock);

    std::list<TIMER_S>::iterator i;
    for (i = _timers.begin(); i != _timers.end(); ++i) {
        if (i->timer.get() == timer) {
            _timers.erase(i);
            break;
        }
    }
}

void cThread::removeAllTimers()
{
    cAutoLock al(_timersLock);

    _timers.clear();
}

cString cThread::toString() const
{
    cString s;
    s.format(L"\"%s\"(%u)", &name(), id());
    return s;
}

