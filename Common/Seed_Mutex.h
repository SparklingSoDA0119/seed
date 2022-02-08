#ifndef _SEED_MUTEX_H
#define _SEED_MUTEX_H

#include <Common/Seed_Object.h>

#if !defined(_MSC_VER)
    #include <pthread.h>
#endif

namespace Seed {
    class cMutexLockable : public cObject {
    public:
        virtual ~cMutexLockable() {}
        virtual void lock() = 0;
        virtual void unlock() = 0;
    };

    class cMutex : public cMutexLockable {
    public :
#if defined(_MSC_VER)
        typedef void* Handle;
#else
        typedef pthread_mutex_t Handle;
#endif
    
    public :
        cMutex(bool interprocess = true);

        virtual ~cMutex();

    private :
        Handle _handle;
#if defined(_MSC_VER)
        bool _interProcess;
#endif
    
    public :
        Handle& handle() { return _handle; }
        void lock();
        void unlock();

    private :    // UNCOPYABLE!!! (DO NOT IMPL.)
        cMutex(const cMutex&);
        cMutex& operator=(const cMutex&);
    };

    class cAutoMutex {
    public :
        explicit cAutoMutex(cMutexLockable& mutex)
            : _mutex(mutex) { _mutex.lock(); }

        ~cAutoMutex() { _mutex.unlock(); }
    
    private :
        cMutexLockable& _mutex;

    private :    // UNCOPYABLE!!! (DO NOT IMPL.)
        cAutoMutex(const cAutoMutex&);
        cAutoMutex& operator=(const cAutoMutex&);
    };

    class cRecursiveMutex : public cMutexLockable {
    public :
#if defined(_MSC_VER)
        typedef void* Handle;
#else
        typedef pthread_mutex_t Handle;
#endif

    public :
        cRecursiveMutex(bool interprocess = true);
        virtual ~cRecursiveMutex();

    private :
        Handle _handle;
#if defined(_MSC_VER)
        bool _interProcess;
#endif

    public :
        Handle& handle() { return _handle; }

        void lock();
        void unlock();

    private :    // UNCOPYABLE!!! (DO NOT IMPL.)
        cRecursiveMutex(const cRecursiveMutex&);
        cRecursiveMutex& operator=(const cRecursiveMutex&);
    };

}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_MUTEX_H
