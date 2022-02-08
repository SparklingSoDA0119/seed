#include <Common/Seed_Object.h>
#include <Common/Seed_String.h>
#include <Common/Seed_Exception.h>

#include <stdexcept>
#include <exception>    // bad_exception
#include <typeinfo>     // bad_cast, bad_typeid
#include <new>          // bad_alloc
#include <cstdlib>      // abort()

#if defined(_MSC_VER)
    #include <windows.h>
#endif

using namespace std;
using namespace Seed;

void cObject::exceptionCaught(cException& e)
{
    //VIXTRACE(L"cObject::exceptionCaught(%@)", &e);
    abort();
}

void cObject::objectExceptionCaught(cObject& e)
{
    //VIXTRACE(L"cObject::objectExceptionCaught(%@)", &e);
    abort();
}

void cObject::stdExceptionCaught(std::exception& e)
{
    cString s(e.what());
    //VIXTRACE(L"cObject::stdExceptionCaught(%@)", &s);
    abort();
}

void cObject::unknownExceptionCaught()
{
    //VIXTRACE(L"cObject::unknownExceptionCaught()");
    abort();
}

#if defined(_MSC_VER) && defined(_VIX_USE_DUMP)
namespace {
    void _vix_win32_performWithoutException(cObject* object,
                                           void (cObject::*func)())
    {
        (object->*func)();

        __try {
            (object->*func)();
        }
        __except(VIXDUMP(GetExceptionInformation(), L"cThread"),
                 EXCEPTION_CONTINUE_SEARCH) {
        }
    }
}
#endif

#undef performWithoutException
bool cObject::performWithoutException(void (cObject::*func)())
{
    try {
    #if defined(_MSC_VER) && defined(_VIX_USE_DUMP)
        _vix_win32_performWithoutException(this, func);
    #else
        (this->*func)();
    #endif
        return true;
    }
    #define CATCH_STD_EXCEPTION(name)   \
        catch (std::name& e) { this->stdExceptionCaught(e); }
    // c-based
    catch (cException& e) { this->exceptionCaught(e); }
    catch (cObject& e) { this->objectExceptionCaught(e); }
    // std::logic_errors
    CATCH_STD_EXCEPTION(domain_error)
    CATCH_STD_EXCEPTION(invalid_argument)
    CATCH_STD_EXCEPTION(length_error)
    CATCH_STD_EXCEPTION(out_of_range)
    CATCH_STD_EXCEPTION(logic_error)
    // std::runtime_errors
    CATCH_STD_EXCEPTION(range_error)
    CATCH_STD_EXCEPTION(overflow_error)
    CATCH_STD_EXCEPTION(runtime_error)
    // std::others
    CATCH_STD_EXCEPTION(bad_alloc)
    CATCH_STD_EXCEPTION(bad_cast)
    CATCH_STD_EXCEPTION(bad_typeid)
    CATCH_STD_EXCEPTION(bad_exception)
    CATCH_STD_EXCEPTION(exception)
    // etc.
    catch (...) { this->unknownExceptionCaught(); }
    return false;
}

cString cObject::toString() const { return cString(L""); }

std::wostream& operator<<(std::wostream& stream, const Seed::cObject& object)
{
    return stream << object.toString().to_wstring();
}

std::ostream& operator<<(std::ostream& stream, const Seed::cObject& object)
{
    return stream << object.toString().to_string();
}