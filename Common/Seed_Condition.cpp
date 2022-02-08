#include <Common/Seed_Condition.h>

#if defined(_MSC_VER)
#include <windows.h>
#else   // __linux__
#include <linux/unistd.h>
#include <sys/time.h>
#include <errno.h>      // ETIMEOUT, ...
#endif

using namespace Seed;

cCondition::cCondition()
    : _signaled(false)
{
#if defined(_MSC_VER)
    _handle = ::CreateEvent(0, FALSE, FALSE, 0);
#else
    ::pthread_cond_init(&_handle, 0);
#endif
}

cCondition::~cCondition()
{
#if defined(_MSC_VER)
    ::CloseHandle(_handle);
#else
    ::pthread_cond_destroy(&_handle);
#endif
}

////////////////////////////////////////////////////////////////////////////////

bool cCondition::wait(uint32 timeoutMillisec)
{
    bool signaled = false;

    _lock.lock();
    if (isSignaled()) {
        _lock.unlock();
        signaled = true;
    }
    else {
#if defined(_MSC_VER)
        _lock.unlock();    // unlock here
        MSG msg;
        DWORD ret;
        DWORD waiting = (0 == timeoutMillisec) ? INFINITE : timeoutMillisec;
        do {
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            ret = ::MsgWaitForMultipleObjects(1, &_handle,
                FALSE, waiting, QS_ALLINPUT);
        } while (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT);
        signaled = (ret == WAIT_OBJECT_0);
#else
        int32 ret = 0;
        if (0 < timeoutMillisec) {    // timed-wait
            struct timespec ts;
            struct timeval tv;
            ::gettimeofday(&tv, 0);

            uint64 nsec = static_cast<uint64>(timeoutMillisec) * 1000000 + (tv.tv_usec * 1000);
            ts.tv_sec = tv.tv_sec + nsec / 1000000000;
            ts.tv_nsec = nsec % 1000000000;
            ret = ::pthread_cond_timedwait(&_handle, &_lock.pthread_mutex(), &ts);
        }
        else {                      // inifite-wait
            ret = ::pthread_cond_wait(&_handle, &_lock.pthread_mutex());
        }
        _lock.unlock();    // unlock here
        signaled = (ret == 0);
#endif
    }

    reset();  // auto reset

    return signaled;
}

void cCondition::signal()
{
    cAutoLock al(_lock);

#if defined(_MSC_VER)
    ::SetEvent(_handle);
#else
    if (!_signaled) {
        _signaled = true;
        ::pthread_cond_signal(&_handle);
    }
#endif
}

void cCondition::reset()
{
    cAutoLock al(_lock);

    _signaled = false;
#if defined(_MSC_VER)
    ::ResetEvent(_handle);
#endif
}