#include <Common/Seed_Exception.h>

using namespace Seed;

cString cException::toString() const
{
    cString s;
    s.format(L"\"%s\", %d, 0x%x", &domain(), code(), info());

    return s;
}

cString cRangeException::toString() const
{
    if (info()) {
        const Info* i = reinterpret_cast<const Info*>(info());
        cString s;
        s.format(L"\"%s\", %d in [%d, %d]", &domain(), i->position, i->begin, i->end);

        return s;
    }
    else {
        return cException::toString();
    }
}

cString cBadAllocException::toString() const
{
    if (info()) {
        uint32 allocSize = *reinterpret_cast<const uint32*>(info());
        cString s;
        s.format(L"%s\", allocSize=%u", &domain(), allocSize);

        return s;
    }
    else {
        return cException::toString();
    }
}

