#include <iostream>
#include "CustomerInfo.h"
#include "eTicket.h"
#include "TicketInfo.h"

using namespace std;

int main()
{
    TicketInfo ticketInfo;
    Flight flights;
    string Name, Phone, CCCD, Email, YearBirth;

    CustomerInfo customer(Name, Phone, Email, YearBirth);

    flights.readDataFromFile("database/Flight.txt");

    int choice;

    do
    {
        cout << "Menu:" << endl;
        cout << "1. Book a ticket" << endl;
        cout << "2. Search for flight information" << endl;
        cout << "3. Find your ticket" << endl;
        cout << "0. Exit" << endl;
        cout << "Select: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            ticketInfo.displayOptions(choice);
            ticketInfo.chooseFlight(choice);

            if (ticketInfo.getSelectedOutboundFlight().getFlightStatus() == 1)
            {
                ticketInfo.chooseSeats();
                ticketInfo.saveTicketInfoToFile("database/TicketInfo.txt");
                ticketInfo.readTicketInfoFromFile("database/TicketInfo.txt");

                int amountOfTicket = ticketInfo.getAdultAmount() + ticketInfo.getChildAmount();

                customer.inputCustomerInfo(ticketInfo.getAdultAmount(), ticketInfo.getChildAmount());

                eTicket ticket(customer, ticketInfo);
                
                int paymentConfirmed = ticket.Payment();
                if (paymentConfirmed == 1) 
                {
                    ticket.TicketToFile(amountOfTicket);
                    ticketInfo.updateAvailableSeats();
                    ticketInfo.writeSeatToFile();
                    ticket.Display();
                    ticket.ticketList(amountOfTicket);
                } else if(paymentConfirmed == 2){
                    break;
                } else {
                    break;
                }
            break;
            }
            else
            {
                cout << "Selected flight is not available for booking." << endl;
            }
            break;

        case 2:
            ticketInfo.displayOptions(choice);
            ticketInfo.chooseFlight(choice);
            cout << "Press any key to return to the main menu...";
            cin.ignore();
            cin.get();
            break;
        case 3:
        {
            string ticketID;
            cout << "Enter your ticket ID: ";
            cin >> ticketID;
            eTicket eticket(customer, ticketInfo);
            eticket.FindTicket(ticketID);
            cin.ignore();
            cin.get();
            break;
        }
        case 0:
            cout << "Exiting the program." << endl;
            break;

        default:
            cout << "Invalid choice. Please choose again." << endl;
            break;
        }

    } while (choice != 0);

    return 0;
}
