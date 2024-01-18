#pragma once
#include "CustomerInfo.h"
#include "TicketInfo.h"
#include "Flight.h"
#include <iostream>
using namespace std;
class eTicket : public CustomerInfo, public TicketInfo{
    private:
        string TicketID;
        
    public:
        eTicket(const CustomerInfo& customer, const TicketInfo& cticket);
        void TicketToFile(int);
        int Payment();
        void Display();
        bool checkAccValid(string);
        void displayTicket(const string&);
        void ticketList(int);
        void FindTicket(const string&);
        string paystatus;
};



