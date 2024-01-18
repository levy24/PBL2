#include "Flight.h"
#include <iostream>
using namespace std;

Flight *Flight::head = nullptr;

Flight::Flight()
    : flightID(""), from(""), to(""), departureDateTime(05, 11, 2023, 8, 00), flightStatus(0), initialTotalSeats(0), price(0.0),
      airplaneID(""), next(nullptr), availableSeats(0)
{}

Flight::Flight(const string &ID, const string &from, const string &to, const DateTime &dateTime, int status, int initialTotalSeats,
               double price, const string &airplaneID, Flight *nextFlight, int seats)
    : Airplane(airplaneID, "TypeOfAirplane", initialTotalSeats),
      flightID(ID), from(from), to(to), departureDateTime(dateTime), flightStatus(status), initialTotalSeats(initialTotalSeats), price(price),
      airplaneID(airplaneID), next(nextFlight), availableSeats(seats)
{}

string Flight::getFlightID() const
{
    return flightID;
}

int Flight::getAvailableSeats() const
{
    return availableSeats;
}

int Flight::getFlightStatus() const
{
    return flightStatus;
}

double Flight::getPrice() const
{
    return price;
}
string Flight::getFrom() const
{
    return this->from;
}

string Flight::getTo() const
{
    return this->to;
}

DateTime Flight::getDepartureDateTime() const
{
    return this->departureDateTime;
}

void Flight::readDataFromFile(const string &filename)
{
    Airplane::readDataFromFile("database/Airplane.txt");
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
        string id, from, to, date, time, airplaneID;
        int initialTotalSeats, seats, price;

        getline(ss, id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, date, ',');
        getline(ss, time, ',');
        ss >> initialTotalSeats;
        ss.ignore();
        ss >> price;
        ss.ignore();
        getline(ss, airplaneID, ',');
        ss >> seats;

        string datetime = date + " " + time;
        departureDateTime = convertToDateTime(datetime);

        // Set the initial available seats for the flight
        addFlight(id, from, to, departureDateTime, 1, initialTotalSeats, price, airplaneID, seats);
    }

    file.close();
}

void Flight::displayFlightDetails() const
{
    cout << "    " << flightID << "     "
              << "      " << from << "     "
              << "      " << to << "      "
              << "      " << departureDateTime << "     "
              << "    " << price << "    "
              << "      " << airplaneID << "     "
              << "      " << initialTotalSeats << "          "
              << "     " << availableSeats << "    " << endl;
}

void Flight::addFlight(const string &flightID, const string &from, const string &to, const DateTime &departureDateTime,
                       int flightStatus, int initialTotalSeats, double price, const string &airplaneID, int availableSeats)
{
    ifstream file("database/Airplane.txt");
    if (!file.is_open())
    {
        cout << "Unable to open file: database/Airplane.txt" << endl;
        return;
    }

    string line;
    bool airplaneIDExists = false;

    // Kiểm tra xem airplaneID có tồn tại trong danh sách máy bay không
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, type;
        int seats;

        getline(ss, id, ',');
        getline(ss, type, ',');
        ss >> seats;

        if (id == airplaneID)
        {
            airplaneIDExists = true;
            break;
        }
    }

    file.close();

    if (!airplaneIDExists)
    {
        cout << "Invalid airplane ID. Flight not added." << endl;
        return;
    }

    else if (!departureDateTime.isValidDateTime())
    {
        cout << "Invalid date/time. Flight not added." << endl;
        return;
    }

    else if (airplaneIDExists && departureDateTime.isValidDateTime())
    {

        Flight *newFlight = new Flight(flightID, from, to, departureDateTime, flightStatus, initialTotalSeats, price, airplaneID, nullptr, availableSeats);

        if (head == nullptr)
            head = newFlight;
        else
        {
            Flight *temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newFlight;
        }
    }
}

DateTime Flight::convertToDateTime(const string &datetimeStr)
{
    stringstream ss(datetimeStr);
    int year, month, day, hour, minute;
    char discard;
    ss >> year >> discard >> month >> discard >> day >> hour >> discard >> minute;
    return DateTime(year, month, day, hour, minute);
}

Flight *Flight::searchFlightByID(string flightID)
{
    Flight *currentFlight = Flight::head;
    while (currentFlight != nullptr)
    {
        if (currentFlight->getFlightID() == flightID)
        {
            return currentFlight;
        }
        currentFlight = currentFlight->next;
    }
    return nullptr;
}
void Flight::checkAndUpdateFlightStatus()
{
    tm currentDateTime = DateTime::getCurrentDateTime();

    // Lấy thời gian khởi hành của chuyến bay
    tm departureTime = departureDateTime.getTM(); // Sử dụng phương thức getTM() trong DateTime

    time_t currentTimestamp = mktime(&currentDateTime);
    time_t departureTimestamp = mktime(&departureTime);

    if (currentTimestamp > departureTimestamp)
    {
        if (availableSeats == this->getSeatCount())
            flightStatus = 0; // Chuyến bay đã bị hủy do đã qua giờ bay mà không có hành khách đặt vé
        else
            flightStatus = 3; // Chuyến bay đã khởi hành
    }
    else
    {
        if (availableSeats == 0)
            flightStatus = 2; // Hết vé
        else
            flightStatus = 1; // Còn vé
    }
}

bool Flight::bookSeats(int numSeats)
{
    if ((availableSeats -= numSeats) >= 0)
    {
        checkAndUpdateFlightStatus();
        saveDataToFile("database/Flight.txt");
        return true;
    }
    else
    {
        cout << "Khong du ghe ngoi." << endl;
        return false;
    }
}

void Flight::saveDataToFile(const string &newFilename)
{
    ofstream file(newFilename);
    if (!file.is_open())
    {
        cout << "Unable to create or open file " << newFilename << endl;
        return;
    }

    Flight *currentFlight = this->head;
    while (currentFlight != nullptr)
    {
        file << currentFlight->flightID << ",";
        file << currentFlight->from << ",";
        file << currentFlight->to << ",";
        file << currentFlight->departureDateTime.getDate() << ",";
        file << currentFlight->departureDateTime.getTime() << ",";
        file << currentFlight->initialTotalSeats << ",";
        file << currentFlight->price << ",";
        file << currentFlight->airplaneID << ",";
        file << currentFlight->availableSeats << endl;
        currentFlight = currentFlight->next;
    }

    file.close();
}

bool Flight::matchDate(const string &date) const
{
    string flightDate = departureDateTime.getDate();
    return (flightDate == date);
}

void Flight::findFlightsByDate(const string &date)
{
    const Flight *currentFlight = head;
    while (currentFlight != nullptr)
    {
        if (currentFlight->matchDate(date))
        {
            currentFlight->displayFlightDetails();
        }
        currentFlight = currentFlight->next;
    }
}
