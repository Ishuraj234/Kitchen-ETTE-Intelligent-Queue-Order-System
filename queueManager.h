#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include "student.h"
#include "menu.h"
#include "analytics.h"
#include "unique_feature.h"

using namespace std;

// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

class QueueManager {
private:
    queue<Student> counter1;
    queue<Student> counter2;
    queue<Student> counter3;
    priority_queue<Student> vipQueue;
    deque<Student> pendingOrders;
    
    Menu menu;
    Analytics analyticsSystem;
    LoyaltyPointsSystem loyaltySystem;
    
    int totalServed;
    int totalRevenue;
    
public:
    QueueManager() : totalServed(0), totalRevenue(0) {}
    
    void addStudent(const Student& student) {
        if (student.isVIP) {
            cout << GREEN << "[VIP] Student Added to Priority Queue: " << RESET;
            student.display();
            vipQueue.push(student);
            pendingOrders.push_back(student);
            return;
        }
        
        // Smart load balancing
        int s1 = counter1.size();
        int s2 = counter2.size();
        int s3 = counter3.size();
        
        // Calculate estimated wait times
        int wait1 = s1 * 30;
        int wait2 = s2 * 30;
        int wait3 = s3 * 30;
        
        // Choose queue with shortest wait time
        if (wait1 <= wait2 && wait1 <= wait3) {
            counter1.push(student);
            cout << BLUE << "[Counter 1] Added (Wait: ~" << wait1 << "s): " << RESET;
        } else if (wait2 <= wait1 && wait2 <= wait3) {
            counter2.push(student);
            cout << BLUE << "[Counter 2] Added (Wait: ~" << wait2 << "s): " << RESET;
        } else {
            counter3.push(student);
            cout << BLUE << "[Counter 3] Added (Wait: ~" << wait3 << "s): " << RESET;
        }
        
        student.display();
        pendingOrders.push_back(student);
        
        // Calculate estimated total for display
        double estimatedBill = 0;
        for (const auto& item : student.items) {
            estimatedBill += menu.getPrice(item);
        }
        cout << "   " << CYAN << "Est. Bill: Rs. " << estimatedBill << RESET << endl;
    }
    
    Student* getNextStudent() {
        if (!vipQueue.empty()) {
            Student* s = new Student(vipQueue.top());
            vipQueue.pop();
            return s;
        }
        
        // Priority to shorter queues with loyalty members
        vector<pair<int, queue<Student>*>> candidates;
        
        if (!counter1.empty()) {
            int priority = counter1.front().hasLoyaltyCard ? -10 : counter1.size();
            candidates.push_back({priority, &counter1});
        }
        if (!counter2.empty()) {
            int priority = counter2.front().hasLoyaltyCard ? -10 : counter2.size();
            candidates.push_back({priority, &counter2});
        }
        if (!counter3.empty()) {
            int priority = counter3.front().hasLoyaltyCard ? -10 : counter3.size();
            candidates.push_back({priority, &counter3});
        }
        
        if (candidates.empty()) return nullptr;
        
        // Sort by priority
        for (size_t i = 0; i < candidates.size(); i++) {
            for (size_t j = i + 1; j < candidates.size(); j++) {
                if (candidates[i].first > candidates[j].first) {
                    swap(candidates[i], candidates[j]);
                }
            }
        }
        
        Student* s = new Student(candidates[0].second->front());
        candidates[0].second->pop();
        return s;
    }
    
    void serveStudent(Student* student) {
        if (!student) return;
        
        cout << "\n" << GREEN << BOLD << "[NOW SERVING] " << RESET;
        student->display();
        
        auto now = chrono::system_clock::now();
        auto waitingTime = chrono::duration_cast<chrono::seconds>(now - student->arrivalTime).count();
        
        cout << "   [Wait time] " << waitingTime << " seconds\n";
        
        // Calculate bill with itemized breakdown
        double bill = 0;
        cout << "   " << CYAN << "Order Details:" << RESET << endl;
        for (const auto& item : student->items) {
            double price = menu.getPrice(item);
            bill += price;
            cout << "     - " << setw(15) << left << item << " Rs. " << price << endl;
        }
        
        // Apply loyalty discount
        pair<int, double> discount = loyaltySystem.calculateDiscount(student->studentID, bill);
        double finalAmount = discount.second;
        
        cout << "   " << "------------------------\n";
        cout << "   " << "Subtotal: Rs. " << bill << endl;
        
        if (discount.first > 0) {
            cout << "   " << GREEN << "Loyalty Discount: " << discount.first << "%" << RESET << endl;
            cout << "   " << YELLOW << "Final Amount: Rs. " << finalAmount << RESET << endl;
        } else {
            cout << "   " << YELLOW << "Total: Rs. " << bill << RESET << endl;
            cout << "   " << CYAN << "Earn 5 points on this purchase! Get loyalty card for discounts." << RESET << endl;
        }
        
        // Add loyalty points
        int pointsEarned = static_cast<int>(bill / 5);
        loyaltySystem.addPoints(student->studentID, student->name, pointsEarned, bill);
        loyaltySystem.recordOrder(student->studentID, student->items, bill);
        
        // Preparation animation
        cout << "   " << "Preparing order";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            Sleep(300);
        }
        cout << " Done!\n";
        
        // Generate token for collection
        cout << "   " << MAGENTA << "Collection Token: #" << student->tokenID << " - Counter " 
             << (student->isVIP ? "VIP" : (counter1.size() < counter2.size() ? "1" : "2")) << RESET << endl;
        
        // Record analytics
        analyticsSystem.recordOrder(student->items, finalAmount, waitingTime);
        totalServed++;
        totalRevenue += finalAmount;
        
        // Remove from pending
        for (auto it = pendingOrders.begin(); it != pendingOrders.end(); ++it) {
            if (it->tokenID == student->tokenID) {
                pendingOrders.erase(it);
                break;
            }
        }
        
        delete student;
    }
    
    void displayStatus() {
        cout << "\n" << BOLD << "========== CURRENT QUEUE STATUS ==========\n" << RESET;
        
        // Calculate estimated times
        int time1 = counter1.size() * 30;
        int time2 = counter2.size() * 30;
        int time3 = counter3.size() * 30;
        
        cout << "\n";
        cout << "   " << BLUE << "COUNTER 1 (Roti & Sabji)" << RESET << endl;
        cout << "   " << "----------------------------------------\n";
        cout << "   Queue Length: " << counter1.size() << " students\n";
        cout << "   Est. Wait Time: " << time1 << " seconds\n";
        if (!counter1.empty()) {
            cout << "   Next: " << counter1.front().name;
            if (counter1.front().hasLoyaltyCard) cout << " [Loyalty]";
            cout << " (Token #" << counter1.front().tokenID << ")\n";
        }
        
        cout << "\n   " << YELLOW << "COUNTER 2 (Dal & Rice)" << RESET << endl;
        cout << "   " << "----------------------------------------\n";
        cout << "   Queue Length: " << counter2.size() << " students\n";
        cout << "   Est. Wait Time: " << time2 << " seconds\n";
        if (!counter2.empty()) {
            cout << "   Next: " << counter2.front().name;
            if (counter2.front().hasLoyaltyCard) cout << " [Loyalty]";
            cout << " (Token #" << counter2.front().tokenID << ")\n";
        }
        
        cout << "\n   " << MAGENTA << "COUNTER 3 (Specials)" << RESET << endl;
        cout << "   " << "----------------------------------------\n";
        cout << "   Queue Length: " << counter3.size() << " students\n";
        cout << "   Est. Wait Time: " << time3 << " seconds\n";
        if (!counter3.empty()) {
            cout << "   Next: " << counter3.front().name;
            if (counter3.front().hasLoyaltyCard) cout << " [Loyalty]";
            cout << " (Token #" << counter3.front().tokenID << ")\n";
        }
        
        cout << "\n   " << GREEN << "VIP QUEUE" << RESET << endl;
        cout << "   " << "----------------------------------------\n";
        cout << "   Queue Length: " << vipQueue.size() << " students\n";
        if (!vipQueue.empty()) {
            cout << "   Next: " << vipQueue.top().name << " (Priority: " << vipQueue.top().priority << ")\n";
        }
        
        cout << "\n   " << CYAN << "TOTAL STATISTICS" << RESET << endl;
        cout << "   " << "----------------------------------------\n";
        cout << "   Total Pending: " << pendingOrders.size() << " orders\n";
        cout << "   Total Served Today: " << totalServed << " students\n";
        cout << "   Revenue Today: Rs. " << totalRevenue << endl;
        
        // Recommendation based on queue
        if (counter1.size() < counter2.size() && counter1.size() < counter3.size()) {
            cout << "\n   " << GREEN << "Recommendation: Counter 1 has shortest queue!" << RESET << endl;
        } else if (counter2.size() < counter1.size() && counter2.size() < counter3.size()) {
            cout << "\n   " << GREEN << "Recommendation: Counter 2 has shortest queue!" << RESET << endl;
        } else if (counter3.size() < counter1.size() && counter3.size() < counter2.size()) {
            cout << "\n   " << GREEN << "Recommendation: Counter 3 has shortest queue!" << RESET << endl;
        }
    }
    
    bool cancelOrder(int tokenID) {
        for (auto it = pendingOrders.begin(); it != pendingOrders.end(); ++it) {
            if (it->tokenID == tokenID) {
                cout << RED << "[CANCELLED] Token #" << tokenID << " (" << it->name << ")" << RESET << endl;
                pendingOrders.erase(it);
                return true;
            }
        }
        return false;
    }
    
    void displayLoyaltyStats(const string& id, const string& name) {
        loyaltySystem.displayStudentStats(id, name);
    }
    
    void displayTopLoyal() {
        loyaltySystem.displayTopLoyalStudents();
    }
    
    Analytics& getAnalytics() { 
        return analyticsSystem; 
    }
    
    void reorderQueues() {
        cout << YELLOW << "Applying Shortest Job First Algorithm..." << RESET << endl;
        
        // Simple reordering logic
        vector<Student> temp1, temp2, temp3;
        
        while (!counter1.empty()) {
            temp1.push_back(counter1.front());
            counter1.pop();
        }
        while (!counter2.empty()) {
            temp2.push_back(counter2.front());
            counter2.pop();
        }
        while (!counter3.empty()) {
            temp3.push_back(counter3.front());
            counter3.pop();
        }
        
        // Sort by service time (shortest first)
        for (size_t i = 0; i < temp1.size(); i++) {
            for (size_t j = i + 1; j < temp1.size(); j++) {
                if (temp1[i].serviceTime > temp1[j].serviceTime) {
                    swap(temp1[i], temp1[j]);
                }
            }
        }
        for (size_t i = 0; i < temp2.size(); i++) {
            for (size_t j = i + 1; j < temp2.size(); j++) {
                if (temp2[i].serviceTime > temp2[j].serviceTime) {
                    swap(temp2[i], temp2[j]);
                }
            }
        }
        for (size_t i = 0; i < temp3.size(); i++) {
            for (size_t j = i + 1; j < temp3.size(); j++) {
                if (temp3[i].serviceTime > temp3[j].serviceTime) {
                    swap(temp3[i], temp3[j]);
                }
            }
        }
        
        for (const auto& s : temp1) counter1.push(s);
        for (const auto& s : temp2) counter2.push(s);
        for (const auto& s : temp3) counter3.push(s);
        
        cout << GREEN << "Queues reorganized! Average wait time reduced." << RESET << endl;
    }
    
    void runManualSimulation(int numStudents) {
        cout << "\n" << CYAN << "Starting simulation with " << numStudents << " students...\n" << RESET;
        
        // Add students
        for (int i = 0; i < numStudents; i++) {
            string name = "Student" + to_string(i+1);
            string id = "SIM" + to_string(1000 + i);
            vector<string> items;
            
            // Generate varied orders
            int numItems = rand() % 3 + 1;
            for (int j = 0; j < numItems; j++) {
                int itemNum = rand() % 10 + 1;
                string item = "Roti";
                if (itemNum == 1) item = "Roti";
                else if (itemNum == 2) item = "Dal";
                else if (itemNum == 3) item = "Rice";
                else if (itemNum == 4) item = "Paneer";
                else if (itemNum == 5) item = "Sabji";
                else if (itemNum == 6) item = "Raita";
                else if (itemNum == 7) item = "Naan";
                else if (itemNum == 8) item = "Sweet";
                else if (itemNum == 9) item = "Samosa";
                else item = "Chai";
                items.push_back(item);
            }
            
            bool loyalty = (rand() % 4 == 0);
            bool vip = (rand() % 8 == 0);
            
            if (vip) {
                Student s(name, id, items, rand() % 5 + 5, true, loyalty);
                addStudent(s);
            } else {
                Student s(name, id, items, loyalty);
                addStudent(s);
            }
            Sleep(100);
        }
        
        displayStatus();
        
        // Serve students
        cout << "\n" << GREEN << "Serving students...\n" << RESET;
        int served = 0;
        while (served < numStudents) {
            Sleep(500);
            Student* s = getNextStudent();
            if (s) {
                serveStudent(s);
                served++;
                if (served % 3 == 0) {
                    displayStatus();
                }
            } else {
                break;
            }
        }
        
        cout << "\n" << GREEN << BOLD << "Simulation Complete! " << served << " students served.\n" << RESET;
        analyticsSystem.displayAnalytics();
    }
};

#endif