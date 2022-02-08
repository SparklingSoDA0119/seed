#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include <Common/Seed_Time.h>
#include <Common/Seed_TimeFormatter.h>
#include <Common/Seed_Assert.h>
//#include <Common/Seed_SystemUtil.h>
#include <Common/Seed_StringUtil.h>

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

using namespace Seed;

cTime::RtcFunc cTime::_setRtc = 0;

namespace { // unnamed

    const int32 s_monthDays[13] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
    };

}   // unnamed namespace

bool cTime::isLeapYear(int32 year)
{
    return (((year & 3) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

int32 cTime::daysInMonth(int32 year, int32 month)
{
    SEED_ASSERT(1 <= month && month <= 12, L"");

    int32 days = s_monthDays[month] - s_monthDays[month - 1];
    return (isLeapYear(year) && month == 2) ? days + 1 : days;
}

////////////////////////////////////////////////////////////////////////////////

void cTime::setCurrentTime(const cTime& t)
{
    SEED_ASSERT(t.isValid(), L"");
#if defined(_MSC_VER)
#else
    struct timeval tv;
    //struct timezone tz;
    tv.tv_sec = t.to_time_t();
    tv.tv_usec = 0;
    settimeofday(&tv, 0/*&tz*/);
#endif
    if (_setRtc) {
        _setRtc(t);
    }
}

////////////////////////////////////////////////////////////////////////////////

namespace { // unnamed

    bool isValidTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
    {
        return (cTime::MIN_YEAR <= year && year <= cTime::MAX_YEAR &&
            1 <= month && month <= 12 &&
            1 <= day && day <= cTime::daysInMonth(year, month) &&
            0 <= hour && hour <= 23 &&
            0 <= minute && minute <= 59 &&
            0 <= second && second <= 59);
    }

}   // unnamed namespace

bool cTime::isValid() const
{
    return isValidTime(year(), month(), day(),
        hour(), minute(), second());
}

cTime::DayOfWeek cTime::dayOfWeek() const
{
    // DayOfWeek index is same with struct tm::tm_wday.
    return static_cast<DayOfWeek>(to_struct_tm().tm_wday);
}

void cTime::makeInvalid()
{
    _field.year = 0;
    _field.month = 13;
    _field.day = 0;
    _field.hour = 25;
    _field.minute = 61;
    _field.second = 61;
}

void cTime::set(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
    if (isValidTime(year, month, day, hour, minute, second)) {
        _field.year = year - MIN_YEAR;
        _field.month = month;
        _field.day = day;
        _field.hour = hour;
        _field.minute = minute;
        _field.second = second;
    }
    else {
        makeInvalid();
    }
}

////////////////////////////////////////////////////////////////////////////////

namespace { // unnamed

    struct tm convertToTM(const cTime& time)
    {
        struct tm t;
        t.tm_year = time.year() - 1900;
        t.tm_mon = time.month() - 1;
        t.tm_mday = time.day();
        t.tm_hour = time.hour();
        t.tm_min = time.minute();
        t.tm_sec = time.second();
        t.tm_isdst = -1;
        return t;
    }

    inline time_t32 platform_mktime(struct tm* lt)
    {
#if defined(_MSC_VER)
        return ::_mktime32(lt);
#else
        return static_cast<time_t32>(::mktime(lt));
#endif
    }

    inline bool platform_localtime(time_t32 t, struct tm* lt)
    {
#if defined(_MSC_VER)
        struct tm* p = ::_localtime32(&t);
        return (p) ? (*lt = *p, true) : false;
#else
        time_t tt = static_cast<time_t>(t);
        return (0 != ::localtime_r(&tt, lt)); // thread-safe version
#endif
    }

    inline bool platform_gmtime(time_t32 t, struct tm* gt)
    {
#if defined(_MSC_VER)
        struct tm* p = ::_gmtime32(&t);
        return (p) ? (*gt = *p, true) : false;
#else
        time_t tt = static_cast<time_t>(t);
        return (0 != ::gmtime_r(&tt, gt)); // thread-safe version
#endif
    }
}   // unnamed namespace

time_t32 cTime::to_time_t() const
{
    struct tm lt = convertToTM(*this);
    return ::platform_mktime(&lt);
}

struct tm cTime::to_struct_tm() const
{
    struct tm lt = convertToTM(*this);
    ::platform_mktime(&lt);
    return lt;
}

void cTime::from_time_t(time_t32 t)
{
    struct tm lt;
    if (::platform_localtime(t, &lt)) {
        from_struct_tm(lt);
    }
    else {
        makeInvalid();
    }
}

void cTime::from_struct_tm(const struct tm& t)
{
    set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
        t.tm_hour, t.tm_min, t.tm_sec);
}

////////////////////////////////////////////////////////////////////////////////

namespace { // unnamed

    inline uint32 numberOfLeapDays(uint32 from, uint32 to)
    {
        from--; to--;
        return (to / 4 - from / 4) - (to / 100 - from / 100) + (to / 400 - from / 400);
    }

    const int32 s_numberOfLeapDays[] = { // calculated by numberOfLeapDays()
         0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,
         4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,
         8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11, 11, 11, 11, 12,
        12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16,
    };

    void checkLimit(int32& val, int32 min, int32 max)
    {
        if (val < min) {
            val = min;
        }
        if (val >= max) {
            val = max - 1;
        }
    }

}   // unnamed namespace

time_t32 cTime::utcTime() const
{
    struct tm gt = convertToTM(*this);

    int32 year = gt.tm_year + 1900;
    int32 mon = gt.tm_mon;

#if defined(_DEBUG)
    assert(year >= MIN_YEAR && year < MAX_YEAR);
    assert(0 <= mon && mon < 12);
#else
    checkLimit(year, MIN_YEAR, MAX_YEAR);
    checkLimit(mon, 0, 12);
#endif

    int32 yearCount = year - MIN_YEAR;
    uint32 days = (yearCount * 365) + s_numberOfLeapDays[yearCount];
    days += s_monthDays[mon];
    if (mon > 1 && isLeapYear(year)) {
        days++;
    }
    days += (gt.tm_mday - 1);

    time_t32 hours = (time_t32)((days * 24) + gt.tm_hour);

    return (((hours * 3600) + gt.tm_min * 60) + gt.tm_sec);
}

void cTime::setUtcTime(time_t32 t)
{
    struct tm gt;
    if (::platform_gmtime(t, &gt)) {
        from_struct_tm(gt);
    }
    else {
        makeInvalid();
    }
}

cString cTime::toString() const
{
    if (!isValid()) {
        cString s;
        s.format(L"%04d-%02d-%02d %02d:%02d:%02d",
            year(), month(), day(), hour(), minute(), second());
        return s;
    }

    cString s;
    s.format(L"%@-%@-%@ %@:%@:%@",
        &cTimeFormatter::FORMAT_YEAR4,
        &cTimeFormatter::FORMAT_MONTH,
        &cTimeFormatter::FORMAT_DAY,
        &cTimeFormatter::FORMAT_HOUR24,
        &cTimeFormatter::FORMAT_MINUTE,
        &cTimeFormatter::FORMAT_SECOND);
    return cTimeFormatter(s).formatTime(*this);
}

namespace Seed {

    bool isSameDate(const cTime& t1, const cTime& t2)
    {
        return (t1.year() == t2.year() &&
            t1.month() == t2.month() &&
            t1.day() == t2.day());
    }

    bool isSameHour(const cTime& t1, const cTime& t2)
    {
        return (isSameDate(t1, t2) && t1.hour() == t2.hour());
    }

    bool isSameMinute(const cTime& t1, const cTime& t2)
    {
        return (isSameHour(t1, t2) && t1.minute() == t2.minute());
    }
}   // namespace Seed
