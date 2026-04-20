#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include "student.h"
#include "menu.h"
#include "queueManager.h"
#include "analytics.h"
#include "unique_feature.h"

using namespace std;

// Color codes for Windows console
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void clearScreen() {
    system("cls");
}

void printHeader() {
    cout << CYAN << BOLD;
    cout << "\n==================================================\n";
    cout << "               Kitchen ETTE        \n";
    cout << "         LPU - Student Canteen            \n";
    cout << "==================================================\n";
    cout << RESET;
}

void printLoadingAnimation(string text, int seconds) {
    cout << text;
    for (int i = 0; i < seconds; i++) {
        cout << ".";
        Sleep(500);
    }
    cout << " Done!\n";
}

void printSuccess(string message) {
    cout << GREEN << "[SUCCESS] " << message << RESET << endl;
}

void printError(string message) {
    cout << RED << "[ERROR] " << message << RESET << endl;
}

void printInfo(string message) {
    cout << BLUE << "[INFO] " << message << RESET << endl;
}

void printWarning(string message) {
    cout << YELLOW << "[WARNING] " << message << RESET << endl;
}

string getCurrentTime() {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%I:%M:%S %p", timeinfo);
    return string(buffer);
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}

void displayWelcomeMessage() {
    clearScreen();
    printHeader();
    cout << "\n" << getCurrentTime() << "\n";
    cout << GREEN << "\n* Welcome! Today's Special: Paneer Butter Masala @ Rs. 90\n" << RESET;
    cout << "* Get Loyalty Card and Save up to 20% on every order!\n";
    waitForEnter();
}

string generateName() {
    string first[] = {"Raj", "Priya", "Amit", "Neha", "Vikram", "Pooja", "Sanjay", "Divya", 
                      "Rahul", "Anjali", "Arjun", "Kavya", "Rohan", "Isha", "Ravi", "Sneha"};
    string last[] = {"Sharma", "Verma", "Patel", "Singh", "Kumar", "Gupta", "Joshi", "Reddy",
                     "Mehta", "Malhotra", "Choudhary", "Mishra", "Trivedi", "Desai", "Nair"};
    
    return first[rand() % 16] + " " + last[rand() % 15];
}

string generateID() {
    int year = 2020 + (rand() % 5);
    return to_string(year) + "CS" + to_string(rand() % 100 + 10);
}

vector<string> generateOrder() {
    string items[] = {"Roti", "Butter Roti", "Dal", "Rice", "Paneer", 
                      "Sabji", "Raita", "Salad", "Papad", "Sweet", 
                      "Naan", "Biryani", "Chai", "Samosa"};
    vector<string> order;
    int n = rand() % 4 + 1;
    
    for (int i = 0; i < n; i++) {
        int idx = rand() % 14;
        bool found = false;
        for (const auto& item : order) {
            if (item == items[idx]) {
                found = true;
                break;
            }
        }
        if (!found) {
            order.push_back(items[idx]);
        }
    }
    
    if (order.empty()) {
        order.push_back("Roti");
    }
    
    return order;
}

void displayMainMenu() {
    clearScreen();
    printHeader();
    cout << "\n" << BOLD << "MAIN MENU" << RESET << "\n";
    cout << "-----------------------------------------------\n\n";
    
    cout << GREEN << " 1. " << RESET << "+ Add Normal Student\n";
    cout << YELLOW << " 2. " << RESET << "* Add VIP/Faculty\n";
    cout << BLUE << " 3. " << RESET << "$ Add Student with Loyalty Card\n";
    cout << CYAN << " 4. " << RESET << "Q View Queue Status\n";
    cout << MAGENTA << " 5. " << RESET << "S Serve Next Student\n";
    cout << " 6. " << RESET << "R Run Auto Simulation\n";
    cout << RED << " 7. " << RESET << "X Cancel Order\n";
    cout << " 8. " << RESET << "M View Food Menu\n";
    cout << " 9. " << RESET << "L View Loyalty Stats\n";
    cout << "10. " << RESET << "T View Top Loyal Students\n";
    cout << "11. " << RESET << "A View Analytics\n";
    cout << "12. " << RESET << "O Reorder Queues (SJF)\n";
    cout << "13. " << RESET << "I System Info\n";
    cout << RED << " 0. " << RESET << "E Exit\n";
    
    cout << "\n-----------------------------------------------\n";
    cout << "Current Time: " << getCurrentTime() << "\n";
    cout << "Your choice: ";
}

void displaySystemInfo() {
    clearScreen();
    printHeader();
    cout << "\n" << BOLD << "SYSTEM INFORMATION" << RESET << "\n";
    cout << "-----------------------------------------------\n\n";
    
    cout << "Canteen: IIT Patna Central Canteen\n";
    cout << "Chef: Master Chef Sharma\n";
    cout << "Hours: 8:00 AM - 10:00 PM\n";
    cout << "Date: ";
    
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    cout << put_time(timeinfo, "%d-%m-%Y") << "\n\n";
    
    cout << "System Features:\n";
    cout << "  * Multi-Counter Queue Management\n";
    cout << "  * VIP Priority Handling\n";
    cout << "  * Loyalty Points Program\n";
    cout << "  * Real-time Analytics\n";
    cout << "  * Token-based Ordering\n";
    cout << "  * Load Balancing\n\n";
    
    cout << "Current Offers:\n";
    cout << "  * Buy any 3 items, get 1 Papad FREE\n";
    cout << "  * Loyalty members get 5% extra discount on Tuesdays\n";
    cout << "  * Combo Offer: Roti + Dal + Rice @ Rs. 70 only\n\n";
    
    waitForEnter();
}

int main() {
    srand(time(nullptr));
    
    QueueManager qm;
    Menu menu;
    int choice;
    
    displayWelcomeMessage();
    
    while (true) {
        displayMainMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            printError("Please enter a valid number!");
            Sleep(1000);
            continue;
        }
        
        if (choice == 0) {
            clearScreen();
            printHeader();
            cout << "\n" << YELLOW << "Generating Final Report...\n" << RESET;
            printLoadingAnimation("Processing", 2);
            
            cout << "\n" << BOLD << "FINAL ANALYTICS REPORT" << RESET << "\n";
            cout << "-----------------------------------------------\n\n";
            qm.getAnalytics().displayAnalytics();
            
            cout << "\n" << GREEN << BOLD << "Thank you for using Smart Canteen System!\n" << RESET;
            cout << "Have a great day!\n";
            break;
        }
        
        switch(choice) {
            case 1: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "ADD NORMAL STUDENT" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                string name, id;
                cout << "Enter student name (or type 'auto' for random): ";
                cin.ignore();
                getline(cin, name);
                
                if (name == "auto" || name == "AUTO" || name == "Auto") {
                    name = generateName();
                    id = generateID();
                    printSuccess("Generated: " + name + " (ID: " + id + ")");
                    // No extra pause here - continue to item selection
                } else {
                    cout << "Enter student ID: ";
                    getline(cin, id);
                }
                
                menu.displayMenu();
                vector<string> items;
                int itemChoice;
                
                cout << "\n" << CYAN << "Select items (enter numbers, 0 to finish):\n" << RESET;
                while (true) {
                    cout << "Item number: ";
                    cin >> itemChoice;
                    if (itemChoice == 0) break;
                    
                    string item = menu.getItemByNumber(itemChoice);
                    if (item != "") {
                        items.push_back(item);
                        cout << GREEN << "  + Added " << item << RESET << endl;
                    } else {
                        printError("Invalid item number! Please enter 1-24");
                    }
                }
                
                if (items.empty()) {
                    printWarning("No items selected! Generating random order...");
                    items = generateOrder();
                    cout << "Order: ";
                    for (const auto& item : items) cout << item << " ";
                    cout << endl;
                }
                
                Student s(name, id, items, false);
                qm.addStudent(s);
                waitForEnter();
                break;
            }
            
            case 2: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "ADD VIP/FACULTY" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                string name, id;
                cout << "Enter faculty name (or type 'auto' for random): ";
                cin.ignore();
                getline(cin, name);
                
                if (name == "auto" || name == "AUTO" || name == "Auto") {
                    name = "Dr. " + generateName();
                    id = "FAC" + to_string(rand() % 1000 + 100);
                    printSuccess("Generated: " + name + " (ID: " + id + ")");
                    // No extra pause - continue to priority input
                } else {
                    cout << "Enter faculty ID: ";
                    getline(cin, id);
                }
                
                int priority;
                cout << "Enter priority (1-10, higher = more priority): ";
                cin >> priority;
                
                menu.displayMenu();
                vector<string> items;
                int itemChoice;
                
                cout << "\n" << CYAN << "Select items (enter numbers, 0 to finish):\n" << RESET;
                while (true) {
                    cout << "Item number: ";
                    cin >> itemChoice;
                    if (itemChoice == 0) break;
                    
                    string item = menu.getItemByNumber(itemChoice);
                    if (item != "") {
                        items.push_back(item);
                        cout << GREEN << "  + Added " << item << RESET << endl;
                    } else {
                        printError("Invalid item number!");
                    }
                }
                
                if (items.empty()) {
                    printWarning("No items selected! Generating random order...");
                    items = generateOrder();
                    cout << "Order: ";
                    for (const auto& item : items) cout << item << " ";
                    cout << endl;
                }
                
                Student s(name, id, items, priority, true, false);
                qm.addStudent(s);
                waitForEnter();
                break;
            }
            
            case 3: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "ADD LOYALTY STUDENT" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                cout << GREEN << "Special Offer: Get 5% extra discount on first order!\n\n" << RESET;
                
                string name, id;
                cout << "Enter student name (or type 'auto' for random): ";
                cin.ignore();
                getline(cin, name);
                
                if (name == "auto" || name == "AUTO" || name == "Auto") {
                    name = generateName();
                    id = generateID();
                    printSuccess("Generated: " + name + " (ID: " + id + ")");
                    // No extra pause - continue to item selection
                } else {
                    cout << "Enter student ID: ";
                    getline(cin, id);
                }
                
                menu.displayMenu();
                vector<string> items;
                int itemChoice;
                
                cout << "\n" << CYAN << "Select items (enter numbers, 0 to finish):\n" << RESET;
                while (true) {
                    cout << "Item number: ";
                    cin >> itemChoice;
                    if (itemChoice == 0) break;
                    
                    string item = menu.getItemByNumber(itemChoice);
                    if (item != "") {
                        items.push_back(item);
                        cout << GREEN << "  + Added " << item << RESET << endl;
                    } else {
                        printError("Invalid item number!");
                    }
                }
                
                if (items.empty()) {
                    printWarning("No items selected! Generating random order...");
                    items = generateOrder();
                    cout << "Order: ";
                    for (const auto& item : items) cout << item << " ";
                    cout << endl;
                }
                
                Student s(name, id, items, true);
                qm.addStudent(s);
                waitForEnter();
                break;
            }
            
            case 4: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "CURRENT QUEUE STATUS" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                qm.displayStatus();
                waitForEnter();
                break;
            }
            
            case 5: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "SERVING NEXT STUDENT" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                printLoadingAnimation("Getting next student", 1);
                Student* s = qm.getNextStudent();
                if (s) {
                    qm.serveStudent(s);
                } else {
                    printError("No students in queue!");
                }
                waitForEnter();
                break;
            }
            
            case 6: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "AUTO SIMULATION MODE" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                int num;
                cout << "Enter number of students to simulate (5-20): ";
                cin >> num;
                
                if (num < 5) num = 5;
                if (num > 20) num = 20;
                
                cout << "\n" << CYAN << "Starting simulation with " << num << " students...\n" << RESET;
                qm.runManualSimulation(num);
                
                waitForEnter();
                break;
            }
            
            case 7: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "CANCEL ORDER" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                int token;
                cout << "Enter token number to cancel: ";
                cin >> token;
                
                if (qm.cancelOrder(token)) {
                    printSuccess("Order cancelled successfully!");
                } else {
                    printError("Token not found!");
                }
                waitForEnter();
                break;
            }
            
            case 8: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "TODAY'S MENU" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                menu.displayMenu();
                waitForEnter();
                break;
            }
            
            case 9: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "LOYALTY STATS" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                
                string id, name;
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                
                qm.displayLoyaltyStats(id, name);
                waitForEnter();
                break;
            }
            
            case 10: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "TOP LOYAL STUDENTS" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                qm.displayTopLoyal();
                waitForEnter();
                break;
            }
            
            case 11: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "SYSTEM ANALYTICS" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                qm.getAnalytics().displayAnalytics();
                waitForEnter();
                break;
            }
            
            case 12: {
                clearScreen();
                printHeader();
                cout << "\n" << BOLD << "REORDERING QUEUES" << RESET << "\n";
                cout << "-----------------------------------------------\n\n";
                printLoadingAnimation("Applying Shortest Job First algorithm", 2);
                qm.reorderQueues();
                printSuccess("Queues reorganized successfully!");
                waitForEnter();
                break;
            }
            
            case 13: {
                displaySystemInfo();
                break;
            }
            
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
    
    return 0;
}