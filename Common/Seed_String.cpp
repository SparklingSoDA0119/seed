#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include <Common/Seed_String.h>
#include <Common/Seed_Exception.h>

#include <string>
#include <algorithm>
#include <locale>
#include <vector>
#include <stdexcept>    // runtime_error
#include <iostream>     // wcerr
#include <cstring>

using namespace std;

namespace Seed {

#if !defined(_MSC_VER)
#define _snwprintf  swprintf
#endif

    void _seed_wstring_vformat(std::wstring* result, const std::wstring& format, va_list args)
    {
        static const std::wstring types(L"@diouxXDOUeEfFgGaACcSspn");

        result->clear();

        std::wstring tf;
        const uint32 TS_N = 256;
        wchar_t ts[TS_N];
        std::unique_ptr<wchar_t> tss;
        std::wstring::const_iterator si = format.begin();
        std::wstring::const_iterator t, i = si;
        while (true) {
            i = std::find(i, format.end(), L'%');
            if (i == format.end()) {
                result->append(si, format.end());
                break;
            }

            if (*(i + 1) == L'%') {
                result->append(si, i + 1);
                i += 2;
                si = i;
                continue;
            }

            t = std::find_first_of(i, format.end(), types.begin(), types.end());
            if (t == format.end()) {
                std::wcerr << __FUNCTION__ << L"(): Unknown type "
                    << "\"%" << *(i + 1) << "\"" << std::endl;
                abort();
                return;
            }

            result->append(si, i);

            if (*t == L'@') {
                cObject* pObject = va_arg(args, cObject*);
                if (pObject) {
                    result->append(pObject->toString().to_wstring());
                }
                else {
                    result->append(L"(null)");
                }
            }
            else {
                tf.assign(i, t + 1);
                int32 dist = static_cast<int32>(std::distance(i, t));
                if (*t == L'd' || *t == L'i' || *t == L'o' ||
                    *t == L'u' || *t == L'x' || *t == L'X') {
                    if (4 <= dist && *(t - 3) == 'I' && *(t - 2) == '6' && *(t - 1) == '4') {
#if !defined(_MSC_VER)
                        tf.replace(tf.length() - 4, 3, L"ll");
#endif
                        _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int64));
                    }
                    else if (3 <= dist && *(t - 2) == L'l' && *(t - 1) == L'l') {
#if defined(_MSC_VER)
                        tf.replace(tf.length() - 3, 2, L"I64");
#endif
                        _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int64));
                    }
                    else if (2 <= dist && *(t - 1) == L'l') {
                        _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, long));
                    }
                    else {
                        _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
                    }
                }
                else if (*t == L'e' || *t == L'E' || *t == L'f' || *t == L'F' ||
                         *t == L'g' || *t == L'G' || *t == L'a' || *t == L'A') {
                    _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, double));
                }
                else if (*t == L'c') {
#if defined(_MSC_VER)
                    tf[tf.length() - 1] = L'C';
#else
                    tf[tf.length() - 1] = L'c';
#endif
                    _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
                }
                else if (*t == L'C') {
#if defined(_MSC_VER)
                    tf[tf.length() - 1] = L'c';
#else
                    tf[tf.length() - 1] = L'C';
#endif
                    _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, wint_t));
                }
                else if (*t == L's') {
#if defined(_MSC_VER)
                    tf[tf.length() - 1] = L'S';
#else
                    tf[tf.length() - 1] = L's';
#endif
                    char* s = va_arg(args, char*);
                    size_t tssn = std::strlen(s) + 1;
                    if (TS_N < tssn) {
                        tss.reset(new wchar_t[tssn]);
                        _snwprintf(tss.get(), tssn, tf.c_str(), s);
                    }
                    else {
                        _snwprintf(ts, TS_N, tf.c_str(), s);
                    }
                }
                else if (*t == L'S') {
#if defined(_MSC_VER)
                    tf[tf.length() - 1] = L's';
#else
                    tf[tf.length() - 1] = L'S';
#endif
                    wchar_t* ws = va_arg(args, wchar_t*);
                    size_t tssn = std::wcslen(ws) + 1;
                    if (TS_N < tssn) {
                        tss.reset(new wchar_t[tssn]);
                        _snwprintf(tss.get(), tssn, tf.c_str(), ws);
                    }
                    else {
                        _snwprintf(ts, TS_N, tf.c_str(), ws);
                    }
                }
                else if (*t == L'p') {
                    _snwprintf(ts, TS_N, tf.c_str(), va_arg(args, void*));
                }

                if (tss.get()) {
                    result->append(tss.get());
                    tss.reset();
                }
                else {
                    ts[TS_N - 1] = L'\0';
                    result->append(ts);
                }
            }
            i = t + 1;
            si = i;
        }
    }
}   // namespace Seed

using namespace Seed;

void cString::format(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _seed_wstring_vformat(&_wstring, format, args);
    va_end(args);
}

void cString::format(const cString* format, ...)
{
    va_list args;
    va_start(args, format);
    _seed_wstring_vformat(&_wstring, format->_wstring, args);
    va_end(args);
}

void cString::vformat(const wchar_t* format, va_list args)
{
    _seed_wstring_vformat(&_wstring, format, args);
}

void cString::vformat(const cString* format, va_list args)
{
    std::string s;
    _seed_wstring_vformat(&_wstring, format->_wstring, args);
    from_string(s);
}

void cString::appendf(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring ws;
    _seed_wstring_vformat(&ws, format, args);
    va_end(args);

    _wstring += ws;
}

void cString::appendf(const cString* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring ws;
    _seed_wstring_vformat(&ws, format->_wstring, args);
    va_end(args);

    _wstring += ws;
}

void cString::appendvf(const wchar_t* format, va_list args)
{
    std::wstring ws;
    _seed_wstring_vformat(&ws, format, args);

    _wstring += ws;
}

void cString::appendvf(const cString* format, va_list args)
{
    std::wstring ws;
    _seed_wstring_vformat(&ws, format->_wstring, args);

    _wstring += ws;
}

void cString::makeUpper(const std::locale& loc)
{
    std::wstring r;
    for (int i = 0; i < static_cast<int32>(_wstring.size()); i++) {
        r.push_back(std::toupper<wchar_t>(_wstring[i], loc));
    }
    _wstring.swap(r);
}

void cString::makeLower(const std::locale& loc)
{
    std::wstring r;
    for (int i = 0; static_cast<int32>(_wstring.size()); i++) {
        r.push_back(std::tolower<wchar_t>(_wstring[i], loc));
    }
    _wstring.swap(r);
}

void cString::trimLeft()
{
    for (std::wstring::iterator i = _wstring.begin(); ; ++i) {
        if (i == _wstring.end()) {
            _wstring.clear();
            break;
        }
        else if (!std::isspace(*i, defaultLocale())) {
            _wstring.erase(_wstring.begin(), i);
            break;
        }
    }
}

void cString::trimRight()
{
    for (std::wstring::iterator i = _wstring.end() - 1; ; --i) {
        if (!std::isspace(*i, defaultLocale())) {
            _wstring.erase(i + 1, _wstring.end());
            break;
        }
        else if (i == _wstring.begin()) {
            _wstring.clear();
            break;
        }
    }
}

void cString::adjustToBytes(uint32 bytes)
{
    if (0 == bytes) {
        _wstring = L"";
    }
    else {
        std::string s = to_string();
        if (bytes < s.length()) {
            while (bytes < s.length()) {
                _wstring.resize(_wstring.length() - 1);
                s = to_string();
            }
            from_string(s);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

#define THROW_RANGE_EXCEPTION(domain, index)                                            \
    cRangeException::Info info(0, static_cast<int32>(_wstring.length()) - 1, index);   \
    throw cRangeException(domain, info)

#if !defined(_MSC_VER)
const uint32 cString::NPOS;
#endif

const wchar_t& cString::at(uint32 index) const
{
    if (_wstring.length() <= index) {
        THROW_RANGE_EXCEPTION(L"cString::at()", index);
    }
    return _wstring[index];
}

////////////////////////////////////////////////////////////////////////////////

int32 cString::compare(uint32 index, uint32 length, const cString& s) const
{
    if (_wstring.length() <= index) {
        THROW_RANGE_EXCEPTION(L"cString::compare()", index);
    }
    return _wstring.compare(index, length, s._wstring);
}

bool cString::caseInsensitiveCompare(const cString& s) const
{
    struct lt_str_2 : public std::binary_function<std::wstring, std::wstring, bool> {
        bool operator()(const std::wstring& x, const std::wstring& y, const std::locale& loc) const
        {
            
            return std::lexicographical_compare(x.begin(),
                x.end(),
                y.begin(),
                y.end());
                //boost::algorithm::is_iequal(loc));
        }
    };
    lt_str_2 comp;
    return comp(_wstring, s._wstring, defaultLocale());
}

////////////////////////////////////////////////////////////////////////////////

uint32 cString::find(const cString& s, uint32 index) const
{
    std::wstring::size_type i = _wstring.find(s._wstring, index);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::findFirstOf(const cString& s, uint32 index) const
{
    std::wstring::size_type i = _wstring.find_first_of(s._wstring, index);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::findFirstNotOf(const cString& s, uint32 index) const
{
    std::wstring::size_type i = _wstring.find_first_not_of(s._wstring, index);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::findLastOf(const cString& s, uint32 index) const
{
    std::wstring::size_type i = _wstring.find_last_of(s._wstring, index);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::findLastNotOf(const cString& s, uint32 index) const
{
    std::wstring::size_type i = _wstring.find_last_not_of(s._wstring, index);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::caseInsensitiveFind(const cString& s) const
{
    std::wstring oriUpper;
    for (int32 i = 0; static_cast<int32>(_wstring.size()); i++) {
        oriUpper.push_back(std::toupper<wchar_t>(_wstring[i], defaultLocale()));
    }
    std::wstring sUpper;
    for (int32 i = 0; i < static_cast<int32>(s.length()); i++) {
        sUpper.push_back(std::toupper<wchar_t>(s[i], defaultLocale()));
    }
    std::wstring::size_type i = oriUpper.find_first_of(sUpper, 0);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::reverseFind(const cString& s, uint32 index) const
{
    std::wstring::size_type pos = (index == cString::NPOS) ?
        std::wstring::npos : static_cast<std::wstring::size_type>(index);
    std::wstring::size_type i = _wstring.rfind(s._wstring, pos);
    return (i == std::wstring::npos) ? cString::NPOS : static_cast<uint32>(i);
}

uint32 cString::count(wchar_t ch) const
{
    return static_cast<uint32>(std::count(_wstring.begin(), _wstring.end(), ch));
}

uint32 cString::count(const cString& s) const
{
    uint32 occurrences = 0;

    if (!isEmpty() && !s.isEmpty()) {
        const uint32 len = s.length();

        uint32 index = 0;
        while ((index = find(s, index)) != cString::NPOS) {
            ++occurrences;
            index += len;
        }
    }
    return occurrences;
}

cString& cString::replace(uint32 index, uint32 length, const cString& s)
{
    if (_wstring.length() <= index) {
        THROW_RANGE_EXCEPTION(L"cString::replace()", index);
    }
    _wstring.replace(index, length, s._wstring);
    return *this;
}

cString& cString::replace(const cString& src, const cString& dst, bool all)
{
    uint32 srcLength = src.length();
    uint32 dstLength = dst.length();
    uint32 i = 0;
    do {
        i = find(src, i);
        if (i == NPOS) {
            break;
        }
        
        _wstring.replace(i, srcLength, dst._wstring);
        i += dstLength;
    } while (all);
    return *this;
}

cString& cString::insert(uint32 index, const cString& s)
{
    if (_wstring.length() <= index) {
        THROW_RANGE_EXCEPTION(L"cString::insert()", index);
    }
    _wstring.insert(index, s._wstring);
    return *this;
}

cString cString::substring(uint32 index, uint32 length) const
{
    if (_wstring.length() <= index) {
        THROW_RANGE_EXCEPTION(L"cString::substring()", index);
    }
    else if (length != NPOS && _wstring.length() < index + length) {
        THROW_RANGE_EXCEPTION(L"cString::substring()", index + length);
    }
    return _wstring.substr(index, length).c_str();
}

void cString::from_string(const std::string& cs, const std::locale& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

    const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = std::mbstate_t();
    std::vector<wchar_t> buffer(cs.length() + 1);
    const char* in = cs.c_str();
    wchar_t* out = &buffer[0];
    if (std::codecvt_base::error ==
        codecvt.in(state,
            cs.c_str(), cs.c_str() + cs.length(), in,
            &buffer[0], &buffer[0] + buffer.size(), out)) {
        buffer[0] = L'\0';
    }
    _wstring = &buffer[0];
}

void cString::from_string(const std::string& cs, const std::string& charset)
{
    /* not supported yet */
}

std::string cString::to_string(const std::locale& loc) const
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;

    const std::wstring& ws = _wstring;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = std::mbstate_t();
    std::vector<char> buffer((ws.length() + 1) * codecvt.max_length());
    const wchar_t* in = ws.c_str();
    char* out = &buffer[0];
    if (std::codecvt_base::error ==
        codecvt.out(state,
            ws.c_str(), ws.c_str() + ws.length(), in,
            &buffer[0], &buffer[0] + buffer.size(), out)) {
        buffer[0] = '\0';
    }
    return std::string(&buffer[0]);
}

std::string cString::to_string(const std::string& charset) const
{
    /* not supported yet */

    return "";
}

int8 cString::toInt8() const
{
    int value = 0;
    try {
        value = std::stoi(_wstring);
        if (value > 127 || value < -128) {
            throw cBadCastException(L"cString::toInt8()");
        }
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toInt8()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toInt8()");
    }
    return value;
}

uint8 cString::toUint8() const
{
    unsigned long value = 0;
    try {
        value = std::stoul(_wstring);
        if (value > 255) {
            throw cBadCastException(L"cString::toUInt8()");
        }
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toUInt8()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toUInt8()");
    }
    return static_cast<uint8>(value);
}

int16 cString::toInt16() const
{
    int value = 0;
    try {
        value = std::stoi(_wstring);
        if (value > 32767 || value < -32768) {
            throw cBadCastException(L"cString::toInt16()");
        }
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toInt16()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toInt16()");
    }
    return value;
}

uint16 cString::toUint16() const
{
    unsigned long value = 0;
    try {
        value = std::stoul(_wstring);
        if (value > 65535) {
            throw cBadCastException(L"cString::toUInt16()");
        }
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toUInt16()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toInt16()");
    }
    return static_cast<uint16>(value);
}

int32 cString::toInt32() const
{
    int value = 0;
    try {
        value = std::stoi(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toInt32()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toInt32()");
    }
    return value;
}

uint32 cString::toUint32() const
{
    unsigned long value = 0;
    try {
        value = std::stoul(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toUInt32()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toUInt32()");
    }
    return value;
}

int64 cString::toInt64() const
{
    int64 value = 0;
    try {
        value = std::stoll(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toInt64()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toInt64()");
    }
    return value;
}

uint64 cString::toUint64() const
{
    uint64 value = 0;
    try {
        value = std::stoull(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toUInt64()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toUInt64()");
    }
    return value;
}

float32 cString::toFloat32() const
{
    float32 value = 0;
    try {
        value = std::stof(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toFloat32()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toFloat32()");
    }
    return value;
}

float64 cString::toFloat64() const
{
    float64 value = 0;
    try {
        value = std::stod(_wstring);
    }
    catch (std::invalid_argument&) {
        throw cBadCastException(L"cString::toFloat64()");
    }
    catch (std::out_of_range&) {
        throw cBadCastException(L"cString::toFloat64()");
    }
    return value;
}