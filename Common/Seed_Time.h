#ifndef _SODA_TIME_H
#define _SODA_TIME_H

#include <Common/Seed_Object.h>

#include <ctime>    // time_t, struct tm

namespace Seed {
    class cTime : public cObject {
    public:
        static bool isLeapYear(int32 year);
        static int32 daysInMonth(int32 year, int32 month);

    public:
        static cTime currentTime();
        static void setCurrentTime(const cTime& t);

    public:
        cTime(int32 year, int32 month, int32 day,
            int32 hour = 0, int32 minute = 0, int32 second = 0)
        {
            set(year, month, day, hour, minute, second);
        }

        cTime(const cTime& t) { set(t); }

        explicit cTime(time_t32 t) { from_time_t(t); }

        explicit cTime(const struct tm& lt) { from_struct_tm(lt); }

        cTime() { makeInvalid(); }

        virtual ~cTime() {}

    public:
        static const int32 MIN_YEAR = 1970;
        static const int32 MAX_YEAR = 2033; // MIN_YEAR + 63;

        enum DayOfWeek { SUN = 0, MON, TUE, WED, THU, FRI, SAT };

    private:
        union {
            struct {
#if defined(_BIG_ENDIAN)
                uint32 year : 6;    // 0 ~ 63
                uint32 month : 4;    // 1 ~ 12
                uint32 day : 5;    // 1 ~ 31
                uint32 hour : 5;    // 0 ~ 23
                uint32 minute : 6;    // 0 ~ 59
                uint32 second : 6;    // 0 ~ 59
#else
                uint32 second : 6;    // 0 ~ 59
                uint32 minute : 6;    // 0 ~ 59
                uint32 hour : 5;    // 0 ~ 23
                uint32 day : 5;    // 1 ~ 31
                uint32 month : 4;    // 1 ~ 12
                uint32 year : 6;    // 0 ~ 63
#endif
            } _field;
            uint32 _value;
        };

    public:
        bool isValid() const;
        int32 year() const { return MIN_YEAR + _field.year; }
        int32 month() const { return _field.month; }
        int32 day() const { return _field.day; }
        int32 hour() const { return _field.hour; }
        int32 minute() const { return _field.minute; }
        int32 second() const { return _field.second; }
        DayOfWeek dayOfWeek() const;

    public:
        void makeInvalid();
        void set(int32 year, int32 month, int32 day, int32 hour = 0, int32 minute = 0, int32 second = 0);
        void set(const cTime& t) { _value = t._value; }

        cTime& operator=(const cTime& t) { set(t); return *this; }

    public:
        bool operator==(const cTime& t) const { return _value == t._value; }
        bool operator!=(const cTime& t) const { return _value != t._value; }
        bool operator<=(const cTime& t) const { return _value <= t._value; }
        bool operator< (const cTime& t) const { return _value < t._value; }
        bool operator>=(const cTime& t) const { return _value >= t._value; }
        bool operator> (const cTime& t) const { return _value > t._value; }

    public:
        time_t32 to_time_t() const;
        struct tm to_struct_tm() const;

    public:
        void from_time_t(time_t32 t);
        void from_struct_tm(const struct tm& t);

    public:
        time_t32 utcTime() const;
        void setUtcTime(time_t32 t);

    public:
        uint32 toUint32() const { return _value; }
        void fromUint32(uint32 i) { _value = i; }
        virtual cString toString() const;

    public:
        typedef void (*RtcFunc)(const cTime& time);
        static RtcFunc _setRtc;
        static void setRtcFunc(RtcFunc func) { _setRtc = func; }
    };

#if defined(_MSC_VER)
    inline cTime cTime::currentTime() { return cTime(_time32(0)); }
#else
    inline cTime cTime::currentTime() { return cTime(time(0)); }
#endif

    bool isSameDate(const cTime& t1, const cTime& t2);
    bool isSameHour(const cTime& t1, const cTime& t2);
    bool isSameMinute(const cTime& t1, const cTime& t2);

}   // namespace Seed

////////////////////////////////////////////////////////////////////////////////
#endif  // _SODA_TIME_H
#pragma once
