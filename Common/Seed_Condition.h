#ifndef _SEED_CONDITION_H
#define _SEED_CONDITION_H

#include <Common/Seed_Object.h>
#include <Common/Seed_Lock.h>

#if !defined(_WIN32)
    #include <pthread.h>
#endif

namespace Seed {

    class cCondition : public cObject {
    public :
#if defined (_MSC_VER)
        typedef void* Handle;
#else
        typedef pthread_cond_t Handle;
#endif

    public :
        cCondition();

        virtual ~cCondition();

    private :
        Handle _handle;
        bool _signaled;
        cLock _lock;

    public :
        Handle& handle() { return _handle; }
        bool isSignaled() const { return _signaled; }
        bool wait(uint32 timeoutMilisec = 0);
        void signal();
        void reset();
    };

}   // namespace Seed

#endif
