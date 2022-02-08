#ifndef _SEED_STRING_UTIL_H
#define _SEED_STRING_UTIL_H

#include <Common/Seed_String.h>
#include <string.h>
#include <stdio.h>

namespace Seed {

    namespace StringUtil {
        cString lastPathComponent(const cString& path);
        void removeLastPathComponent(cString* path);
        void removeLastPathSeparator(cString* path);
        cString filenameOnly(const cString& pathname);
        cString fileExtensionOnly(const cString& pathname);
        bool checkFileExtension(const cString& pathname, const cString& fileExt);
        int32 htoi(const cString& hex);

#if defined(__linux__)
        uint32 replaceIllegalUtf8(char* cString, const char* src, bool isMBCS = false, const char rep = ' ');
        uint32 replaceIllegalUtf8(cString* utf8String, const char* src, bool isMBCS = false, const char rep = ' ');
#endif

        cString numericSubStr(const cString& str);
        void binaryToHexaString(cString* dest, const uint8* src, int32 srcLength);
        void binaryFromHexaString(uint8* dest, const cString& src);

        template<class T>
        bool numberFromString(T* result, const char* s, bool isMonetary = true)
        {
            try {
                if (!s || !*s || !result) {
                    throw "null pointer exception";
                }
                struct lconv* conf = localeconv();
                if (!conf) {
                    throw "localeconv failed";
                }
                const char* decimalPointLocale = \
                    (isMonetary ? conf->mon_decimal_point : conf->decimal_point);
                const char* thousandsSepLocale = \
                    (isMonetary ? conf->mon_thousands_sep : conf->thousands_sep);
                const char* negativeSign = \
                    (conf->negative_sign && conf->negative_sign[0] ? conf->negative_sign : "-");
                const char* positiveSign = \
                    (conf->positive_sign && conf->positive_sign[0] ? conf->positive_sign : "+");
                const char* currencySymbol = \
                    (conf->currency_symbol && conf->currency_symbol[0] ? conf->currency_symbol : "$");
                const char* decimalPoint = \
                    (decimalPointLocale && decimalPointLocale[0] ? decimalPointLocale : ".");
                const char* thousandsSep = \
                    (thousandsSepLocale && thousandsSepLocale[0] ? thousandsSepLocale : ",");
                const char* hexHeading = "0x";

                uint32 decimalPointLen = static_cast<uint32>(strlen(decimalPoint));
                uint32 thousandsSepLen = static_cast<uint32>(strlen(thousandsSep));
                uint32 negativeSignLen = static_cast<uint32>(strlen(negativeSign));
                uint32 positiveSignLen = static_cast<uint32>(strlen(positiveSign));
                uint32 currencySymbolLen = static_cast<uint32>(strlen(currencySymbol));
                uint32 hexHeadingLen = static_cast<uint32>(strlen(hexHeading));

                char buffer[256], * end = 0, * p = buffer;
                bool hexMode = false, inverse = false;
                bool haveSign = false;
                uint32 length = 0;

                // copy number string
                buffer[0] = buffer[255] = '\0';
                for (char* dest = buffer; s && *s != '\0'; ) {
                    bool saveChar = false;
                    if (!isspace(*s)) {
                        if (0 == length && !strncmp(s, negativeSign, negativeSignLen)) {
                            //trace("negativeSign\n");
                            saveChar = true;
                            haveSign = inverse = true;
                        }
                        else if (0 == length && !strncmp(s, positiveSign, positiveSignLen)) {
                            //trace("positiveSign\n");
                            saveChar = true;
                            haveSign = true;
                        }
                        else if (0 == length && !strncmp(s, hexHeading, hexHeadingLen)) {
                            //trace("hexHeading\n");
                            hexMode = true;
                            s += hexHeadingLen;
                            continue;
                        }
                        else if (haveSign && !strncmp(s, negativeSign, negativeSignLen)) {
                            throw "invalid negative sign";
                        }
                        else if (haveSign && !strncmp(s, positiveSign, positiveSignLen)) {
                            throw "invalid positive sign";
                        }
                        else if ((haveSign ? 1 : 0) != length && !strncmp(s, thousandsSep, thousandsSepLen)) {
                            //trace("thousandsSep\n");
                            s += thousandsSepLen;
                            continue;
                        }
                        else if ((haveSign ? 1 : 0) == length && !strncmp(s, currencySymbol, currencySymbolLen)) {
                            //trace("currencySymbol\n");
                            if (!isMonetary)
                                break;  // invalid character
                            s += currencySymbolLen;
                            continue;
                        }
                        else if (!strncmp(s, decimalPoint, decimalPointLen)) {
                            //trace("decimalPoint\n");
                            saveChar = true;
                        }
                        else if ((*s == 'e' || *s == 'E') && !strncmp(s + 1, negativeSign, negativeSignLen)) {
                            //trace("E-###\n");
                            *dest++ = *s++; *dest = '\0'; // E
                            *dest++ = *s++; *dest = '\0'; // -
                            continue;
                        }
                        else if ((*s == 'e' || *s == 'E') && !strncmp(s + 1, positiveSign, positiveSignLen)) {
                            //trace("E+###\n");
                            *dest++ = *s++; *dest = '\0'; // E
                            *dest++ = *s++; *dest = '\0'; // +
                            continue;
                        }
                        else if (hexMode && !isxdigit(*s)) {
                            //trace("invalid character(hexmod)!!\n");
                            break;
                        }
                        else if (!hexMode && !isdigit(*s)) {
                            //trace("invalid character!!\n");
                            break;
                        }
                        else if (hexMode && isxdigit(*s)) {
                            saveChar = true;
                        }
                        else if (!hexMode && isdigit(*s)) {
                            saveChar = true;
                        }
                    }
                    else if ((haveSign ? 1 : 0) != length) {
                        *dest = '\0';
                        break;
                    }

                    if (saveChar) {
                        *dest++ = *s; *dest = '\0';
                        length++;
                    }
                    s++;
                }

                //trace("buffer = '%s'\n", buffer);
                length = strlen(p);
                if ((haveSign ? 1 : 0) == length) {
                    throw "invalid number string";
                }

                if (hexMode) {
                    *result = (T)strtoul(p, &end, 16);
                }
                else {
                    *result = (T)strtol(p, &end, 10);
                    p = end;

                    if (0 == strncmp(p, decimalPoint, decimalPointLen)) {
                        p += decimalPointLen;

                        T pl = (T)strtoul(p, &end, 10);
                        uint32 n = end - p;
                        p = end;

                        if (n >= 20) {
                            throw "array boundary!";
                        }

                        double CONV_TABLE[20];
                        for (uint32 i = 0; i < 20; ++i) {
                            if (0 == i) {
                                CONV_TABLE[i] = 0.0f;
                            }
                            else {
                                CONV_TABLE[i] = 1.0f;
                                for (uint32 j = 0; j < i; ++j) {
                                    CONV_TABLE[i] /= 10.0f;
                                }
                            }
                        }

                        pl *= (T)CONV_TABLE[n];
                        if (inverse) {
                            pl *= (T)-1;
                        }
                        *result += pl;
                    }

                    if (*p == 'e' || *p == 'E') { //  Exp.
                        ++p;

                        bool minusExp = false;
                        if (0 == strncmp(p, negativeSign, negativeSignLen)) {
                            minusExp = true;
                            p += negativeSignLen;
                        }

                        T exp = (T)strtol(p, &end, 10);
                        T val = (T)1.0f;

                        for (uint32 i = 0; i < exp; ++i) {
                            val = (T)(minusExp ? val / 10.0f : val * 10.0f);
                        }

                        if (inverse) {
                            val *= (T)-1;
                        }

                        *result *= val;
                    }
                }
            }
            catch (const char* e) {
#if defined(DEBUG) || defined(_DEBUG)
                fprintf(stderr, "Exception: %s!\n", e);
#endif
                return false;
            }
            catch (...) {
#if defined(DEBUG) || defined(_DEBUG)
                fprintf(stderr, "Exception: unknown (maybe localeconv)!\n");
#endif
                return false;
            }

            return true;
        }
    }   // end of StringUtil
}   // end of Vix

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_STRING_UTIL_H