#include "CustomerInfo.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <regex>
using namespace std;

CustomerInfo::CustomerInfo(string Name, string Phone, string Email, string YearBirth)
    : Name(Name), Phone(Phone), Email(Email), YearBirth(YearBirth)
{
}
CustomerInfo::~CustomerInfo()
{
    delete[] nameList;
}
void CustomerInfo::setName(string Name)
{
    this->Name = Name;
}
string CustomerInfo::getName()
{
    return Name;
}
void CustomerInfo::setPhone(string Phone)
{
    this->Phone = Phone;
}
string CustomerInfo::getPhone()
{
    return Phone;
}
void CustomerInfo::setYearBirth(string YearBirth)
{
    this->YearBirth = YearBirth;
}
string CustomerInfo::getYearBirth()
{
    return YearBirth;
}
void CustomerInfo::setEmail(string Email)
{
    this->Email = Email;
}
string CustomerInfo::getEmail()
{
    return Email;
}
bool CustomerInfo::checkNameValid(string Name)
{
    for (char const &c : Name)
    {
        if (!isalpha(c) && c != ' ')
        {
            return false;
        }
    }
    return true;
}
bool CustomerInfo::checkPhoneValid(string Phone)
{
    if (Phone.length() != 10)
    {
        return false;
    }
    for (char const &c : Phone)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool CustomerInfo::checkBirthValid(string YearBirth, bool isAdult)
{
    if (isAdult)
    {
        int year = stoi(YearBirth);
        if (year > 2012 || year < 1900)
        {
            cout << "Year birth is unsuitable." << endl;
            cout << "Enter Year of Birth: ";
            return false;
        }

        if (YearBirth.length() != 4)
        {
            return false;
        }
    }

    if (!isAdult)
    {
        if (YearBirth.length() != 4)
        {
            return false;
        }
        int year = stoi(YearBirth);
        if (year < 2013 || year > 2024)
        {
            cout << "Year birth is unsuitable" << endl;
            cout << "Enter Year of Birth: ";
            return false;
        }
    }

    return true;
}

bool CustomerInfo::checkOTP(string OTP)
{
    if (OTP.length() != 6)
    {
        return false;
    }
    for (char const &c : OTP)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

void CustomerInfo::security(string &otp)
{
    string temp;
    char c;
    int count = 0;
    do
    {
        c = _getch();
        if ((c == 13 || c == ' ') && count == 6)
        {
            break;
        }
        if ((c == 8 || c == 127) && count > 0)
        {
            cout << "\b \b";
            temp.pop_back();
            count--;
        }
        else if (isalnum(c))
        {
            temp.push_back(c);
            cout << "*";
            count++;
        }
        if (count == 6)
        {
            otp = temp;
            break;
        }

    } while (true);
}
void CustomerInfo::setOTP()
{
    string OTP;
    cout << "Nhap ma OTP(6 chu so) da duoc gui ve so dien thoai cua ban: ";
    while (true)
    {

        security(OTP);
        if (checkOTP(OTP))
        {
            break;
        }
        cout << "Invalid OTP. Please try again." << endl;
        cout << "Nhap ma OTP(6 chu so) da duoc gui ve so dien thoai cua ban: ";
        OTP.clear();
    }
}
bool CustomerInfo::checkEmailValid(string Email)
{
    regex pattern("^[a-zA-Z0-9][a-zA-Z0-9._]*@[a-zA-Z0-9]+(\\.[a-zA-Z]{2,})+$");
    return regex_match(Email, pattern);
}

struct Customer
{
    string Phone;
    string Name;
    string Email;
    string YearBirth;
};

bool CustomerInfo::checkPhoneExist(const string &phone)
{
    ifstream file("database/Customer.txt");
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        getline(iss, token, ',');
        if (token == phone)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void displayCustomerInfo(const string &phone)
{
    ifstream file("database/Customer.txt");
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        getline(iss, token, ',');
        if (token == phone)
        {
            Customer customer;
            customer.Phone = token;
            getline(iss, customer.Name, ',');

            getline(iss, customer.Email, ',');
            getline(iss, customer.YearBirth, ',');

            cout << "\nThong tin cua ban da duoc luu tru:" << endl;
            cout << "Phone: " << customer.Phone << endl;
            cout << "Name: " << customer.Name << endl;

            cout << "Email: " << customer.Email << endl;
            cout << "YearBirth: " << customer.YearBirth << endl;
            break;
        }
    }
    file.close();
}

void CustomerInfo::inputCustomerInfo(int adult, int child)
{
    int amount = adult + child;
    nameList = new string[amount];
    cin.ignore();
    int i = 0;
    bool isAdult = true;
    while (amount)
    {
        while (adult)
        {
            cout << "Enter Phone: ";
            getline(cin, Phone);
            if (checkPhoneValid(Phone))
            {
                if (checkPhoneExist(Phone))
                {
                    setOTP();
                    ifstream file("database/Customer.txt");
                    string line;
                    while (getline(file, line))
                    {
                        istringstream iss(line);
                        string token;
                        getline(iss, token, ',');
                        if (token == Phone)
                        {
                            getline(iss, nameList[i], ',');
                            file.close();
                            displayCustomerInfo(Phone);
                            i++;
                            break;
                        }
                    }
                    --adult;
                    --amount;

                    continue;
                }
                break;
            }
            cout << "Invalid Phone. Please do again." << endl;
        }
    
        if (!amount) 
            break;
        cout << "Enter Name: ";
        while (true)
        {
            getline(cin, nameList[i]);
            if (checkNameValid(nameList[i]))
            {
                break;
            }
            cout << "Invalid Name. Please enter alphabetic characters only." << endl;
            cout << "Enter Name: ";
        }

        while (adult)
        {
            cout << "Enter Email Address: ";
            getline(cin, Email);
            if (checkEmailValid(Email))
            {
                break;
            }
            cout << "Invalid Email. Please do again." << endl;
        }
        
        cout << "Enter Year of Birth: ";
        while (true)
        {
            getline(cin, YearBirth);
            if(adult == 0){
                isAdult = false;
            }
            if (checkBirthValid(YearBirth, isAdult))
            {
                break;
                cout << "Enter Year of Birth: ";
            }
        }
        
        if (adult)
        {
            setOTP();
            --adult;
        }
        ofstream file("database/Customer.txt", ios::app);
        if (file.is_open())
        {
            if(isAdult)
                file << Phone << "," << nameList[i] << "," << Email << "," << YearBirth << "\n";
            else    
                file <<  nameList[i] << "," << YearBirth << "\n";
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }

        ++i;
        --amount;
        cout << endl;
    }
}