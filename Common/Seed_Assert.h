#ifndef _SEED_ASSERT_H
#define _SEED_ASSERT_H

#if defined(_MSC_VER)
# pragma once
#endif

#include <Common/Seed_String.h>

#if !defined(_SEED_DONT_USE_ASSERT)
#define _SEED_USE_ASSERT
#endif

#include <cassert>

namespace Seed {

#if defined(_SEED_USE_ASSERT)
    typedef void (*SeedAssertFunc)(const cString& exp, const cString& msg,
                                   const cString& func, const cString& file, uint32 line);


    void setSeedAssertFunc(SeedAssertFunc func);

    extern const SeedAssertFunc& seed_assert_func;
    
#define SEED_ASSERT(exp, msg)   \
    (void)((exp) || (Seed::seed_assert_func(cString(#exp), msg, cString(__FUNCTION__), cString(__FILE__), __LINE__), 0))
#else
#define setSeedAssertFunc(func)   // do nothing
#define SEED_ASSERT(exp, msg)     // do nothing
#endif

}   // namespace Seed

#endif  // _SEED_ASSERT_H
