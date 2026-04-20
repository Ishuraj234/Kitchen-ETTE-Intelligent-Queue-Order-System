#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

class Student {
private:
    static int nextTokenID;
    
public:
    int tokenID;
    string name;
    string studentID;
    vector<string> items;
    chrono::system_clock::time_point arrivalTime;
    int serviceTime;
    int priority;
    bool isVIP;
    bool hasLoyaltyCard;
    
    // Constructor for normal student
    Student(string n, string id, vector<string> i, bool loyalty = false) {
        tokenID = nextTokenID++;
        name = n;
        studentID = id;
        items = i;
        arrivalTime = chrono::system_clock::now();
        serviceTime = items.size() * 30;
        priority = 0;
        isVIP = false;
        hasLoyaltyCard = loyalty;
    }
    
    // Constructor for VIP
    Student(string n, string id, vector<string> i, int p, bool vip, bool loyalty = false) {
        tokenID = nextTokenID++;
        name = n;
        studentID = id;
        items = i;
        arrivalTime = chrono::system_clock::now();
        serviceTime = items.size() * 30;
        priority = p;
        isVIP = vip;
        hasLoyaltyCard = loyalty;
    }
    
    bool operator<(const Student& other) const {
        if (priority != other.priority)
            return priority < other.priority;
        return tokenID > other.tokenID;
    }
    
    void display() const {
        cout << "Token #" << tokenID << " | " << name;
        if (isVIP) cout << " [VIP]";
        if (hasLoyaltyCard) cout << " [LOYALTY]";
        cout << " | Items: ";
        for (const auto& item : items) {
            cout << item << " ";
        }
        cout << "| Time: " << serviceTime << "s\n";
    }
};

int Student::nextTokenID = 1000;

#endif