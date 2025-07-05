#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <map>

struct Holding {
    std::string symbol;
    int shares;
    double avgBuyPrice;
};

class Portfolio {
private:
    std::map<std::string, Holding> holdings;
    double cash;

public:
    Portfolio(double startingCash = 10000.0);

    void buyStock(const std::string& symbol, int shares, double price);
    void sellStock(const std::string& symbol, int shares, double price);
    void viewPortfolio() const;

    double getCashBalance() const;
    bool hasStock(const std::string& symbol) const;
    int getShares(const std::string& symbol) const;

    // ✅ Properly declared as member functions
    bool loadFromFile(const std::string& username);
    bool saveToFile(const std::string& username) const;
};

// ✅ Declared as standalone (non-member) functions
bool buyStock(const std::string& username, const std::string& symbol, int shares, const std::string& date);
bool sellStock(const std::string& username, const std::string& symbol, int shares, const std::string& date);
int showPortfolio(const std::string& username);

#endif
