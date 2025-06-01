#include "auth.h"
#include "portfolio.h"
#include "sp500_data.h"

#include <iostream>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::string username, password;
    bool loggedIn = false;
    Portfolio userPortfolio;
    int choice;

    loadMarketData("backend/data/sp500.csv");

    while (!loggedIn) {
        std::cout << "\n=== Welcome to S&P500 Trading Simulator ===\n";
        std::cout << "1. Sign Up\n";
        std::cout << "2. Log In\n";
        std::cout << "3. Exit\n";
        std::cout << "Select option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Choose a username: ";
            std::cin >> username;
            std::cout << "Choose a password: ";
            std::cin >> password;

            if (signUp(username, password)) {
                std::cout << "✅ Account created successfully!\n";
            } else {
                std::cout << "❌ Error creating account.\n";
            }
        } else if (choice == 2) {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;

            if (login(username, password)) {
                std::cout << "✅ Logged in successfully!\n";
                loggedIn = true;
                userPortfolio.loadFromFile(username);  // ✅ Add this
            } else {
                std::cout << "❌ Invalid credentials.\n";
            }
        } else if (choice == 3) {
            std::cout << "Goodbye!\n";
            return 0;
        } else {
            std::cout << "Invalid choice.\n";
            clearInput();
        }
    }

    // Post-login menu
    while (true) {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. View Portfolio\n";
        std::cout << "2. Buy Stock\n";
        std::cout << "3. Sell Stock\n";
        std::cout << "4. View Stock Price\n";
        std::cout << "5. Log Out\n";
        std::cout << "Choose an action: ";
        std::cin >> choice;

        if (choice == 1) {
            userPortfolio.loadFromFile(username);
            userPortfolio.viewPortfolio();

        } else if (choice == 2) {
            std::string sym, date;
            int shares;
            std::cout << "Enter stock symbol (e.g., AAPL): ";
            std::cin >> sym;
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "Enter shares to buy: ";
            std::cin >> shares;

            double price = getClosingPrice(sym, date);
            if (price == -1.0) {
                std::cout << "❌ Invalid symbol or date.\n";
            } else {
                userPortfolio.buyStock(sym, shares, price);
                userPortfolio.saveToFile(username);
            }

        } else if (choice == 3) {
            std::string sym, date;
            int shares;
            std::cout << "Enter stock symbol: ";
            std::cin >> sym;
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "Enter shares to sell: ";
            std::cin >> shares;

            double price = getClosingPrice(sym, date);
            if (price == -1.0) {
                std::cout << "❌ Invalid symbol or date.\n";
            } else {
                userPortfolio.sellStock(sym, shares, price);
                userPortfolio.saveToFile(username);
            }

        } else if (choice == 4) {
            std::string sym, date;
            std::cout << "Enter symbol: ";
            std::cin >> sym;
            std::cout << "Enter date: ";
            std::cin >> date;

            double price = getClosingPrice(sym, date);
            if (price != -1.0) {
                std::cout << "📈 Closing price for " << sym << " on " << date << " is $" << price << "\n";
            } else {
                std::cout << "❌ Could not find data for that day.\n";
            }

        } else if (choice == 5) {
            std::cout << "🚪 Logging out...\n";
            break;
        } else {
            std::cout << "❌ Invalid option.\n";
            clearInput();
        }
    }

    return 0;
}
