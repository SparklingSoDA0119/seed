#ifndef _SEED_SINGLETON_H_
#define _SEED_SINGLETON_H_

#include <Common/Seed_Object.h>
#include <Common/Seed_Lock.h>
#include <Common/Seed_Assert.h>
#include <memory>

namespace Seed {
    template <typename T>
    class cSingleton {
    public:
        cSingleton()
        {
            SEED_ASSERT(_allocatingInstance, L"Cannot make an instance of Singlton");
        }
        static T* get();

    private:
        static std::unique_ptr<T> _instance;
        static cLock _lock;
        static bool _allocatingInstance;
    };

    template <typename T>
    T* cSingleton<T>::get()
    {
        if (!_instance.get()) {
            cAutoLock al(_lock);
            if (!_instance.get()) {
                _allocatingInstance = true;
                _instance.reset(new T);
                _allocatingInstance = false;
            }
        }

        return _instance.get();
    }

    template <typename T>
    std::unique_ptr<T> cSingleton<T>::_instance;

    template <typename T>
    cLock cSingleton<T>::_lock;

    template <typename T>
    bool cSingleton<T>::_allocatingInstance;
}

#endif