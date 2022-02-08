#ifndef _SEED_STRING_TOKENIZER_H
#define _SEED_STRING_TOKENIZER_H

#include <Common/Seed_String.h>

namespace Seed {
    class cStringTokenizer : public cObject {
    public:
        cStringTokenizer(const cString& s, const cString& delims = L" ", bool dropEmptyToken = true)
            : _string(s), _delims(delims), _index(0) {}

        virtual ~cStringTokenizer() {}

    private:
        cString _string;
        cString _delims;   // each character is separator.
        uint32 _index;     // current parsing position in _string.

    public:
        bool getToken(cString* token);

    private:    // UNCOPYABLE!!! (DO NOT IMPL.)
        cStringTokenizer(const cStringTokenizer&);
        cStringTokenizer& operator=(const cStringTokenizer&);
    };

}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_STRING_TOKENIZER_H
