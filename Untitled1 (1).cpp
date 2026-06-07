#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;


struct Room {
    int roomNumber;
    string roomType;
    bool isOccupied;
    double pricePerNight;
};


struct BookingHistory {
    string customerName;
    int roomNumber;
    string roomType;
    double pricePerNight;
    int daysBooked;
    double totalPrice;
};


void readDataFromFile(const char* filename, Room rooms[], int& count);
void displayRooms(const Room rooms[], int count);
void writeDataToFile(const char* filename, const Room rooms[], int count);
void bookRoom(Room rooms[], int count, int roomNumber, const string& customerName, int daysBooked);
void checkOutRoom(Room rooms[], int count, int roomNumber);
void logBookingHistory(const BookingHistory& history, bool isCheckIn);
void openBookingHistoryFile();

int main() {
    const int MAX_ROOMS = 100;
    Room rooms[MAX_ROOMS];
    int roomCount = 0;
    
    cout << "HOTEL MANAGEMENT SYSTEM" << endl;
    
    readDataFromFile("rooms.txt", rooms, roomCount);
    displayRooms(rooms, roomCount);

    int choice, roomNumber, daysBooked;
    string customerName;
    do {
        cout << endl;
        cout << "===MENU===" << endl;
        cout << "1. Book Room\n2. Check Out Room\n3. Display Rooms\n4. Booking History\n5. Save & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                cout << "Enter customer name: ";
                cin >> customerName;
                cout << "Enter room number to book: ";
                cin >> roomNumber;
                cout << "Enter number of days booked: ";
                cin >> daysBooked;
                bookRoom(rooms, roomCount, roomNumber, customerName, daysBooked);
                break;
            case 2:
                cout << "Enter room number to check out: ";
                cin >> roomNumber;
                checkOutRoom(rooms, roomCount, roomNumber);
                break;
            case 3:
                displayRooms(rooms, roomCount);
                break;
            case 4:
                openBookingHistoryFile();
                break;
            case 5:
                writeDataToFile("rooms.txt", rooms, roomCount);
                cout << "Data saved to rooms.txt. Exiting program.\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 5);

    return 0;
}

void readDataFromFile(const char* filename, Room rooms[], int& count) {
    ifstream file(filename);
    if (file.is_open()) {
        while (file >> rooms[count].roomNumber >> rooms[count].roomType >> rooms[count].isOccupied >> rooms[count].pricePerNight) {
            count++;
        }
        file.close();
    } else {
        cerr << "Unable to open file\n";
    }
}

void displayRooms(const Room rooms[], int count) {
    cout << "===================================ROOM LIST==================================" << endl;
    for (int i = 0; i < count; i++) {
        cout << "Room Number: " << rooms[i].roomNumber 
             << ", Room Type: " << rooms[i].roomType 
             << ", Is Occupied: " << (rooms[i].isOccupied ? "Yes" : "No") 
             << ", Price per Night: " << rooms[i].pricePerNight << "\n";
    }
    cout << "===============================================================================" << endl;
}

void writeDataToFile(const char* filename, const Room rooms[], int count) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < count; i++) {
            file << rooms[i].roomNumber << " "
                 << rooms[i].roomType << " "
                 << rooms[i].isOccupied << " "
                 << rooms[i].pricePerNight << "\n";
        }
        file.close();
    } else {
        cerr << "Unable to open file\n";
    }
}

void bookRoom(Room rooms[], int count, int roomNumber, const string& customerName, int daysBooked) {
    for (int i = 0; i < count; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            if (!rooms[i].isOccupied) {
                rooms[i].isOccupied = true;
                double totalPrice = rooms[i].pricePerNight * daysBooked;
                BookingHistory history = {customerName, roomNumber, rooms[i].roomType, rooms[i].pricePerNight, daysBooked, totalPrice};
                logBookingHistory(history, true);
                cout << "Room " << roomNumber << " booked successfully.\n";
                
              
                ofstream clearReceiptFile("receipt.txt");
                if (clearReceiptFile.is_open()) {
                    clearReceiptFile.close();
                } else {
                    cerr << "Unable to clear receipt file\n";
                }
            } else {
                cout << "Room " << roomNumber << " is already occupied.\n";
            }
            return;
        }
    }
    cout << "Room " << roomNumber << " not found.\n";
}

void checkOutRoom(Room rooms[], int count, int roomNumber) {
    for (int i = 0; i < count; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            if (rooms[i].isOccupied) {
                rooms[i].isOccupied = false;
                BookingHistory history = {"", roomNumber, rooms[i].roomType, rooms[i].pricePerNight, 0, 0};
                logBookingHistory(history, false);
                cout << "Room " << roomNumber << " checked out successfully.\n";
            } else {
                cout << "Room " << roomNumber << " is already vacant.\n";
            }
            return;
        }
    }
    cout << "Room " << roomNumber << " not found.\n";
}

void logBookingHistory(const BookingHistory& history, bool isCheckIn) {
    ofstream historyFile("booking_history.txt", ios::app);
    if (historyFile.is_open()) {
        historyFile << "====================\n";
        historyFile << (isCheckIn ? "Check In" : "Check Out") << "\n";
        if (isCheckIn) {
            historyFile << "Customer Name: " << history.customerName << "\n";
        }
        historyFile << "Room Number: " << history.roomNumber << "\n";
        historyFile << "Room Type: " << history.roomType << "\n";
        if (isCheckIn) {
            historyFile << "Price per Night: " << history.pricePerNight << "\n";
            historyFile << "Days Booked: " << history.daysBooked << "\n";
            historyFile << "Total Price: " << history.totalPrice << "\n";
        }
        historyFile << "====================\n";
        historyFile.close();
        
        if (isCheckIn) {
            
            ofstream receiptFile("receipt.txt", ios::app);
            if (receiptFile.is_open()) {
                receiptFile << "\n===== RECEIPT =====\n";
                receiptFile << "Customer Name: " << history.customerName << "\n";
                receiptFile << "Room Number: " << history.roomNumber << "\n";
                receiptFile << "Room Type: " << history.roomType << "\n";
                receiptFile << "Price per Night: " << history.pricePerNight << "\n";
                receiptFile << "Days Booked: " << history.daysBooked << "\n";
                receiptFile << "Total Price: " << history.totalPrice << "\n";
                receiptFile << "====================\n";
                receiptFile.close();
                
              
                system("notepad receipt.txt"); 
            } else {
                cerr << "Unable to open receipt file\n";
            }
        }
    } else {
        cerr << "Unable to open booking history file\n";
    }
}

void openBookingHistoryFile() {
    system("notepad booking_history.txt");
}
