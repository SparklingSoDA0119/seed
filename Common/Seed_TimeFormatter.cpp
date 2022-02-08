#include <Common/Seed_TimeFormatter.h>
#include <Common/Seed_Time.h>


#include <algorithm>    // find
#include <iostream>

using namespace std;
using namespace Seed;

cTimeFormatter::cTimeFormatter(const cTimeFormatter& formatter)
{
    setFormat(formatter.format());
    setAmpmString(formatter.amString(), formatter.pmString());
}

cString cTimeFormatter::formatTime(const cTime& time) const
{
    static const std::wstring types(L"@AaBbCcDdEeFGgHhIjklMmnOpRrSsTtUuVvWwXxYyZz");

    struct tm tm = time.to_struct_tm();
    const cString& ampm = (time.hour() < 12) ? _amString : _pmString;

    const uint32 TS_N = 256;
    wchar_t ts[TS_N];
    std::wstring tf, result;
    const std::wstring& format = _format.to_wstring();
    std::wstring::const_iterator si = format.begin();
    std::wstring::const_iterator t, i = si;
    while (true) {
        i = std::find(i, format.end(), L'%');
        if (i == format.end()) {
            result.append(si, format.end());
            break;
        }

        if (*(i + 1) == L'%') {
            result.append(si, i + 1);
            i += 2;
            si = i;
            continue;
        }

        t = std::find_first_of(i, format.end(), types.begin(), types.end());
        if (t == format.end()) {
            std::wcerr << __FUNCTION__ << L"(): Unknown type "
                << "\"%" << *(i + 1) << "\"" << std::endl;
            abort();
            break;
        }

        result.append(si, i);

        tf.assign(i, t + 1);
        if (*t == L'p' && !ampm.isEmpty()) {
            result.append(ampm.to_wstring());
        }
        else {
            ::wcsftime(ts, TS_N, tf.c_str(), &tm);
            result.append(ts);
        }

        i = t + 1;
        si = i;
    }
    return cString(result.c_str());
}

bool cTimeFormatter::operator==(const cTimeFormatter& tf) const
{
    return _format == tf._format &&
        _amString == tf._amString &&
        _pmString == tf._pmString;
}

cString cTimeFormatter::toString() const
{
    cString s;
    s.format(L"format=\"%@\", am=\"%@\", pm=\"%@\"",
        &_format, &_amString, &_pmString);
    return s;
}

////////////////////////////////////////////////////////////////////////////////

// followings are stardard from wcsftime().
const cString cTimeFormatter::FORMAT_YEAR4(L"%Y");
const cString cTimeFormatter::FORMAT_YEAR2(L"%y");
const cString cTimeFormatter::FORMAT_MONTH(L"%m");
const cString cTimeFormatter::FORMAT_DAY(L"%d");
const cString cTimeFormatter::FORMAT_HOUR24(L"%H");
const cString cTimeFormatter::FORMAT_HOUR12(L"%I");
const cString cTimeFormatter::FORMAT_MINUTE(L"%M");
const cString cTimeFormatter::FORMAT_SECOND(L"%S");
const cString cTimeFormatter::FORMAT_AMPM(L"%p");

cString cTimeFormatter::format_YYYY_MM_DD(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_YEAR4, dateSeparator,
        &FORMAT_MONTH, dateSeparator,
        &FORMAT_DAY);
    return s;
}

cString cTimeFormatter::format_MM_DD_YYYY(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_MONTH, dateSeparator,
        &FORMAT_DAY, dateSeparator,
        &FORMAT_YEAR4);
    return s;
}

cString cTimeFormatter::format_DD_MM_YYYY(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_DAY, dateSeparator,
        &FORMAT_MONTH, dateSeparator,
        &FORMAT_YEAR4);
    return s;
}

cString cTimeFormatter::format_YY_MM_DD(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_YEAR2, dateSeparator,
        &FORMAT_MONTH, dateSeparator,
        &FORMAT_DAY);
    return s;
}

cString cTimeFormatter::format_MM_DD_YY(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_MONTH, dateSeparator,
        &FORMAT_DAY, dateSeparator,
        &FORMAT_YEAR2);
    return s;
}

cString cTimeFormatter::format_DD_MM_YY(wchar_t dateSeparator)
{
    cString s;
    s.format(L"%@%c%@%c%@", &FORMAT_DAY, dateSeparator,
        &FORMAT_MONTH, dateSeparator,
        &FORMAT_YEAR2);
    return s;
}

cString cTimeFormatter::format_H24_MM()
{
    cString s;
    s.format(L"%@:%@", &FORMAT_HOUR24,
        &FORMAT_MINUTE);
    return s;
}

cString cTimeFormatter::format_H24_MM_SS()
{
    cString s;
    s.format(L"%@:%@:%@", &FORMAT_HOUR24,
        &FORMAT_MINUTE,
        &FORMAT_SECOND);
    return s;
}

cString cTimeFormatter::format_H12_MM_AP()
{
    cString s;
    s.format(L"%@:%@ %@", &FORMAT_HOUR12,
        &FORMAT_MINUTE,
        &FORMAT_AMPM);
    return s;
}

cString cTimeFormatter::format_H12_MM_SS_AP()
{
    cString s;
    s.format(L"%@:%@:%@ %@", &FORMAT_HOUR12,
        &FORMAT_MINUTE,
        &FORMAT_SECOND,
        &FORMAT_AMPM);
    return s;
}

cString cTimeFormatter::format_AP_H12_MM()
{
    cString s;
    s.format(L"%@ %@:%@", &FORMAT_AMPM,
        &FORMAT_HOUR12,
        &FORMAT_MINUTE);
    return s;
}

cString cTimeFormatter::format_AP_H12_MM_SS()
{
    cString s;
    s.format(L"%@ %@:%@:%@", &FORMAT_AMPM,
        &FORMAT_HOUR12,
        &FORMAT_MINUTE,
        &FORMAT_SECOND);
    return s;
}
