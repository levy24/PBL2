#pragma once

#include <iostream>
#include <string>

using namespace std;

class CustomerInfo
{
    protected:
        string Name;
        string Phone;
        string Email;
        string YearBirth;
        string* nameList = nullptr;
        
    public:
        CustomerInfo(string, string, string, string);
        ~CustomerInfo();
        void setName(string);
        string getName();
        void setPhone(string);
        string getPhone();
        void setYearBirth(string);
        string getYearBirth();
        void setEmail(string);
        string getEmail();
        void inputCustomerInfo(int, int);
        bool checkNameValid(string);
        bool checkPhoneValid(string);
        bool checkBirthValid(string, bool);
        bool checkEmailValid(string);
        bool checkPhoneExist(const string&);
        void setOTP();
        bool checkOTP(string OTP);
        void security(string&);
};


