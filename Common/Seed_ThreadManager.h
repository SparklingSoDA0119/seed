#ifndef _SEED_THREAD_MANAGER_H
#define _SEED_THREAD_MANAGER_H

#include <Common/Seed_Object.h>
#include <Common/Seed_String.h>
#include <Common/Seed_Lock.h>

#include <list>

namespace Seed {
    class cThread;

    class cThreadManager : public cObject {
    public:
        static cThreadManager& defaultManager();
        static uint32 currentThreadId();
        static const cThread* currentThread(const cThreadManager* givenManager = 0);
        static const cString& currentThreadName(const cThreadManager* givenManager = 0);

    public:
        cThreadManager(const cString& name) : _name(name) {}

        virtual ~cThreadManager() {}

    public:

        class Enumerator {
        public:
            Enumerator(std::list<cThread*>::const_iterator begin,
                std::list<cThread*>::const_iterator end)
                : _iterator(begin), _end(end) {}

            Enumerator(const Enumerator& e) : _iterator(e._iterator), _end(e._end) {}

            Enumerator() {}

        private:
            std::list<cThread*>::const_iterator _iterator;
            std::list<cThread*>::const_iterator _end;

        public:
            cThread* next() { return (_iterator != _end) ? *_iterator++ : 0; }
        };

    private:
        cString _name;
        std::list<cThread*> _list;
        mutable cLock _lock;

    public:
        void registerThread(cThread* thread);
        void unregisterThread(cThread* thread);

    public:
        const cString& name() const { return _name; }
        uint32 threadCount() const { return static_cast<uint32>(_list.size()); }
        Enumerator threadEnumerator() const;
        cThread* findThread(uint32 id) const;
        cThread* findThread(const cString& name) const;

    public:
        virtual cString toString() const;
    };

}   // namespace Seed

#endif  // _SEED_THREAD_MANAGER_H
