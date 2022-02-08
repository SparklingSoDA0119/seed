#include <Common/Seed_StringUtil.h>

#include <sstream>

namespace Seed {

    namespace StringUtil {

        cString lastPathComponent(const cString& path)
        {
            uint32 i = path.findLastOf(L"/\\");
            if (i == cString::NPOS) {
                return path;
            }
            uint32 length = path.length();
            if (length == 1) {
                return path;
            }
            else if (i < length - 1) {
                return path.substring(i + 1);
            }
            else {
                i = path.findLastOf(L"/\\", length - 2);
                return path.substring(i + 1, length - 2 - (i + 1) + 1);
            }
        }

        void removeLastPathComponent(cString* path)
        {
            removeLastPathSeparator(path);
            cString s = lastPathComponent(*path);
            if (0 < s.length() && s != *path) {
                path->resize(path->length() - s.length());
                removeLastPathSeparator(path);
            }
        }

        void removeLastPathSeparator(cString* path)
        {
            uint32 i = path->findLastOf(L"/\\");
            if (i != cString::NPOS) {
                uint32 length = path->length();
                if (0 < i && i == length - 1) {
                    path->resize(length - 1);
                }
            }
        }

        cString filenameOnly(const cString& pathname)
        {
            uint32 si = pathname.findLastOf(L"/\\");
            si = (si == cString::NPOS) ? 0 : (si + 1);

            uint32 ei = pathname.findLastOf(L".");
            if (ei == cString::NPOS) {
                ei = pathname.length();
            }
            return (si == ei) ? pathname : pathname.substring(si, ei - si);
        }

        cString fileExtensionOnly(const cString& pathname)
        {
#if defined(__linux__)
            if (pathname == L"." || pathname == L"..") {
                return cString(L"");
            }
#endif
            uint32 i = pathname.findLastOf(L".");
            return (i == cString::NPOS) ? cString(L"") : pathname.substring(i + 1);
        }

        bool checkFileExtension(const cString& pathname, const cString& fileExt)
        {
#if defined(_MSC_VER)
            cString ext = fileExtensionOnly(pathname);
            return fileExtensionOnly(pathname).caseInsensitiveCompare(fileExt);
#else
            return fileExtensionOnly(pathname) == fileExt;
#endif
        }

        int32 htoi(const cString& hex)
        {
            int32 temp = 0;
            std::istringstream in(hex.to_string());
            in >> std::hex >> temp;

            return temp;
        }

#if defined(__linux__)
        inline int32 sbcsCode(const char** s)
        {
            return int32(*(*s)++) & 0x000000ff;
        }

        inline int32 mbcsCode(const char** s)
        {
            int32 code = int32(*(*s)++) & 0x000000ff;
            if ((code & 0x80) && **s != '\0') {
                code = (code << 8) | (int32(*(*s)++) & 0x000000ff);
            }
            return code;
        }

        inline bool isVisibleChar(const char ch)
        {
            return (0x20 <= ch && 0x7F != ch);
        }

        uint32 replaceIllegalUtf8Internal(const char* src, const char rep, \
            bool isMBCS, char* cString, cString* cString)
        {
            if (!src || '\0' == src[0]) {
                return 0;
            }

            if (' ' != rep && !::isalnum(rep)) {
                return 0;
            }

            if (!cString && !cString) {
                return 0;
            }

            const char repStr[2] = { rep, 0 };
            const char* s = src;
            const char* p;
            char tmp[3];

            int32 bytes = 0;

            while (s && '\0' != *s) {
                p = s;
                int32 code = (isMBCS ? mbcsCode(&s) : sbcsCode(&s));
                if (code) {
                    int32 len = (int32)(s - p);
                    if (1 == len) {
                        tmp[0] = p[0];
                        tmp[1] = '\0';
                    }
                    else if (2 == len) {
                        tmp[0] = p[0];
                        tmp[1] = p[1];
                        tmp[2] = '\0';
                    }
                    else {
                        VIXASSERT(false, L"invalid charcode!");
                    }

                    // detecting illegal UTF-8 character.
                    //  - should system-locale properly set.
                    cString utf8Char(tmp);

                    if (utf8Char.length() > 0 && (2 == len || isVisibleChar(tmp[0]))) {
                        if (cString) {
                            // convetible to UTF-8 character.
                            ::strcat(cString, tmp);
                        }
                        else {
                            cString->append(utf8Char);
                        }
                        bytes += len;
                    }
                    else {
                        // un-convetible to UTF-8 character.
                        if (cString) {
                            ::strcat(cString, repStr);
                        }
                        else {
                            cString->appendf(L"%s", repStr);
                        }
                        bytes++;
                        if (2 == len) {
                            tmp[0] = p[1];
                            tmp[1] = '\0';
                            cString utf8Char2(tmp);
                            if (utf8Char2.length() > 0 && isVisibleChar(tmp[0])) {
                                if (cString) {
                                    // convetible to UTF-8 character.
                                    ::strcat(cString, tmp);
                                }
                                else {
                                    cString->append(utf8Char2);
                                }
                                bytes++;
                            }
                            else {
                                // un-convetible to UTF-8 character.
                                if (cString) {
                                    ::strcat(cString, repStr);
                                }
                                else {
                                    cString->appendf(L"%s", repStr);
                                }
                                bytes++;
                            }
                        }
                    }
                }
            }

            return bytes;
        }

        uint32 replaceIllegalUtf8(char* cString, const char* src, \
            bool isMBCS, const char rep)
        {
            return replaceIllegalUtf8Internal(src, rep, isMBCS, cString, 0);
        }

        uint32 replaceIllegalUtf8(cString* utf8String, const char* src, \
            bool isMBCS, const char rep)
        {
            return replaceIllegalUtf8Internal(src, rep, isMBCS, 0, utf8String);
        }
#endif  // __linux__

        cString numericSubStr(const cString& str)
        {
            int32 start = -1, end = -1;
            cString ret;
            for (int32 i = 0; i < static_cast<int32>(str.length()); ++i) {
                if (start < 0 && L'0' <= str[i] && str[i] <= L'9') {
                    start = i;
                }
                else if (start >= 0 && (L'0' > str[i] || str[i] > L'9')) {
                    end = i;
                    break;
                }
            }
            if (start >= 0) {
                if (end < 0) {
                    end = str.length();
                }
                ret = str.substring(start, end - start);
            }
            return ret;
        }

        void binaryToHexaString(cString* dest, const uint8* src, int32 srcLength)
        {
            *dest = L"";
            cString temp;
            for (int32 i = 0; i < srcLength; ++i) {
                temp.format(L"%02X", src[i]);
                dest->append(temp);
            }
        }

        void binaryFromHexaString(uint8* dest, const cString& src)
        {
            int32 index = 0;
            for (uint32 i = 0; i < src.length(); i = i + 2) {
                std::wstring upper = src.substring(i, 1).to_wstring();
                std::wstring lower = src.substring(i + 1, 1).to_wstring();

                dest[index++] = static_cast<uint8>((wcstol(upper.c_str(), 0, 16) << 4) + wcstol(lower.c_str(), 0, 16));
            }
        }

    }   // end of cStringUtil

}   // end of Vix


// <EOF>

