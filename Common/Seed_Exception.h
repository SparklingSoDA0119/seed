#ifndef _SEED_EXCEPTION_H
#define _SEED_EXCEPTION_H

#include <Common/Seed_Object.h>
#include <Common/Seed_String.h>

namespace Seed {
    class cException : public cObject {
    public :
        cException() : _code(0), _info(0) {}

        cException(const cString& domain, int32 code = 0, const void* info = 0)
            : _domain(domain), _code(code), _info(info) {}

        virtual ~cException() {}

    private :
        cString _domain;
        int32 _code;
        const void* _info;

    public :
        const cString& domain() const { return _domain; }
        int32 code() const { return _code; }
        const void* info() const { return _info; }
        void setDomain(const cString& domain) { _domain = domain; }
        void setCode(int32 code) { _code = code; }
        void setInfo(const void* info) { _info = info; }
    
    public :
        virtual cString toString() const;
    };

    class cRangeException : public cException {
    public :
        struct Info {
            int32 begin;
            int32 end;
            int32 position;

            Info() : begin(0), end(0), position(0) {}
            Info(int32 b, int32 e, int32 p) : begin(b), end(e), position(p) {}
        };
    
    public :
        cRangeException(const cString& domain, const Info& info = Info())
            : cException(domain, 0, &_rangeInfo), _rangeInfo(info) {}
        
        virtual ~cRangeException() {}

    protected :
        Info _rangeInfo;

    public :
        virtual cString toString() const;
    };

    class cBadAllocException : public cException {
    public :
        cBadAllocException(const cString& domain, uint32 allocSize = 0)
            : cException(domain, 0, &_allocSize), _allocSize(allocSize) {}
        
        virtual ~cBadAllocException() {}

    protected :
        uint32 _allocSize;

    public :
        virtual cString toString() const;
    };

    class cBadCastException : public cException {
    public :
        cBadCastException(const cString& domain, int32 code = 0, void* info = 0)
            : cException(domain, code, info) {}

        virtual ~cBadCastException() {}
    };
}   // namespace Seed

#endif  // _SEED_EXCEPTION_H
