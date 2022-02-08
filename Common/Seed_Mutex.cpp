#include <Common/Seed_Mutex.h>

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#endif

#if !defined(VIX_MUTEX_DUMP_LOCK_INTERVAL)
    #define VIX_MUTEX_DUMP_LOCK_INTERVAL    (30 * 60 * 1000)
#endif

using namespace Seed;

cMutex::cMutex(bool interprocess)
#if defined(_MSC_VER)
    : _interProcess(interprocess)
    #define _criticalSection static_cast<CRITICAL_SECTION*>(_handle)
#endif
{
#if defined(_MSC_VER)
    if (_interProcess) {
        _handle = ::CreateMutex(0, FALSE, 0);
    }
    else {
        _handle = new CRITICAL_SECTION;
        ::InitializeCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_init(&_handle, 0);
#endif
}

cMutex::~cMutex()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::WaitForSingleObject(_handle, INFINITE);
    }
    else {
        ::EnterCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_destroy(&_handle);
#endif
}

void cMutex::lock()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::ReleaseMutex(_handle);
    }
    else {
        ::LeaveCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_lock(&_handle);
#endif
}

void cMutex::unlock()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::ReleaseMutex(_handle);
}
    else {
        ::LeaveCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_unlock(&_handle);
#endif
}

cRecursiveMutex::cRecursiveMutex(bool interprocess)
#if defined(_MSC_VER)
    : _interProcess(interprocess)
    #define _criticalSection static_cast<CRITICAL_SECTION*>(_handle)
#endif
{
#if defined(_MSC_VER)
    if (_interProcess) {
        _handle = ::CreateMutex(0, FALSE, 0);
    }
    else {
        _handle = new CRITICAL_SECTION;
        ::InitializeCriticalSection(_criticalSection);
    }
#else
    pthread_mutexattr_t attr;
    ::pthread_mutexattr_init(&attr);
    ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    ::pthread_mutex_init(&_handle, &attr);
#endif
}

cRecursiveMutex::~cRecursiveMutex()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::CloseHandle(_handle);
}
    else {
        ::DeleteCriticalSection(_criticalSection);
        delete _criticalSection;
    }
#else
    ::pthread_mutex_destroy(&_handle);
#endif
}

void cRecursiveMutex::lock()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::WaitForSingleObject(_handle, INFINITE);
    }
    else {
        ::EnterCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_lock(&_handle);
#endif
}

void cRecursiveMutex::unlock()
{
#if defined(_MSC_VER)
    if (_interProcess) {
        ::ReleaseMutex(_handle);
    }
    else {
        ::LeaveCriticalSection(_criticalSection);
    }
#else
    ::pthread_mutex_unlock(&_handle);
#endif
}
