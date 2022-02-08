#ifndef _SEED_LOOPER_H
#define _SEED_LOOPER_H

#include <Common/Seed_Thread.h>
#include <Common/Seed_Condition.h>

namespace Seed {

    class cLooper : public cThread {
    public:
        cLooper(const cString& name, cThreadManager* threadManager = 0);

        virtual ~cLooper() {}

    protected:
        uint32 _autoResumeInterval;

    public:
        uint32 autoResumeInterval() const { return _autoResumeInterval; }
        void setAutoResumeInterval(uint32 interval) { _autoResumeInterval = interval; }

    private:
        bool _quitRequested;
        cCondition _condition;

    protected:
        virtual void threadFunc();
        virtual void looperFunc() = 0;
        virtual void quitRequested();
        virtual void terminated() { _quitRequested = false; }

    protected:
        virtual void suspending() {}
        virtual void suspended() {}
        virtual void resumed() {}

    public:
        bool isQuitRequested() const { return _quitRequested; }
        void suspend();
        void resume();
    };

}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_LOOPER_H
