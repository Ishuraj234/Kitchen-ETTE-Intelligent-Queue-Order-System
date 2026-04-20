#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

class Analytics {
private:
    map<string, int> itemFrequency;
    double totalRevenue;
    vector<int> waitingTimes;
    int totalStudents;
    int maxWaitingTime;
    
public:
    Analytics() : totalRevenue(0), totalStudents(0), maxWaitingTime(0) {}
    
    void recordOrder(const vector<string>& items, double orderValue, int waitingTime) {
        for (const auto& item : items) {
            itemFrequency[item]++;
        }
        
        totalRevenue += orderValue;
        waitingTimes.push_back(waitingTime);
        if (waitingTime > maxWaitingTime) {
            maxWaitingTime = waitingTime;
        }
        totalStudents++;
    }
    
    void displayAnalytics() {
        cout << "\n========== ANALYTICS REPORT ==========\n";
        
        // Popular items
        cout << "\n📊 Item Frequencies:\n";
        for (const auto& item : itemFrequency) {
            cout << "  " << item.first << ": " << item.second << " orders\n";
        }
        
        // Waiting time stats
        double avgWaiting = 0;
        if (!waitingTimes.empty()) {
            for (int wt : waitingTimes) avgWaiting += wt;
            avgWaiting /= waitingTimes.size();
        }
        
        cout << "\n⏱️  Waiting Times:\n";
        cout << "  Average: " << fixed << setprecision(2) << avgWaiting << " seconds\n";
        cout << "  Maximum: " << maxWaitingTime << " seconds\n";
        cout << "  Total Students: " << totalStudents << endl;
        
        // Revenue
        cout << "\n💰 Revenue:\n";
        cout << "  Total: Rs. " << fixed << setprecision(2) << totalRevenue << endl;
        cout << "  Average per student: Rs. " 
             << (totalStudents > 0 ? totalRevenue / totalStudents : 0) << endl;
        
        cout << "======================================\n";
    }
    
    void reset() {
        itemFrequency.clear();
        totalRevenue = 0;
        waitingTimes.clear();
        totalStudents = 0;
        maxWaitingTime = 0;
    }
};

#endif