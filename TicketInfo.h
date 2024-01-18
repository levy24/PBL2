#pragma once

#include "Flight.h"
#include <string>
#include <iostream>
using namespace std;

class TicketInfo
{
    private:
        string departureCity;
        string destinationCity;
        int isRoundTrip;
        string departureDate;
        string returnDate;
        Flight selectedOutboundFlight;
        Flight selectedReturnFlight;
        string selectedOutboundSeat;
        string selectedReturnSeat;
        int adultAmount;
        int childAmount;
        double price;       

    public:
        TicketInfo();
        ~TicketInfo();

        Flight getSelectedOutboundFlight() const;
        Flight getSelectedReturnFlight() const;
        int getRoundTrip() const;
        string getDepartureCity() const;
        string getDestinationCity() const;
        int getAdultAmount()const;
        int getChildAmount()const;
        //int getPrice() const;

        void displayOptions(int);
        void chooseFlight(int);
        string getLineByFlightID(const string &identifier) const;
        void printSeatMapForFlight(const Flight &flight) const;
        bool isSeatInRange(const string &seat, int seatCount);
        void chooseSeats();
        void chooseSeatsForFlight(const Flight &flight, string *seats);
        void updateAvailableSeats();
        void saveTicketInfoToFile(const string &filename) const;
        void readTicketInfoFromFile(const string &filename);
        bool isValidSeat(const string &seat);
        double calculatePrice();
        void writeSeatToFile();
};