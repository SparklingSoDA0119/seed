#include <Common/Seed_Looper.h>

using namespace Seed;

cLooper::cLooper(const cString& name, cThreadManager* threadManager)
    : cThread(name, threadManager), _autoResumeInterval(0),
    _quitRequested(false)
{

}

void cLooper::threadFunc()
{
    while (!isQuitRequested()) {
        if (status() == STATUS_SUSPENDING) {
            setStatus(STATUS_SUSPENDED);
            suspended();
            //cTRACE(L"VixLooper[%s] is suspended.", this->name());

            _condition.wait(_autoResumeInterval);
            _condition.reset();
            if (!isQuitRequested()) {
                setStatus(STATUS_WORKING);
                resumed();
                //VIXTRACE(L"VixLooper[%s] is resumed.", this->name());
            }
        }
        else {
            looperFunc();
        }
    }
}

void cLooper::quitRequested()
{
    _quitRequested = true;
    resume();   // wake up
}

void cLooper::suspend()
{
    while (status() == STATUS_STARTING) {
        //SEED_TRACE(L"cLooper[%s]::suspend() is waiting for starting.", this->name());
        cThread::yield();
    }

    if (status() == STATUS_WORKING) {
        _condition.reset();
        setStatus(STATUS_SUSPENDING);
        suspending();
        //SEED_TRACE(L"cLooper[%s] is suspending.", this->name());
    }
    else {
        //SEED_TRACE(L"cLooper[%s]::suspend() is ignored. because it's not working (status = %d).", this->name(), status());
    }
}

void cLooper::resume()
{
    if (status() != STATUS_WORKING) {
        _condition.signal();
    }
    else {
        //SEED_TRACE(L"cLooper[%s]::resume() is ignored. because it's working.(status = %d).", this->name(), status());
    }
}