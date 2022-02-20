#include <Sample/SampleApp.h>
#include <Common/Seed_SystemUtil.h>
#if defined(_MSC_VER)
#include <System/Seed_Windows10.h>
#elif defined(__linux__)
#include <System/Seed_Ubuntu2004.h>
#else
#endif

using namespace Seed;

cSampleApp::cSampleApp()
    : cLooper(L"SampleApp", nullptr), _isRun(false)
{
    
}

cSampleApp::~cSampleApp()
{
}

int32 cSampleApp::initailization()
{
    if (!isAlive()) {
        run();
    }
    else {
        resume();
    }

    _isRun = true;

    return SEED_SUCCESS;
}

void cSampleApp::looperFunc()
{
    if (_isRun) {
        printf("<%s:%s(%d)> ** Running. currentTick : %llu\n", __FILE__, __FUNCTION__, __LINE__, TickUtil::currentTick());
        
    }

    cThread::sleep(30);
}