#ifndef MENU_H
#define MENU_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class Menu {
private:
    unordered_map<int, pair<string, double>> menuItems;
    unordered_map<string, double> priceMap;
    
public:
    Menu() {
        initializeMenu();
    }
    
    void initializeMenu() {
        // Counter 1 - Roti & Sabji
        menuItems[1] = {"Roti", 10.0};
        menuItems[2] = {"Butter Roti", 15.0};
        menuItems[3] = {"Naan", 20.0};
        menuItems[4] = {"Garlic Naan", 25.0};
        menuItems[5] = {"Sabji", 50.0};
        menuItems[6] = {"Mix Veg", 60.0};
        
        // Counter 2 - Dal & Rice
        menuItems[7] = {"Dal", 40.0};
        menuItems[8] = {"Dal Makhani", 60.0};
        menuItems[9] = {"Rice", 30.0};
        menuItems[10] = {"Jeera Rice", 40.0};
        menuItems[11] = {"Biryani", 90.0};
        menuItems[12] = {"Pulao", 70.0};
        
        // Counter 3 - Specials
        menuItems[13] = {"Paneer", 80.0};
        menuItems[14] = {"Shahi Paneer", 100.0};
        menuItems[15] = {"Kadai Paneer", 110.0};
        menuItems[16] = {"Raita", 25.0};
        menuItems[17] = {"Salad", 20.0};
        menuItems[18] = {"Papad", 10.0};
        menuItems[19] = {"Sweet", 35.0};
        menuItems[20] = {"Samosa", 15.0};
        menuItems[21] = {"Kachori", 15.0};
        menuItems[22] = {"Chai", 10.0};
        menuItems[23] = {"Coffee", 15.0};
        menuItems[24] = {"Cold Drink", 20.0};
        
        // Also populate priceMap for quick lookup
        for (const auto& item : menuItems) {
            priceMap[item.second.first] = item.second.second;
        }
    }
    
    double getPrice(const string& item) {
        if (priceMap.find(item) != priceMap.end())
            return priceMap[item];
        return 0.0;
    }
    
    string getItemByNumber(int num) {
        if (menuItems.find(num) != menuItems.end()) {
            return menuItems[num].first;
        }
        return "";
    }
    
    void displayMenu() {
        cout << "\n========== TODAY'S MENU ==========\n\n";
        
        cout << "COUNTER 1 - ROTI & SABJI:\n";
        cout << "-------------------------\n";
        for (int i = 1; i <= 6; i++) {
            cout << "  " << setw(2) << i << ". " << setw(15) << left << menuItems[i].first 
                 << " Rs. " << menuItems[i].second << endl;
        }
        
        cout << "\nCOUNTER 2 - DAL & RICE:\n";
        cout << "----------------------\n";
        for (int i = 7; i <= 12; i++) {
            cout << "  " << setw(2) << i << ". " << setw(15) << left << menuItems[i].first 
                 << " Rs. " << menuItems[i].second << endl;
        }
        
        cout << "\nCOUNTER 3 - SPECIALS & BEVERAGES:\n";
        cout << "--------------------------------\n";
        for (int i = 13; i <= 24; i++) {
            cout << "  " << setw(2) << i << ". " << setw(15) << left << menuItems[i].first 
                 << " Rs. " << menuItems[i].second << endl;
        }
        
        cout << "\nTODAY'S SPECIAL COMBOS:\n";
        cout << "  * Combo 1: Roti + Dal + Rice = Rs. 70 (Save Rs. 10)\n";
        cout << "  * Combo 2: Paneer + Naan + Raita = Rs. 120 (Save Rs. 15)\n";
        cout << "  * Combo 3: Biryani + Raita + Cold Drink = Rs. 130 (Save Rs. 20)\n";
    }
};

#endif