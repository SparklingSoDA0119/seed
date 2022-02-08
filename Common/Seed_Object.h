#ifndef _SEED_OBJECT_H_
#define _SEED_OBJECT_H_

#include <Common/Seed_Type.h>

namespace Seed {

    class cException;
    class cString;

    class cObject {
    public:
        cObject() {}
        virtual ~cObject() = 0;
    

    protected :
        virtual void exceptionCaught(cException& e);
        virtual void objectExceptionCaught(cObject& e);
        virtual void stdExceptionCaught(std::exception& e);
        virtual void unknownExceptionCaught();

    public :
        virtual bool performWithoutException(void (cObject::* func)());

        #define performWithoutException(func) \
            performWithoutException(static_cast<void (cObject::*)()>(func))

    public :
        virtual cString toString() const;
    };

    inline cObject::~cObject() {}
}

std::wostream& operator<<(std::wostream& stream, const Seed::cObject& object);
std::ostream& operator<<(std::ostream& stream, const Seed::cObject& object);

#endif