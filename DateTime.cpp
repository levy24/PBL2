#include "DateTime.h"
#include <iostream>
using namespace std;
DateTime::DateTime()
{}

DateTime::DateTime(int y, int m, int d, int h, int min)
    : year(y), month(m), day(d), hour(h), minute(min)
{}

int DateTime::getDay() const
{
    return day;
}

int DateTime::getMonth() const
{
    return month;
}

int DateTime::getYear() const
{
    return year;
}

int DateTime::getHour() const
{
    return hour;
}

int DateTime::getMinute() const
{
    return minute;
}

string DateTime::getDate() const
{
    string monthStr = (month < 10) ? "0" + to_string(month) : to_string(month);
    string dayStr = (day < 10) ? "0" + to_string(day) : to_string(day);
    return to_string(year) + "-" + monthStr + "-" + dayStr;
}
string DateTime::getTime() const
{
    string hourStr = (hour < 10) ? "0" + to_string(hour) : to_string(hour);
    string minStr = (minute < 10) ? "0" + to_string(minute) : to_string(minute);
    return hourStr + ":" + minStr;
}

bool DateTime::isLapYear() const
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return true;
    return false;
}

bool DateTime::isValidDateTime() const
{
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31 || hour < 0 || hour >= 24 || minute < 0 || minute >= 60)
        return false;
    if (month == 2)
    {
        if (isLapYear() && day > 29)
            return false;
        if (!isLapYear() && day > 28)
            return false;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    return true;
}

tm DateTime::getCurrentDateTime()
{
    time_t now = time(0);
    return *localtime(&now);
}
tm DateTime::getTM() const
{
    tm timeinfo;
    timeinfo.tm_year = year - 1900; // Năm - 1900
    timeinfo.tm_mon = month - 1;    // Tháng (0-11)
    timeinfo.tm_mday = day;         // Ngày trong tháng (1-31)
    timeinfo.tm_hour = hour;        // Giờ (0-23)
    timeinfo.tm_min = minute;       // Phút (0-59)
    timeinfo.tm_sec = 0;            // Giây (0-61)

    return timeinfo;
}

ostream &operator<<(ostream &o, const DateTime &dt)
{
    o << dt.getDate() << "   " << dt.getTime();
    return o;
}

DateTime DateTime::convertToDate(const string &dateStr)
{
    stringstream ss(dateStr);
    int year, month, day;
    char discard;

    ss >> year >> discard >> month >> discard >> day;
    return DateTime(year, month, day, 0, 0);
}

