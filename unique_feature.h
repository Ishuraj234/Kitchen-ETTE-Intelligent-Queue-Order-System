#ifndef UNIQUE_FEATURE_H
#define UNIQUE_FEATURE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

class LoyaltyPointsSystem {
private:
    unordered_map<string, int> studentPoints;
    unordered_map<string, int> studentVisits;
    unordered_map<string, vector<string>> studentOrderHistory;
    
    struct DiscountSlab {
        int minPoints;
        int discountPercent;
        string description;
    };
    
    vector<DiscountSlab> discountSlabs;
    
public:
    LoyaltyPointsSystem() {
        DiscountSlab s1 = {100, 5, "Bronze - 5% off"};
        DiscountSlab s2 = {250, 10, "Silver - 10% off"};
        DiscountSlab s3 = {500, 15, "Gold - 15% off"};
        DiscountSlab s4 = {1000, 20, "Platinum - 20% off"};
        
        discountSlabs.push_back(s1);
        discountSlabs.push_back(s2);
        discountSlabs.push_back(s3);
        discountSlabs.push_back(s4);
    }
    
    void addPoints(const string& studentID, const string& studentName, int points, double billAmount) {
        studentPoints[studentID] += points;
        studentVisits[studentID]++;
        
        cout << "\n⭐ LOYALTY EARNED:\n";
        cout << "  " << studentName << " earned " << points << " points!\n";
        cout << "  Total points: " << studentPoints[studentID] << "\n";
    }
    
    pair<int, double> calculateDiscount(const string& studentID, double billAmount) {
        int discountPercent = 0;
        
        if (studentPoints.find(studentID) != studentPoints.end()) {
            int points = studentPoints[studentID];
            
            for (size_t i = 0; i < discountSlabs.size(); i++) {
                if (points >= discountSlabs[i].minPoints) {
                    discountPercent = discountSlabs[i].discountPercent;
                }
            }
        }
        
        double discountAmount = (billAmount * discountPercent) / 100;
        double finalAmount = billAmount - discountAmount;
        
        return make_pair(discountPercent, finalAmount);
    }
    
    void recordOrder(const string& studentID, const vector<string>& items, double billAmount) {
        string order = "Order: ";
        for (size_t i = 0; i < items.size(); i++) {
            order += items[i] + " ";
        }
        
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
        
        order = string(buffer) + " - " + order + " | Rs." + to_string((int)billAmount);
        studentOrderHistory[studentID].push_back(order);
    }
    
    void displayStudentStats(const string& studentID, const string& studentName) {
        cout << "\n========== LOYALTY STATS ==========\n";
        cout << "Student: " << studentName << " (ID: " << studentID << ")\n";
        cout << "Visits: " << studentVisits[studentID] << "\n";
        cout << "Points: " << studentPoints[studentID] << "\n";
        
        // Show current level
        int points = studentPoints[studentID];
        string level = "Regular";
        for (size_t i = 0; i < discountSlabs.size(); i++) {
            if (points >= discountSlabs[i].minPoints) {
                level = discountSlabs[i].description;
            }
        }
        cout << "Level: " << level << "\n";
        
        // Show recent orders
        if (studentOrderHistory.find(studentID) != studentOrderHistory.end()) {
            cout << "\nRecent Orders:\n";
            const vector<string>& history = studentOrderHistory[studentID];
            int start = (history.size() > 3) ? history.size() - 3 : 0;
            for (size_t i = start; i < history.size(); i++) {
                cout << "  " << history[i] << "\n";
            }
        }
        cout << "===================================\n";
    }
    
    void displayTopLoyalStudents(int count = 3) {
        cout << "\n🏆 TOP LOYAL STUDENTS:\n";
        
        vector<pair<string, int>> pointsVector;
        for (const auto& pair : studentPoints) {
            pointsVector.push_back(pair);
        }
        
        // Sort by points
        for (size_t i = 0; i < pointsVector.size(); i++) {
            for (size_t j = i + 1; j < pointsVector.size(); j++) {
                if (pointsVector[i].second < pointsVector[j].second) {
                    swap(pointsVector[i], pointsVector[j]);
                }
            }
        }
        
        for (int i = 0; i < min(count, (int)pointsVector.size()); i++) {
            cout << "  " << (i+1) << ". ID: " << pointsVector[i].first 
                 << " - " << pointsVector[i].second << " points\n";
        }
    }
};

#endif