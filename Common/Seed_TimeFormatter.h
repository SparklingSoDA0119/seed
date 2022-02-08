#ifndef _SEED_TIME_FORMATTER_H
#define _SEED_TIME_FORMATTER_H

#include <Common/Seed_String.h>

namespace Seed {

    class cTime;

    class cTimeFormatter : public cObject {
    public:
        cTimeFormatter(const cString& format = L"") { setFormat(format); }

        cTimeFormatter(const cTimeFormatter& formatter);

        virtual ~cTimeFormatter() {}

    private:
        cString _format;
        cString _amString;
        cString _pmString;

    public:
        bool isValid() const { return !_format.isEmpty(); }
        const cString& format() const { return _format; }
        const cString& amString() const { return _amString; }
        const cString& pmString() const { return _pmString; }
        void setFormat(const cString& format) { _format = format; }
        void setAmpmString(const cString& am, const cString& pm)
        {
            _amString = am, _pmString = pm;
        }

    public:
        cString formatTime(const cTime& time) const;

    public:
        bool operator==(const cTimeFormatter& tf) const;
        bool operator!=(const cTimeFormatter& tf) const { return !operator==(tf); }

        virtual cString toString() const;

    public:
        static const cString FORMAT_YEAR4;
        static const cString FORMAT_YEAR2;
        static const cString FORMAT_MONTH;
        static const cString FORMAT_DAY;
        static const cString FORMAT_HOUR24;
        static const cString FORMAT_HOUR12;
        static const cString FORMAT_MINUTE;
        static const cString FORMAT_SECOND;
        static const cString FORMAT_AMPM;

    public:
        static cString format_YYYY_MM_DD(wchar_t dateSeparator = '-');
        static cString format_MM_DD_YYYY(wchar_t dateSeparator = '-');
        static cString format_DD_MM_YYYY(wchar_t dateSeparator = '-');
        static cString format_YY_MM_DD(wchar_t dateSeparator = '-');
        static cString format_MM_DD_YY(wchar_t dateSeparator = '-');
        static cString format_DD_MM_YY(wchar_t dateSeparator = '-');
        static cString format_H24_MM();

        static cString format_H24_MM_SS();
        static cString format_H12_MM_AP();
        static cString format_H12_MM_SS_AP();
        static cString format_AP_H12_MM();
        static cString format_AP_H12_MM_SS();
    };
}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SEED_TIME_FORMATTER_H
