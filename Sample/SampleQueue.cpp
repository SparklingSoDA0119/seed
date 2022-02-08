#include <Sample/SampleQueue.h>

using namespace Seed;

cSampleQueue::cSampleQueue()
    : cLooper(L"SampleLooper", nullptr)
    , _isRun(false)
{
    _dataQue.clear();
}

cSampleQueue::~cSampleQueue()
{
    _dataQue.clear();
}

int32 cSampleQueue::initialization()
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

int32 cSampleQueue::pushData(SAMPLE_DATA_S stData)
{
    cAutoLock al(_lock);
    _dataQue.push_back(stData);

    return SEED_SUCCESS;
}

void cSampleQueue::looperFunc()
{
    if (_isRun) {
        checkQueue();
    }

    cThread::sleep(100);
}

int32 cSampleQueue::checkQueue()
{
    if (!_dataQue.empty()) {
        SAMPLE_DATA_S stData = _dataQue.front();

        {
            cAutoLock al(_lock);
            _dataQue.pop_front();
            printf("<%s:%s(%d)> ** Pop front. remain : %d\n", __FILE__, __FUNCTION__, __LINE__, _dataQue.size());
        }

        if (_dataQue.size() > 100) {
            cAutoLock al(_lock);
            _dataQue.clear();
        }
    }

    return SEED_SUCCESS;
}
