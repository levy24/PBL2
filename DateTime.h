#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include<sstream>
#include <iostream>
using namespace std;
class DateTime
{
    private:
        int day;
        int month;
        int year;
        int hour;
        int minute;

    public:
        DateTime();
        DateTime(int, int, int, int, int);
        int getDay() const;
        int getMonth() const;
        int getYear() const;
        int getHour() const;
        int getMinute() const;
        string getDate() const;
        string getTime() const;

        bool isLapYear() const;
        bool isValidDateTime() const;
        static tm getCurrentDateTime();
        tm getTM() const;
        friend ostream &operator<<(ostream &, const DateTime &);
        static DateTime convertToDate(const string &datetimeStr);
};
