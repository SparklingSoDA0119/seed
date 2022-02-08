#ifndef _SEED_STRING_H_
#define _SEED_STRING_H_

#include <Common/Seed_Object.h>
#include <Common/Seed_Mutex.h>
#include <string>
#include <locale>
#include <functional>
#include <cstdarg>

#if defined(_MSC_VER)
#include <windows.h>
#endif

namespace Seed {
    
    class cString : public cObject {
    public :
        static cRecursiveMutex& defaultLocaleMutex()
        {
            static cRecursiveMutex s_defaultLocaleMutex(false);
            return s_defaultLocaleMutex;
        }

        static const std::locale& defaultLocale()
        {
            static std::locale s_defaultLocale(std::locale(""));

            cAutoMutex am(defaultLocaleMutex());
            return s_defaultLocale;
        }

        static void setDefaultLocale(const std::locale& loc)
        {
            cAutoMutex am(defaultLocaleMutex());
            const_cast<std::locale&>(defaultLocale()) = loc;
        }

        static const std::string& defaultCharset()
        {
            static std::string s_defaultCharset("");
            
            cAutoMutex am(defaultLocaleMutex());
            return s_defaultCharset;
        }

        static void setDefaultCharset(const std::string& charset)
        {
            cAutoMutex am(defaultLocaleMutex());
            const_cast<std::string&>(defaultCharset()) = charset;
        }

    public :
        explicit cString() {}
        cString(const cString& s) : _wstring(s._wstring) {}
        cString(const cString& s, uint32 index, uint32 length = NPOS);
        cString(const wchar_t* wcs);
        cString(const wchar_t* wcs, uint32 length);
        cString(uint32 count, wchar_t ch) : _wstring(static_cast<uint32>(count), ch) {}
        cString(const std::string& cs);
        cString(const std::string& cs, const std::locale& loc);
        cString(const std::string& cs, const std::string& charset);
        explicit cString(const char* cs);
        explicit cString(const char* cs, const std::locale& loc);
        explicit cString(const char* cs, const std::string& charset);
        cString(cString&& s) noexcept = default;
        
        virtual ~cString() {}

    public :
        static const uint32 NPOS = 0xFFFFFFFF;

    private :
        std::wstring _wstring;
        
    public :
        virtual cString* clone() const { return new cString(*this); }

    public :
        bool isEmpty() const { return _wstring.empty(); }
        uint32 length() const { return static_cast<uint32>(_wstring.length()); }
        uint32 capacity() const { return static_cast<uint32>(_wstring.capacity()); }
        void reserve(uint32 capacity);

        void clear() { _wstring.clear(); }
        void resize(uint32 size);
        void resize(uint32 size, wchar_t wc);
        void assign(const cString& s) { _wstring.assign(s._wstring); }
        void append(uint32 count, wchar_t wc) { _wstring.append(count, wc); }
        void append(const cString& s) { _wstring.append(s._wstring); }

    public :
        cString& operator=(const cString& s) { _wstring.assign(s._wstring); return *this; }
        cString& operator=(cString&& s) noexcept { _wstring = std::move(s._wstring); return *this; }
        cString& operator+=(const cString& s) { _wstring.append(s._wstring); return *this; }
        cString& operator+=(wchar_t wc) { _wstring += wc; return *this; }
        void swap(cString& s) { _wstring.swap(s._wstring); }
        void makeUpper()
        {
            cAutoMutex am(defaultLocaleMutex());
            makeUpper(defaultLocale());
        }

        void makeUpper(const std::locale& loc);

        void makeLower()
        {
            cAutoMutex am(defaultLocaleMutex());
            makeLower(defaultLocale());
        }

        void makeLower(const std::locale& loc);
        void trimLeft();
        void trimRight();
        void adjustToBytes(uint32 bytes);

    public :
        void format(const wchar_t* format, ...);
        void format(const cString* format, ...);
        void vformat(const wchar_t* format, va_list args);
        void vformat(const cString* format, va_list args);
        void appendf(const wchar_t* format, ...);
        void appendf(const cString* format, ...);
        void appendvf(const wchar_t* format, va_list args);
        void appendvf(const cString* format, va_list args);

    public :
        const wchar_t& operator[](uint32 index) const { return _wstring[index]; }
        wchar_t& operator[](uint32 index)       { return _wstring[index]; }
        const wchar_t& at(uint32 index) const;
         wchar_t& at(uint32 index);

    public :
        int32 compare(const cString& s) const { return _wstring.compare(s._wstring); }
        int32 compare(uint32 index, uint32 length, const cString& s) const;
        bool caseInsensitiveCompare(const cString& s) const;

    public:
        bool operator==(const cString& s)  const { return _wstring == s._wstring; }
        bool operator!=(const cString& s)  const { return _wstring != s._wstring; }
        bool operator<=(const cString& s)  const { return _wstring <= s._wstring; }
        bool operator< (const cString& s)  const { return _wstring <  s._wstring; }
        bool operator>=(const cString& s)  const { return _wstring >= s._wstring; }
        bool operator> (const cString& s)  const { return _wstring >  s._wstring; }
        bool operator==(const wchar_t* wcs) const { return _wstring == wcs; }
        bool operator!=(const wchar_t* wcs) const { return _wstring != wcs; }
        bool operator<=(const wchar_t* wcs) const { return _wstring <= wcs; }
        bool operator< (const wchar_t* wcs) const { return _wstring <  wcs; }
        bool operator>=(const wchar_t* wcs) const { return _wstring >= wcs; }
        bool operator> (const wchar_t* wcs) const { return _wstring >  wcs; }
        friend bool operator==(const wchar_t* wcs, const cString& s);
        friend bool operator!=(const wchar_t* wcs, const cString& s);
        friend bool operator<=(const wchar_t* wcs, const cString& s);
        friend bool operator< (const wchar_t* wcs, const cString& s);
        friend bool operator>=(const wchar_t* wcs, const cString& s);
        friend bool operator> (const wchar_t* wcs, const cString& s);

    public:
        uint32 find(const cString& s, uint32 index = 0) const;
        uint32 findFirstOf(const cString& s, uint32 index = 0) const;
        uint32 findFirstNotOf(const cString& s, uint32 index = 0) const;
        uint32 findLastOf(const cString& s, uint32 index = NPOS) const;
        uint32 findLastNotOf(const cString& s, uint32 index = NPOS) const;
        uint32 caseInsensitiveFind(const cString& s) const;
        uint32 reverseFind(const cString& s, uint32 index = NPOS) const;
        uint32 count(wchar_t ch) const;
        uint32 count(const cString& s) const;
        cString& replace(uint32 index, uint32 length, const cString& s);
        cString& replace(const cString& src, const cString& dst, bool all = true);
        cString& insert(uint32 index, const cString& s);
        cString substring(uint32 index, uint32 length = NPOS) const;

    public :
        const wchar_t* to_cstring() const { return _wstring.c_str(); }
        const std::wstring& to_wstring() const { return _wstring; }
        void from_wstring(const std::wstring& ws) { _wstring = ws; }

    public :
        std::string to_string() const
        {
            cAutoMutex am(defaultLocaleMutex());
            if (!defaultCharset().empty()) {
                return to_string(defaultCharset());
            }
            return to_string(defaultLocale());
        }

        std::string to_string(const std::locale& loc) const;
        std::string to_string(const std::string& charset) const;

        void from_string(const std::string& cs)
        {
            cAutoMutex am(defaultLocaleMutex());
            if (!defaultCharset().empty()) {
                from_string(cs, defaultCharset());
            }
            else {
                from_string(cs, defaultLocale());
            }
        }

        void from_string(const std::string& cs, const std::locale& loc);
        void from_string(const std::string& cs, const std::string& charset);
        void from_string(const char* cs)
        {
            cAutoMutex am(defaultLocaleMutex());
            if (!defaultCharset().empty()) {
                from_string(cs, defaultCharset());
            }
            else {
                from_string(cs, defaultLocale());
            }
        }

        void from_string(const char* cs, const std::locale& loc);
        void from_string(const char* cs, const std::string& charset);

    public:

    #if defined(_MSC_VER)
        #if defined(_UNICODE)
            const TCHAR* to_LPCTSTR() const { return to_wstring().c_str(); }
        #else
            // Fix me.
        #endif
    #endif

    public:
        int8    toInt8() const;
        uint8   toUint8() const;
        int16   toInt16() const;
        uint16  toUint16() const;
        int32   toInt32() const;
        uint32  toUint32() const;
        int64   toInt64() const;
        uint64  toUint64() const;
        float32 toFloat32() const;
        float64 toFloat64() const;

    public:
        virtual size_t hash() const { return std::hash<std::wstring>()(_wstring); }

    public:
        virtual cString toString() const { return *this; }
    };

    inline cString::cString(const cString& s, uint32 index, uint32 length)
        : _wstring(s._wstring, static_cast<size_t>(index),
                   (NPOS == length) ? std::wstring::npos : static_cast<size_t>(length)) {}

    inline cString::cString(const wchar_t* wcs)
    {
        try {
            _wstring = std::wstring(wcs);
        }
        catch (std::exception const&) {
            abort(); // cant not use VixASSERT because cString is more lower class than VixASSERT
        }
    }
    
    inline cString::cString(const wchar_t* wcs, uint32 length) : _wstring(wcs, static_cast<size_t>(length)) {}
    inline cString::cString(const std::string& cs) { from_string(cs); }
    inline cString::cString(const std::string& cs, const std::locale& loc) { from_string(cs, loc); }
    inline cString::cString(const std::string& cs, const std::string& charset) { from_string(cs, charset); }
    inline cString::cString(const char* cs) { from_string(cs); }
    inline cString::cString(const char* cs, const std::locale& loc) { from_string(cs, loc); }
    inline cString::cString(const char* cs, const std::string& charset) { from_string(cs, charset); }

    inline void cString::reserve(uint32 capacity)
    { _wstring.reserve(static_cast<std::wstring::size_type>(capacity)); }

    inline void cString::resize(uint32 size)
    { _wstring.resize(static_cast<std::wstring::size_type>(size)); }
    
    inline void cString::resize(uint32 size, wchar_t wc)
    { _wstring.resize(static_cast<std::wstring::size_type>(size), wc); }

    inline wchar_t& cString::at(uint32 index)
    { return const_cast<wchar_t&>(static_cast<const cString*>(this)->at(index)); }

    inline void cString::from_string(const char* cs, const std::locale& loc)
    {
       try {
           from_string(std::string(cs), loc);
       }
       catch (std::exception const&) {
           abort(); // cant not use VixASSERT because cString is more lower class than VixASSERT
       }
    }

    inline void cString::from_string(const char* cs, const std::string& charset)
    {
        try {
            from_string(std::string(cs), charset);
        }
        catch (std::exception const&) {
            abort(); // cant not use VixASSERT because cString is more lower class than VixASSERT
        }
    }

    inline bool operator==(const wchar_t* wcs, const cString& s) { return wcs == s._wstring; }
    inline bool operator!=(const wchar_t* wcs, const cString& s) { return wcs != s._wstring; }
    inline bool operator<=(const wchar_t* wcs, const cString& s) { return wcs <= s._wstring; }
    inline bool operator< (const wchar_t* wcs, const cString& s) { return wcs <  s._wstring; }
    inline bool operator>=(const wchar_t* wcs, const cString& s) { return wcs >= s._wstring; }
    inline bool operator> (const wchar_t* wcs, const cString& s) { return wcs >  s._wstring; }

    inline cString operator+(const cString& s1, const cString& s2) { return cString(s1) += s2; }
    inline cString operator+(const wchar_t* wcs, const cString& s) { return cString(wcs) += s; }
    inline cString operator+(wchar_t wc, const cString& s) { return cString(1, wc) += s; }
    inline cString operator+(const cString& s, const wchar_t* wcs) { return cString(s) += wcs; }
    inline cString operator+(const cString& s, wchar_t wc) { return cString(s) += wc; }
}

namespace std
{
    template <>
    struct hash<Seed::cString> {
        size_t operator()(const Seed::cString& s) const { return s.hash(); }
    };
} 

#endif