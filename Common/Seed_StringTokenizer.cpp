#include <Common/Seed_StringTokenizer.h>

using namespace Seed;

bool cStringTokenizer::getToken(cString* token)
{
    const std::wstring& str = _string.to_wstring();
    const std::wstring& delims = _delims.to_wstring();

    std::size_t i = str.find_first_not_of(delims, _index);
    if (i == std::wstring::npos) {
        _index = static_cast<uint32>(str.length());
        return false;
    }

    std::size_t j = str.find_first_of(delims, i);
    if (j == std::wstring::npos) {
        *token = str.substr(i).c_str();
        _index = static_cast<uint32>(str.length());
    }
    else {
        *token = str.substr(i, j - i).c_str();
        _index = static_cast<uint32>(j);
    }

    return true;
}