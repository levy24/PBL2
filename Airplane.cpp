#include "Airplane.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
Airplane *Airplane::head1 = nullptr;
Airplane::Airplane()
{}

Airplane::Airplane(const string &id, const string &type, int seats)
    : airplaneID(id), airplaneType(type), seatCount(seats), next(nullptr)
{}

Airplane::~Airplane()
{}

string Airplane::getAirplaneID() const
{
    return airplaneID;
}
string Airplane::getAirplaneType() const
{
    return airplaneType;
}

int Airplane::getSeatCount() const
{
    return seatCount;
}

void Airplane::viewAirplanes() const
{
    Airplane* currentAirplane = head1;
    while (currentAirplane != nullptr)
    {
        cout << "Airplane ID: " << currentAirplane->airplaneID << endl;
        cout << "Airplane Type: " << currentAirplane->airplaneType << endl;
        cout << "Seat Count: " << currentAirplane->seatCount << endl;
        cout << endl;
        currentAirplane = currentAirplane->next;
    }
}
Airplane* Airplane::findAirplaneByID(const string &id)
{
    Airplane* currentAirplane = head1;
    while (currentAirplane != nullptr)
    {
        if(currentAirplane->airplaneID == id)
            return currentAirplane;
        currentAirplane = currentAirplane->next;
    }
    return nullptr;
}
void Airplane::addAirplane(const string &id, const string &type, const int& seats)
{
    Airplane *temp = head1;
    while (temp != nullptr)
    {
        if (temp->getAirplaneID() == id)
        {
            return;
        }
        temp = temp->next;
    }

    Airplane *newAirplane = new Airplane(id, type, seats);
    if (head1 == nullptr)
        head1 = newAirplane;
    else
    {
        Airplane *temp = head1;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newAirplane;
    }
    saveDataToFile("database/Airplane.txt");
}


void Airplane::readDataFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Unable to open file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        string id, type;
        int seats;

        getline(ss, id, ',');
        getline(ss, type, ',');
        ss >> seats;
 
        addAirplane(id, type, seats);
    }
    file.close();
}

void Airplane::saveDataToFile(const string &newFilename) const
{
    ofstream file(newFilename);
    if (!file.is_open())
    {
        cout << "Unable to create or open file " << newFilename << endl;
        return;
    }

    Airplane *currentAirplane = this->head1;
    while (currentAirplane != nullptr)
    {
        file << currentAirplane->airplaneID << ",";
        file << currentAirplane->airplaneType << ",";
        file << currentAirplane->seatCount << endl;
        currentAirplane = currentAirplane->next;
    }
    file.close();
}