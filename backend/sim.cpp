#include <iostream>
#include <string>
#include "auth.h"
#include "portfolio.h"
#include "sp500_data.h"

using namespace std;

extern void testPortfolioLink();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "❌ No command provided.\n";
        return 1;
    }

    string command = argv[1];

    if (command == "signup") {
        if (argc < 4) {
            cout << "Usage: signup <username> <password>\n";
            return 1;
        }
        bool success = signUp(argv[2], argv[3]);
        cout << (success ? "✅ Account created\n" : "❌ Signup failed\n");
        return success ? 0 : 1;
    }

    if (command == "login") {
        if (argc < 4) {
            cout << "Usage: login <username> <password>\n";
            return 1;
        }
        bool success = login(argv[2], argv[3]);
        cout << (success ? "✅ Login successful\n" : "❌ Invalid credentials\n");
        return success ? 0 : 1;
    }

    if (command == "buy") {
        if (argc < 6) {
            cout << "Usage: buy <username> <symbol> <shares> <date>\n";
            return 1;
        }
        bool success = buyStock(argv[2], argv[3], stoi(argv[4]), argv[5]);
        cout << (success ? "✅ Stock purchased\n" : "❌ Buy failed\n");
        return success ? 0 : 1;
    }

    if (command == "sell") {
        if (argc < 6) {
            cout << "Usage: sell <username> <symbol> <shares> <date>\n";
            return 1;
        }
        bool success = sellStock(argv[2], argv[3], stoi(argv[4]), argv[5]);
        cout << (success ? "✅ Stock sold\n" : "❌ Sell failed\n");
        return success ? 0 : 1;
    }

    if (command == "portfolio") {
        if (argc < 3) {
            cout << "Usage: portfolio <username>\n";
            return 1;
        }
        return showPortfolio(argv[2]);
    }

    cout << "❌ Unknown command\n";
    return 1;
}
