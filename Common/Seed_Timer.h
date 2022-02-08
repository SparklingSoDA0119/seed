#ifndef _SEED_TIMER_H
#define _SEED_TIMER_H

#include <Common/Seed_Object.h>

namespace Seed {

    class cTimer;

    typedef void (cObject::*cTimerAction)(cTimer*);
    
    class cTimer : public cObject {
    public :
        cTimer(uint64 interval, bool repeat, cObject* target, cTimerAction action)
            : _interval(interval), _repeat(repeat), _target(target), _action(action) {}

        virtual ~cTimer() {}

    private :
        uint64 _interval;
        bool _repeat;

    public:
        uint64 interval() const { return _interval; }
        void setInterval(uint32 interval) { _interval = interval; }
        bool isRepeat() const { return _repeat; }
        void setRepeat(bool repeat) { _repeat = repeat; }

     private :
        cObject* _target;
        cTimerAction _action;

    public :
        cObject* target() const { return _target; }
        void setTarget(cObject* target) { _target = target; }
        cTimerAction action() const { return _action; }
        void setAction(cTimerAction action) { _action = action; }
    };

}   // namespace Vix

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_TIMER_H

