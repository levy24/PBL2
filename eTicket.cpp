#include "eTicket.h"
#include <regex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <conio.h>
using namespace std;

eTicket::eTicket(const CustomerInfo &customer, const TicketInfo &cticket) : CustomerInfo(customer), TicketInfo(cticket)
{
}

void eTicket::Display()
{
    ifstream eTicketIn("database/eTicket.txt");
    if (!eTicketIn)
    {
        cerr << "Unable to open eTicket file." << endl;
        return;
    }

    string line;
    string ticketInfo;

    while (getline(eTicketIn, line))
    {
        if (!line.empty())
        {
            ticketInfo += line + "\n";
        }
        else
        {
            if (!ticketInfo.empty())
            {
                displayTicket(ticketInfo);
                ticketInfo.clear();
            }
        }
    }

    if (!ticketInfo.empty())
    {
        displayTicket(ticketInfo);
    }

    eTicketIn.close();
}

void eTicket::displayTicket(const string &ticketInfo)
{
    istringstream ticketStream(ticketInfo);
    string line;
    string seat;
    string flightID;
    // Đọc và lưu trữ thông tin chuyến bay và ghế đặt
    while (getline(ticketStream, line))
    {
        if (line.find("Selected Outbound Flight:") != string::npos)
        {
            flightID = line.substr(line.find(":") + 2);
        }
        if (line.find("Selected Outbound Seat:") != string::npos)
        {
            seat = line.substr(line.find(":") + 2);
            seat.erase(remove(seat.begin(), seat.end(), ' '), seat.end());
        }
    }

    TicketID = "2024" + flightID + "VN0AF" + seat;
    cout << "|---------------------------------|" << endl;
    cout << "|" << setw(33 - TicketID.length()) << left << "Ticket ID: " << TicketID << "|" << endl;
    cout << "|---------------------------------|" << endl;

    ticketStream.clear();
    ticketStream.seekg(0, ios::beg);
    while (getline(ticketStream, line))
    {
        if (!line.empty())
        {
            cout << "|" << setw(33) << left << line << "|" << endl;
        }
    }

    cout << "|---------------------------------|" << endl;
    cout << endl;
}

void eTicket::TicketToFile(int amount)
{
    ifstream ticketInfoIn("database/TicketInfo.txt");
    ofstream eTicketOut("database/eTicket.txt");

    if (!ticketInfoIn || !eTicketOut)
    {
        cerr << "Unable to open files for reading or writing." << endl;
        return;
    }

    for (int i = 0; i < amount; i++)
    {
        eTicketOut << nameList[i] << endl;

        string line;

        while (getline(ticketInfoIn, line))
        {
            eTicketOut << line << endl;

            if (line.empty())
            {
                eTicketOut.seekp(-2, ios::cur);
                eTicketOut << paystatus << endl;
                break;
            }
        }
        eTicketOut << endl;
    }
    ticketInfoIn.close();
    eTicketOut.close();
}


bool eTicket::checkAccValid(string account)
{

    regex pattern("[0-9]{12}");
    return regex_match(account, pattern);
}

int eTicket::Payment()
{
    double price = calculatePrice();
    cout << "\nSo tien can phai thanh toan la: " << fixed << setprecision(3)<< price << " VND" << endl;
    cout << "Hay chon phuong thuc thanh toan." << endl;

    int pick;
    do
    {
        cout << "1. Thanh toan qua ngan hang.\n2. Thanh toan sau.\n3. Huy" << endl;
        cin >> pick;
        if (pick == 1)
        {
            int bank;
            do
            {
                cout << "Chon ngan hang cua ban: " << endl;
                cout << "1. Agribank" << endl;
                cout << "2. BIDV" << endl;
                cout << "3. MB bank" << endl;
                cout << "4. Techcombank" << endl;
                cout << "5. Sacombank" << endl;
                cout << "6. VPbank" << endl;
                cout << "7. Vietcombank" << endl;
                cout << "8. Vietinbank" << endl;
                cout << "9. VIB" << endl;
                cin >> bank;
                if (bank < 1 || bank > 9)
                {
                    cout << "Lua chon khong hop le. Hay nhap lai." << endl;
                }
                cout << "Nhap so the: ";
            } while (bank < 1 || bank > 9);
            string sotaikhoan;
            cin.ignore();
            while (true)
            {
                getline(cin, sotaikhoan);
                if (checkAccValid(sotaikhoan))
                {
                    break;
                }
                cout << "So the khong hop le. Hay nhap lai." << endl;
                cout << "Nhap so the: ";
            }
            cout << "Nhap ten chu the: ";
            string name;
            cin.ignore();
            while (true)
            {
                getline(cin, name);
                if (checkNameValid(name))
                {
                    break;
                }
                cout << "Invalid Name. Please enter alphabetic characters only." << endl;
                cout << "Nhap ten chu the: ";
            }

            int confirm;
            cout << "\n1. Xac nhan thanh toan\n2. Huy" << endl;
            do
            {
                cin >> confirm;
                if (confirm == 1)
                {
                    cout << "Thanh toan thanh cong. Cam on ban da chon VN airlines." << endl;
                    paystatus = "Paid";
                    return 1;
                    break;
                }
                else if (confirm == 2)
                {
                    return 2;
                }
                else
                {
                    cout << "Lua chon khong hop le. Hay nhap lai." << endl;
                }
            } while (confirm != 1 && confirm != 2);
        }
        else if (pick == 2)
        {
            cout << "Xac nhan thanh cong. Cam on ban da chon VN airlines." << endl;
            paystatus = "Pay later";
            return 1;
        }
        else if(pick == 3){
            return 3;
        }
        else
        {
            cout << "Lua chon khong hop le. Hay nhap lai." << endl;
        }
    } while (pick != 1 && pick != 2 && pick != 3);
    return 0;
}

void eTicket::ticketList(int amount)
{
    ifstream ticketInfoIn("database/TicketInfo.txt");
    ofstream ticketList("database/TicketList.txt", ios::app);

    if (!ticketInfoIn || !ticketList)
    {
        cerr << "Unable to open files for reading or writing." << endl;
        return;
    }

    ticketList.seekp(0, ios::end); 

    for (int i = 0; i < amount; i++)
    {
        string line;
        string ticketID;
        string ticketInfo;
        string seat;
        bool writeNewTicket = true; 

        while (getline(ticketInfoIn, line))
        {
            if (line.empty())
            {
                break; 
            }

            ticketInfo += line + "\n";

            if (line.find("Selected Outbound Flight:") != string::npos)
            {
                string flightID = line.substr(line.find(":") + 2);
                ticketID = "2024" + flightID;
            }

            if (line.find("Selected Outbound Seat:") != string::npos)
            {
                seat = line.substr(line.find(":") + 2);
                seat.erase(remove(seat.begin(), seat.end(), ' '), seat.end());
            }
        }

        if (!ticketID.empty() && !seat.empty() && writeNewTicket) {
            ticketList << ticketID + "VN0AF" + seat << '\n';
            ticketList << nameList[i] << endl;
            ticketList << ticketInfo;
            ticketList << paystatus << endl;
            ticketList << endl;
        }

        ticketID.clear();
        ticketInfo.clear();
        seat.clear();
        writeNewTicket = true;
    }
    ticketInfoIn.close();
    ticketList.close();
}

void eTicket::FindTicket(const string &inputTicketID)
{
    ifstream ticketListIn("database/TicketList.txt");
    if (!ticketListIn)
    {
        cerr << "Unable to open TicketList file." << endl;
        return;
    }

    string line;
    string ticketInfo;
    string ticketID;

    bool found = false;

    while (getline(ticketListIn, line))
    {
        if (line.empty())
        {
            if (!ticketInfo.empty() && ticketID == inputTicketID)
            {
                displayTicket(ticketInfo);
                found = true;
                break;
            }
            ticketInfo.clear();
            continue;
        }

        if (line.substr(0, inputTicketID.length()) == inputTicketID)
        {
            ticketID = line;
        }

        ticketInfo += line + "\n";
    }

    if (!found)
    {
        cout << "Ticket with ID " << inputTicketID << " not found." << endl;
    }

    ticketListIn.close();
    cout << "Press enter to back to menu";
    return;
}
