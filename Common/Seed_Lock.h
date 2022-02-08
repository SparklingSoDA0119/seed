#ifndef _SEED_LOCK_H_
#define _SEED_LOCK_H_

#include <Common/Seed_Object.h>
#include <mutex>
#include <shared_mutex>

namespace Seed {

    class cAutoLockable : public cObject {
    public :
        virtual ~cAutoLockable() {}
        virtual void lock() const = 0;
        virtual void unlock() const = 0;
    };

    class cAutoLock {
    public :
        explicit cAutoLock(cAutoLockable& lock) : _lock(lock) { _lock.lock(); }
        ~cAutoLock() { _lock.unlock(); }

    private :
        cAutoLockable& _lock;

    private:    // UNCOPYABLE!!! (DO NOT IMPL.)
        cAutoLock(const cAutoLock&);
        cAutoLock& operator=(const cAutoLock&);
    };

#define DEFINE_SEED_LOCK_DEFAULT_IMPL(Class)                                    \
    public :                                                                    \
        Class() {}                                                              \
        virtual ~Class() {}                                                     \
    private :    /* UNCOPYABLE!!! (DO NOT IMPL.) */                             \
        Class(const Class&);                                                    \
        Class& operator=(const Class&);                                         \
    public :                                                                    \
        bool tryLock() const { return _mutex.try_lock(); }                      \
        virtual void lock() const { _mutex.lock(); }                            \
        virtual void unlock() const { _mutex.unlock(); }                        
    
#if defined(_MSC_VER)
#define DEFINE_SEED_LOCK_TIMED_LOCK_IMPL(Class)                                             \
    public :                                                                                \
        bool timedLock(uint32 timeOutMilisec) const                                         \
        {                                                                                   \
            return _mutex.try_lock_for(std::chrono::milliseconds(timeOutMilisec));          \
        }
#else
#define DEFINE_SEED_LOCK_TIMED_LOCK_IMPL(Class)                                             \
    public :                                                                                \
        bool timedLock(uint32 timeOutMilisec) const                                         \
        {                                                                                   \
            std::chrono::milliseconds timeOut(timeOutMilisec);                              \
            auto now = std::chrono::steady_clock::now();                                    \
            return _mutex.try_lock_until(now + std::chrono::milliseconds(timeOutMilisec));  \
        }
#endif

    class cLock : public cAutoLockable {
    private :
        mutable std::mutex _mutex;
    public :
#if defined(_MSC_VER)
        std::mutex& mutex() { return _mutex; }
#else
        pthread_mutex_t& pthread_mutex() { return *_mutex.native_handle(); }
#endif
    public :
        DEFINE_SEED_LOCK_DEFAULT_IMPL(cLock)
    };

    class cRecursiveLock : public cAutoLockable {
    private :
        mutable std::recursive_mutex _mutex;
    public :
        DEFINE_SEED_LOCK_DEFAULT_IMPL(cRecursiveLock)
    };

    class cTimedLock : public cAutoLockable {
    private :
        mutable std::timed_mutex _mutex;

    public :
        DEFINE_SEED_LOCK_DEFAULT_IMPL(cTimedLock)
        DEFINE_SEED_LOCK_TIMED_LOCK_IMPL(cTimedLock)
    };

    class cRecursiveTimedLock : public cAutoLockable {
    private :
        mutable std::recursive_timed_mutex _mutex;

    public :
        DEFINE_SEED_LOCK_DEFAULT_IMPL(cRecursiveTimedLock)
        DEFINE_SEED_LOCK_TIMED_LOCK_IMPL(cRecursiveTimedLock)
    };
}

#endif