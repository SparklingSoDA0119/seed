#ifndef _SEED_THREAD_H
#define _SEED_THREAD_H

#if defined(_MSC_VER)
# pragma once
#endif

#include <Common/Seed_ThreadManager.h>
#include <Common/Seed_Timer.h>
#include <Common/Seed_Lock.h>
#include <Common/Seed_Condition.h>

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#endif

#include <unordered_map>
#include <list>
#include <memory>   // auto_ptr

namespace Seed {
    class cThread : public cObject {
    public:
#if defined(_MSC_VER)
        typedef void* Handle;
#else
        typedef pthread_t Handle;
#endif

    public:
        cThread(const cString& name, cThreadManager* threadManager = 0);

        virtual ~cThread();

    public:
        static const uint32 INVALID_ID = 0;

        static const uint32 PRIORITY_LOWEST = 1;
        static const uint32 PRIORITY_NORMAL = 50;
        static const uint32 PRIORITY_HIGHEST = 99;

        enum Policy {
            POLICY_NORMAL = 0,
            POLICY_REALTIME = 1,
            POLICY_REALTIME_RR = 2,
        };

    private:
        static uint32 DEFAULT_THREAD_AFFINITY_MASK_VALUE;

    public:
        static const uint32& DEFAULT_THREAD_AFFINITY_MASK;
        static void setDefaultThreadAffinityMask(uint32 mask)
        {
            DEFAULT_THREAD_AFFINITY_MASK_VALUE = mask;
        }

    private:
        uint32 _id;
        Handle _handle;
        cString _name;
        uint32 _priority;
        Policy _policy;
        uint32 _threadAffinityMask;
        cThreadManager* _manager;

    public:
        uint32 id() const { return _id; }
        Handle& handle() { return _handle; }
        const cString& name() const { return _name; }
        uint32 priority() const { return _priority; }
        Policy policy() const { return _policy; }
        cThreadManager* manager() const { return _manager; }
        void setName(const cString& name) { _name = name; }
        void setPriority(uint32 priority, Policy policy);
        bool setAffinityMask(uint32 mask);
        uint32 affinityMask() const { return _threadAffinityMask; }

    public:
        enum Status {
            STATUS_READY = 0,
            STATUS_STARTING = 1,
            STATUS_WORKING = 2,
            STATUS_SUSPENDING = 3,
            STATUS_SUSPENDED = 4,
            STATUS_FINISHING = 5,
        };

    private:
        Status _status;
        cLock _statusLock;

    private:
        void readyThread();
        void initThread();
        void runThread();
        void cleanupThread();
        void _coreThreadFunc();

    protected:
        void setStatus(Status status);
        bool startingThread();

#if defined(_MSC_VER)
        static unsigned int WINAPI coreThreadFunc(void* param);
#else
        static void* coreThreadFunc(void* param);
#endif

    protected:
        virtual void starting() {}
        virtual void started() {}
        virtual void threadFunc() = 0;
        virtual void quitRequested() {}
        virtual void finished() {}
        virtual void terminated() {}

    public:
        Status status() const { return _status; }
        bool isAlive() const { return _status == STATUS_STARTING || _id != INVALID_ID; }
        bool run(bool waitUntilStarted = false);
        void quit();
        void join();

    public:
        static void yield();
        static void sleep(uint32 milisecond);

    protected:
        typedef struct timer_s {
            std::shared_ptr<cTimer> timer;
            uint64 lastCheckTick;
            timer_s() : lastCheckTick(0) {}
        } TIMER_S;

        std::list<TIMER_S> _timers;
        cLock _timersLock;

    protected:
        void checkTimers();

    public:
        cTimer* addTimer(uint32 interval, bool repeat, cObject* target, cTimerAction action);
        void restartTimer(cTimer* timer);
        void removeTimer(cTimer* timer);
        void removeAllTimers();

    public:
        virtual cString toString() const;
    };

}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_THREAD_H
