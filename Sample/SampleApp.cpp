#include <Sample/SampleApp.h>
#include <Common/Seed_SystemUtil.h>

using namespace Seed;

cSampleApp::cSampleApp()
    : cLooper(L"SampleApp", nullptr), _isRun(false)
{

}

cSampleApp::~cSampleApp()
{
    _queue.join();
    _queue.quit();
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

    _queue.initialization();

    return SEED_SUCCESS;
}

void cSampleApp::looperFunc()
{
    if (_isRun) {
        //printf("<%s:%s(%d)> ** Running. currentTick : %llu\n", __FILE__, __FUNCTION__, __LINE__, TickUtil::currentTick());
        SAMPLE_DATA_S stData;
        stData._data = 123;
        stData._dataStr = L"Test";
        _queue.pushData(stData);
    }

    cThread::sleep(30);
}