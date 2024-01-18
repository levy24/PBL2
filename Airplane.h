#pragma once
#include<string>
#include <iostream>
using namespace std;
class Airplane
{
    private:
        string airplaneID;
        string airplaneType;

    protected:
        int seatCount;
        
    public:
        static Airplane* head1;
        Airplane *next;
        Airplane();
        Airplane(const string& id, const string& type, int seats);
        virtual ~Airplane();

        string getAirplaneID() const;
        string getAirplaneType() const;
        int getSeatCount() const;

        void viewAirplanes() const;
        Airplane* findAirplaneByID(const string &id);
        void addAirplane(const string &id, const string &type, const int& seats);
        virtual void readDataFromFile(const string &filename);
        virtual void saveDataToFile(const string& filename)const;
};