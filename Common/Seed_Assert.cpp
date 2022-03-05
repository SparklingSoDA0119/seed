#include <Common/Seed_Assert.h>

#if defined(_SEED_USE_ASSERT)

#include <Common/Seed_StringUtil.h>  // lastPathComponent()

#include <iostream> // wcout
#include <cstdlib>  // abort()

using namespace std;

namespace Seed {

    namespace {
        void _defaultAssertFunc(const cString& exp, const cString& msg,
            const cString& func, const cString& file, uint32 line)
        {
            cString s;
            cString filename = StringUtil::lastPathComponent(file);
            s.format(L"SEED_ASSERT(%s, %s) - %s (%s,%d)", &exp, &msg, &func, &filename, line);

            std::cout << s << std::endl;

            abort();
        }

        SeedAssertFunc _assertFunc = &_defaultAssertFunc;
    }

    const SeedAssertFunc& seed_assert_func = _assertFunc;

    void setSeedAssertFunc(SeedAssertFunc func)
    {
        _assertFunc = (func) ? func : &_defaultAssertFunc;
    }

}   // namespace Seed
#endif  // _SEED_USE_ASSERT