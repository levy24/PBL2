#include "TicketInfo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

TicketInfo::TicketInfo()
{
    departureCity = "";
    destinationCity = "";
    isRoundTrip = false;
    departureDate = "";
    returnDate = "";
}

TicketInfo::~TicketInfo()
{
}

Flight TicketInfo::getSelectedOutboundFlight() const
{
    return selectedOutboundFlight;
}

Flight TicketInfo::getSelectedReturnFlight() const
{
    return selectedReturnFlight;
}

int TicketInfo::getRoundTrip() const
{
    if (isRoundTrip)
        return 1;
    else
        return 0;
}

string TicketInfo::getDepartureCity() const
{
    return departureCity;
}
string TicketInfo::getDestinationCity() const
{
    return destinationCity;
}

int TicketInfo::getAdultAmount() const
{
    return adultAmount;
}

int TicketInfo::getChildAmount() const
{
    return childAmount;
}

string toUpperCase(const string &str)
{
    string result;
    for (char c : str)
    {
        result += toupper(static_cast<unsigned char>(c));
    }
    return result;
}
int temp;
void TicketInfo::displayOptions(int choice)
{
    DateTime dt = DateTime(2024, 7, 1, 0, 0);

    cin.ignore(numeric_limits < streamsize>::max(), '\n');
    cout << "Enter Departure City: ";
    getline(cin, departureCity);
    departureCity = toUpperCase(departureCity);

    cout << "Enter Destination City: ";
    getline(cin, destinationCity);
    destinationCity = toUpperCase(destinationCity);

    if (choice == 1)
    {
        cout << "Round Trip? (1 for Yes, 0 for No): ";
        cin >> isRoundTrip;
        temp = isRoundTrip;
    }

    do
    {
        cout << "Enter Departure Date (YYYY-MM-DD): ";
        cin >> departureDate;
        dt = dt.convertToDate(departureDate);

        if (!dt.isValidDateTime())
        {
            cout << "Invalid date format. Please enter the date in YYYY-MM-DD format.\n";
        }
    } while (!dt.isValidDateTime());

    if (isRoundTrip)
    {
        do
        {
            do
            {
                cout << "Enter Return Date (YYYY-MM-DD): ";
                cin >> returnDate;
                dt = dt.convertToDate(returnDate);

                if (!dt.isValidDateTime())
                {
                    cout << "Invalid date format. Please enter the date in YYYY-MM-DD format.\n";
                }
            } while (!dt.isValidDateTime());

            if (returnDate <= departureDate)
            {
                cout << "Return date must be greater than departure date. Please enter again." << endl;
            }

        } while (returnDate <= departureDate);
    }
    if (choice == 1)
    {
        do
        {
            cout << "Enter Amount of adult ticket: ";
            cin >> adultAmount;
            if (adultAmount < 1 || adultAmount > 10)
                cout << "Invalid input. Please enter a number between 1 and 10." << endl;
        } while (adultAmount < 1 || adultAmount > 10);

        do
        {
            cout << "Enter Amount of child ticket: ";
            cin >> childAmount;
            if (childAmount < 0 || childAmount > 10)
                cout << "Invalid input. Please enter a number between 1 and 10." << endl;
        } while (childAmount < 0 || childAmount > 10);
    }
}

void TicketInfo::chooseFlight(int choice)
{
    Flight *currentFlight = Flight::getHead();
    bool isRoundTripSelection = true;
    bool isValidSelection = false;

    while (!isValidSelection)
    {
        // Trước khi hiển thị danh sách chuyến bay, cập nhật trạng thái của chúng
        while (currentFlight != nullptr)
        {
            currentFlight->checkAndUpdateFlightStatus();
            currentFlight = currentFlight->getNext();
        }

        bool hasAvailableFlights = false;
        string flightType = isRoundTripSelection ? "outbound" : "return";

        cout << endl;
        cout << "Available " << flightType << " flights:" << endl;

        cout << "  Flight ID  |"
                  << "        From       |"
                  << "        To        |"
                  << "  Departure Date and Time  |"
                  << "   Price   |"
                  << "   Airplane ID   |"
                  << " Total Seats |"
                  << "   Available Seats  " << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------------------------------"
                  << endl;

        // Hiển thị danh sách chuyến bay sau khi cập nhật trạng thái
        currentFlight = Flight::getHead();
        while (currentFlight != nullptr)
        {
            if ((isRoundTripSelection &&
                 currentFlight->getFrom() == departureCity &&
                 currentFlight->getTo() == destinationCity &&
                 currentFlight->matchDate(departureDate)) ||
                (!isRoundTripSelection &&
                 currentFlight->getFrom() == destinationCity &&
                 currentFlight->getTo() == departureCity &&
                 currentFlight->matchDate(returnDate)))
            {
                if (currentFlight->getFlightStatus() == 1) // Chỉ hiển thị chuyến bay có thể đặt vé
                {
                    hasAvailableFlights = true;
                    currentFlight->displayFlightDetails();
                }
            }

            currentFlight = currentFlight->getNext();
        }

        if (!hasAvailableFlights)
        {
            cout << "No available " << flightType << " flights match your criteria. Please try again." << endl;
            int userChoice;
            cout << "1. Exit" << endl;
            cout << "2. Choose " << flightType << " flight again" << endl;
            cin >> userChoice;

            if (userChoice == 1)
            {
                return;
            }
            else if (userChoice == 2)
            {
                currentFlight = Flight::getHead();
                continue;
            }
            else
            {
                cout << "Invalid choice. Exiting program." << endl;
                return;
            }
        }
        if (choice == 1)
        {
            cout << "Enter the Flight ID of the chosen " << flightType << " flight: ";
            string chosenFlightID;
            cin >> chosenFlightID;

            currentFlight = Flight::getHead();
            while (currentFlight != nullptr)
            {
                if (currentFlight->getFlightID() == chosenFlightID &&
                    currentFlight->getFlightStatus() == 1 &&
                    ((isRoundTripSelection &&
                      currentFlight->getFrom() == departureCity &&
                      currentFlight->getTo() == destinationCity &&
                      currentFlight->matchDate(departureDate)) ||
                     (!isRoundTripSelection &&
                      currentFlight->getFrom() == destinationCity &&
                      currentFlight->getTo() == departureCity &&
                      currentFlight->matchDate(returnDate))))
                {

                    if (isRoundTrip && isRoundTripSelection)
                    {
                        isRoundTripSelection = false; // Chuyển sang chọn chuyến bay về
                        selectedOutboundFlight = *currentFlight;
                        if ((adultAmount + childAmount) > currentFlight->getAvailableSeats())
                        {
                            cout << "Not enough seats available. Exiting program." << endl;
                            return;
                        }
                    }
                    else if (isRoundTripSelection && isRoundTrip == 0)
                    {
                        isValidSelection = true; // Lựa chọn hợp lệ, thoát khỏi vòng lặp
                        selectedOutboundFlight = *currentFlight;
                        if ((adultAmount + childAmount) > currentFlight->getAvailableSeats())
                        {
                            cout << "Not enough seats available. Exiting program." << endl;
                            return;
                        }
                    }

                    else
                    {
                        isValidSelection = true; // Lựa chọn hợp lệ, thoát khỏi vòng lặp
                        selectedReturnFlight = *currentFlight;
                        if ((adultAmount + childAmount) > currentFlight->getAvailableSeats())
                        {
                            cout << "Not enough seats available. Exiting program." << endl;
                            return;
                        }
                    }

                    break;
                }

                currentFlight = currentFlight->getNext();
            }
        }
        else
            return;
    }
}

string *OutboundSeats = nullptr;
string *ReturnSeats = nullptr;

void TicketInfo::chooseSeats()
{
    OutboundSeats = new string[childAmount + adultAmount];

    chooseSeatsForFlight(getSelectedOutboundFlight(), OutboundSeats);

    if (isRoundTrip)
    {
        ReturnSeats = new string[childAmount + adultAmount];
        chooseSeatsForFlight(getSelectedReturnFlight(), ReturnSeats);
    }
}

bool TicketInfo::isSeatInRange(const string &seat, int seatCount)
{
    if (seat.length() < 2)
    {
        return false;
    }

    char seatLetter = seat.back();
    int seatRow = stoi(seat.substr(0, seat.length() - 1));

    return seatRow >= 1 && seatRow <= seatCount / 6 &&
           seatLetter >= 'A' && seatLetter <= 'F';
}

void TicketInfo::chooseSeatsForFlight(const Flight &flight, string *seats)
{
    if (flight.getFlightStatus() != 1)
    {
        cout << "Cannot book seats for the flight. The flight is not available for booking." << endl;
        return;
    }

    printSeatMapForFlight(flight);

    ifstream inFile("database/flight_seats.txt");

    string *lines = nullptr;
    string line;
    int lineCount = 0;

    while (getline(inFile, line))
    {
        string *temp = new string[lineCount + 1];
        for (int i = 0; i < lineCount; ++i)
        {
            temp[i] = lines[i];
        }
        temp[lineCount] = line;

        delete[] lines;
        lines = temp;

        ++lineCount;
    }

    inFile.close();

    bool validSeat = false;
    for (int i = 0; i < childAmount + adultAmount; i++)
    {
        do
        {
            cout << "Choose a seat for the flight " << flight.getFlightID() << " : ";
            cin >> seats[i];
            seats[i] = toUpperCase(seats[i]);

            if (!isSeatInRange(seats[i], flight.getSeatCount()))
            {
                cerr << "Invalid seat. Please choose a seat within the range.\n";
                --i;
                validSeat = false;
                break;
            }

            for (int j = 0; j < i; ++j)
            {
                if (seats[j] == seats[i])
                {
                    cerr << "Seat already chosen. Choose another seat." << endl;
                    --i;
                    validSeat = false;
                    break;
                }
            }

            for (int j = 0; j < lineCount; ++j)
            {
                if (lines[j].find(flight.getFlightID()) != string::npos)
                {
                    if (lines[j].find(" " + seats[i] + " ") != string::npos)
                    {
                        cerr << "Seat already booked. Choose another seat." << endl;
                        validSeat = false;
                        break;
                    }
                    else
                    {
                        validSeat = true;
                        break;
                    }
                }
            }
        } while (!validSeat);
    }
    delete[] lines;
}

string TicketInfo::getLineByFlightID(const string &identifier) const
{
    ifstream inFile("database/flight_seats.txt");

    string line;

    while (getline(inFile, line))
    {
        if (line.find(identifier) != string::npos)
        {
            inFile.close();
            return line;
        }
    }

    inFile.close();

    cerr << "Identifier not found in the file." << endl;
    return "";
}

void TicketInfo::printSeatMapForFlight(const Flight &flight) const
{
    string flightID;
    flightID = flight.getFlightID();
    string line = getLineByFlightID(flightID);

    if (!line.empty())
    {
        size_t pos = line.find(flightID);

        if (pos != string::npos)
        {
            string addedData = line.substr(pos + flightID.length());

            int totalSeats = flight.getSeatCount();

            string *tokens = nullptr;
            int tokenCount = 0;

            istringstream ss(addedData);
            string token;

            while (ss >> token)
            {
                ++tokenCount;
            }

            ss.clear();
            ss.seekg(0);

            tokens = new string[tokenCount];

            for (int i = 0; i < tokenCount; ++i)
            {
                ss >> tokens[i];
            }

            for (int row = 1; row <= totalSeats / 6; ++row)
            {
                for (char seat = 'A'; seat <= 'F'; ++seat)
                {
                    string currentSeat = to_string(row) + seat;

                    bool isBooked = false;
                    for (int j = 0; j < tokenCount; ++j)
                    {
                        if (currentSeat == tokens[j])
                        {
                            isBooked = true;
                            break;
                        }
                    }

                    if (isBooked)
                    {
                        cout << "\x1B[32m" << currentSeat << " "; // Mã màu xanh lá cây
                    }
                    else if (row == 1 || row == 2 || row == 3)
                    {
                        cout << "\x1B[33m" << currentSeat << " "; // Mã màu vàng
                    }
                    else
                    {
                        cout << "\x1B[0m" << currentSeat << " "; // Mã màu trắng
                    }
                }
                cout << endl;
            }

            cout << endl;
            cout << "\x1B[0m" << endl;

            delete[] tokens;
        }
    }
    cout << "The yellow business class seat for adults costs 175% (children costs 150%) of the original price" << endl;
}
void TicketInfo::updateAvailableSeats()
{
    Flight *currentFlight = Flight::getHead();

    while (currentFlight != nullptr)
    {
        if (currentFlight->getFlightID() == selectedOutboundFlight.getFlightID())
        {

            if (currentFlight->bookSeats(childAmount + adultAmount))
            {
                cout << "Seat booked successfully for the outbound flight." << std::endl;
            }
            else
            {
                cout << "Seat booking failed for the outbound flight. The seat may be already taken." << std::endl;
            }

        }

        if (temp)
        {
            if (currentFlight->getFlightID() == selectedReturnFlight.getFlightID())
            {
                if (currentFlight->bookSeats(childAmount + adultAmount))
                {
                    cout << "Seat booked successfully for the return flight." << endl;
                }
                else
                {
                    cout << "Seat booking failed for the return flight. The seat may be already taken." << endl;
                }
                // break;
            }
        }
        currentFlight = currentFlight->getNext();
    }
}
void TicketInfo::saveTicketInfoToFile(const string &filename) const
{
    ofstream outFile(filename);

    if (!outFile)
    {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    for (int i = 0; i < childAmount + adultAmount; i++)
    {
        outFile << "Departure City: " << departureCity << endl;
        outFile << "Destination City: " << destinationCity << endl;

        if (isRoundTrip)
        {
            outFile << "Round Trip" << endl;
        }
        outFile << "Departure Date: " << departureDate << endl;

        if (isRoundTrip)
        {
            outFile << "Return Date: " << returnDate << endl;
        }

        outFile << "Selected Outbound Flight: " << selectedOutboundFlight.getFlightID() << endl;
        outFile << "Selected Outbound Seat: " << OutboundSeats[i] << endl;

        if (selectedOutboundFlight.getFlightID() != "")
        {
            DateTime departureTime = selectedOutboundFlight.getDepartureDateTime();
            outFile << "Departure Time: " << departureTime.getTime() << endl;
        }

        if (isRoundTrip)
        {
            outFile << "Selected Return Flight: " << selectedReturnFlight.getFlightID() << endl;
            outFile << "Selected Return Seat: " << ReturnSeats[i] << endl;
            if (isRoundTrip && selectedReturnFlight.getFlightID() != "")
            {
                DateTime returnTime = selectedReturnFlight.getDepartureDateTime();
                outFile << "Return Time: " << returnTime.getTime() << endl;
            }
        }
        outFile << endl;
    }

    outFile.close();
}

void TicketInfo::readTicketInfoFromFile(const string &filename)
{
    ifstream inFile(filename);

    if (!inFile)
    {
        cerr << "Error: Unable to open file for reading." << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        size_t pos = line.find(":");
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        if (key == "Departure City")
        {
            departureCity = value;
        }
        else if (key == "Destination City")
        {
            destinationCity = value;
        }
        else if (key == "Round Trip")
        {
            isRoundTrip = (value == "Yes");
        }
        else if (key == "Departure Date")
        {
            departureDate = value;
        }
        else if (key == "Return Date")
        {
            returnDate = value;
        }
        else if (key == "Selected Outbound Flight")
        {
            Flight *flight;
            Flight *chosenOutboundFlight = flight->searchFlightByID(value);
            if (chosenOutboundFlight != nullptr)
            {
                selectedOutboundFlight = *chosenOutboundFlight;
            }
        }
        else if (key == "Selected Outbound Seat")
        {
            selectedOutboundSeat = value;
        }
        else if (key == "Selected Return Flight")
        {
            Flight *flight;
            Flight *chosenReturnFlight = flight->searchFlightByID(value);
            if (chosenReturnFlight != nullptr)
            {
                selectedReturnFlight = *chosenReturnFlight;
            }
        }
        else if (key == "Selected Return Seat")
        {
            selectedReturnSeat = value;
        }
    }

    inFile.close();
}

bool TicketInfo::isValidSeat(const string &seat)
{
    if (seat[0] < '4' && !isdigit(seat[1]))
        return true;

    return false;
}
double TicketInfo::calculatePrice()
{
    price = 0.0;
    if (temp == 1)
    {
        for (int i = 0; i < childAmount + adultAmount; i++)
        {
            if (i < adultAmount)
            {
                if (isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice() * 1.75;
                else if (!isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice();

                if (isValidSeat(ReturnSeats[i]))
                    price = price + selectedReturnFlight.getPrice() * 1.75;
                else if (!isValidSeat(ReturnSeats[i]))
                    price = price + selectedReturnFlight.getPrice();
            }

            else
            {
                if (isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice() * 1.5;
                else if (!isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice() * 0.7;

                if (isValidSeat(ReturnSeats[i]))
                    price = price + selectedReturnFlight.getPrice() * 1.5;
                else if (!isValidSeat(ReturnSeats[i]))
                    price = price + selectedReturnFlight.getPrice() * 0.7;
            }
        }
        price = price * 0.95;
    }

    else
    {
        for (int i = 0; i < childAmount + adultAmount; i++)
        {
            if (i < adultAmount)
            {
                if (isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice() * 1.75;
                else
                    price = price + selectedOutboundFlight.getPrice();
            }

            else
            {
                if (isValidSeat(OutboundSeats[i]))
                    price = price + selectedOutboundFlight.getPrice() * 1.5;
                else
                    price = price + selectedOutboundFlight.getPrice() * 0.7;
            }
        }
    }
    return price;
}

void TicketInfo::writeSeatToFile()
{
    ifstream inFile("database/flight_seats.txt");

    string *lines = nullptr;
    string line;
    int lineCount = 0;

    while (getline(inFile, line))
    {
        string *temp = new string[lineCount + 1];
        for (int i = 0; i < lineCount; ++i)
        {
            temp[i] = lines[i];
        }
        temp[lineCount] = line;

        delete[] lines;
        lines = temp;

        ++lineCount;
    }

    inFile.close();
    for (int i = 0; i < lineCount; ++i)
    {
        if (lines[i].find(getSelectedOutboundFlight().getFlightID()) != string::npos)
        {
            for (int j = 0; j < childAmount + adultAmount; ++j)
                lines[i] += " " + OutboundSeats[j];
        }
    }
    if (temp)
    {
        for (int i = 0; i < lineCount; ++i)
        {
            if (lines[i].find(getSelectedReturnFlight().getFlightID()) != string::npos)
            {
                for (int j = 0; j < childAmount + adultAmount; ++j)
                    lines[i] += " " + ReturnSeats[j];
            }
        }
    }

    ofstream outFile("database/flight_seats.txt");

    for (int i = 0; i < lineCount; ++i)
    {
        outFile << lines[i] << endl;
    }

    outFile.close();

    delete[] lines;
}
