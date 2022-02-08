#include <Common/Seed_ThreadManager.h>
#include <Common/Seed_Thread.h>
#include <Common/Seed_StringUtil.h>

#include <algorithm>    // find_if

using namespace Seed;

cThreadManager& cThreadManager::defaultManager()
{
    static cThreadManager s_defaultManager(L"DefaultManager");
    return s_defaultManager;
}

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <syscall.h>
namespace {
    inline pid_t gettid() { return (pid_t)syscall(__NR_gettid); }
}
#endif

uint32 cThreadManager::currentThreadId()
{
    uint32 id = 0xffffffff;

#if defined(_MSC_VER)
    id = static_cast<uint32>(::GetCurrentThreadId());
#else
    id = static_cast<uint32>(::gettid());
#endif

    return id;
}

const cThread* cThreadManager::currentThread(const cThreadManager* givenManager)
{
    uint32 id = currentThreadId();

    cThread* thread = 0;
    if (givenManager) {
        thread = givenManager->findThread(id);
    }
    else {
        thread = defaultManager().findThread(id);
    }

    return (const cThread*)thread;
}

const cString& cThreadManager::currentThreadName(const cThreadManager* givenManager)
{
    static cString s_unknownThread(L"UnknownThread");

    const cThread* thread = cThreadManager::currentThread(givenManager);
    if (thread) {
        return thread->name();
    }

    return s_unknownThread;
}

void cThreadManager::registerThread(cThread* thread)
{
    cAutoLock al(_lock);
    _list.push_back(thread);
}

void cThreadManager::unregisterThread(cThread* thread)
{
    cAutoLock al(_lock);
    _list.remove(thread);
}

cThreadManager::Enumerator cThreadManager::threadEnumerator() const
{
    return Enumerator(_list.begin(), _list.end());
}

namespace {

    typedef struct id_finder_s {
        uint32 _id;
        id_finder_s(uint32 id) : _id(id) {}
        bool operator()(cThread* thread) { return (thread->id() == _id); }
    } ID_FINDER_S;

    typedef struct name_finder_s {
        cString _name;
        name_finder_s(const cString& name) : _name(name) {}
        bool operator()(cThread* thread) { return (thread->name() == _name); }
    } NAME_FINDER_S;
}   // unnamed namespace

cThread* cThreadManager::findThread(uint32 id) const
{
    cAutoLock al(_lock);
    std::list<cThread*>::const_iterator i;
    i = std::find_if(_list.begin(), _list.end(), ID_FINDER_S(id));
    return ((i != _list.end()) ? *i : (cThread*)0);
}

cThread* cThreadManager::findThread(const cString& name) const
{
    cAutoLock al(_lock);
    std::list<cThread*>::const_iterator i;
    i = std::find_if(_list.begin(), _list.end(), NAME_FINDER_S(name));
    return ((i != _list.end()) ? *i : (cThread*)0);
}

cString cThreadManager::toString() const
{
    cString s;
    s.format(L"\"%s\"={ ", &name());
    std::list<cThread*>::const_iterator i = _list.begin();
    while (i != _list.end()) {
        s.appendf(L"[%x]", *i);
        if (++i != _list.end()) {
            s.appendf(L", ");
        }
    }
    s.appendf(L" }");
    return s;
}

