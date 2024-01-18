#ifndef FLIGHT_H
#define FLIGHT_H

#include "Airplane.h"
#include "DateTime.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class Flight : public Airplane
{
    private:
        string flightID;
        string from;
        string to;
        DateTime departureDateTime;
        int flightStatus;
        int initialTotalSeats;
        double price;
        string airplaneID;
        int availableSeats;

    public:
        static Flight *head;
        Flight *next;

        Flight();
        Flight(const string &ID, const string &from, const string &to, const DateTime &dateTime, int status, int seats,
            double price, const string &airplaneID, Flight *nextFlight, int initialTotalSeats);

        string getFlightID() const;
        int getAvailableSeats() const;
        int getFlightStatus() const;
        double getPrice() const;
        string getFrom() const;
        string getTo() const;
        DateTime getDepartureDateTime() const;
        static Flight *getHead()
        {
            return head;
        }

        Flight *getNext() const
        {
            return next;
        }

        void readDataFromFile(const string &filename);
        void displayFlightDetails() const;
        void addFlight(const string &flightID, const string &from, const string &to, const DateTime &departureDateTime,
                    int flightStatus, int availableSeats, double price, const string &airplaneID, int initialTotalSeats);

        static DateTime convertToDateTime(const string &datetimeStr);
        static Flight *searchFlightByID(string flightID);
        void checkAndUpdateFlightStatus();
        bool bookSeats(int numSeats);
        void saveDataToFile(const string &newFilename);
        bool matchDate(const string &date) const;
        void findFlightsByDate(const string &date);
};

#endif 