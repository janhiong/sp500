#include "portfolio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "sp500_data.h"


Portfolio::Portfolio(double startingCash) : cash(startingCash) {}

void Portfolio::buyStock(const std::string& symbol, int shares, double price) {
    double totalCost = shares * price;
    if (cash >= totalCost) {
        cash -= totalCost;
        if (holdings.count(symbol)) {
            Holding& h = holdings[symbol];
            h.avgBuyPrice = ((h.shares * h.avgBuyPrice) + totalCost) / (h.shares + shares);
            h.shares += shares;
        } else {
            holdings[symbol] = {symbol, shares, price};
        }
    }
}

void Portfolio::sellStock(const std::string& symbol, int shares, double price) {
    if (holdings.count(symbol) && holdings[symbol].shares >= shares) {
        cash += shares * price;
        holdings[symbol].shares -= shares;
        if (holdings[symbol].shares == 0) {
            holdings.erase(symbol);
        }
    }
}

void Portfolio::viewPortfolio() const {
    std::cout << "CASH," << std::fixed << std::setprecision(2) << cash << "\n";
    for (const auto& [sym, h] : holdings) {
        std::cout << sym << "," << h.shares << "," << std::fixed << std::setprecision(2) << h.avgBuyPrice << "\n";
    }
}

double Portfolio::getCashBalance() const {
    return cash;
}

bool Portfolio::hasStock(const std::string& symbol) const {
    return holdings.count(symbol) > 0;
}

int Portfolio::getShares(const std::string& symbol) const {
    if (holdings.count(symbol)) return holdings.at(symbol).shares;
    return 0;
}

bool buyStock(const std::string& username, const std::string& symbol, int shares, const std::string& date) {
    Portfolio p;
    p.loadFromFile(username);                    
    double price = getClosingPrice(symbol, date);
    if (price < 0) {
        std::cout << "Invalid symbol or date.\n";
        return false;
    }
    p.buyStock(symbol, shares, price);

    p.saveToFile(username);                      
    std::cout << "Bought " << shares << " shares of " << symbol << "\n";
    return true;
}

bool sellStock(const std::string& username, const std::string& symbol, int shares, const std::string& date) {
    Portfolio p;
    p.loadFromFile(username);                    
    p.sellStock(symbol, shares, 100.0);          
    p.saveToFile(username);                      
    std::cout << "Sold " << shares << " shares of " << symbol << "\n";
    return true;
}

int showPortfolio(const std::string& username) {
    Portfolio p;
    p.loadFromFile(username);   
    p.viewPortfolio();
    return 0;
}

void testPortfolioLink() {
    std::cout << "portfolio.cpp compiled correctly\n";
}

bool Portfolio::loadFromFile(const std::string& username) {
    std::ifstream file("../backend/data/" + username + ".portfolio");
    if (!file) return false;

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string sym;
        int shares;
        double price;

        if (line.rfind("CASH", 0) == 0) {
            std::getline(iss, sym, ',');  
            iss >> cash;                  
        } else {
            std::getline(iss, sym, ',');
            iss >> shares;
            iss.ignore(1); 
            iss >> price;
            holdings[sym] = { sym, shares, price };
        }
    }
    return true;
}


bool Portfolio::saveToFile(const std::string& username) const {
    std::ofstream file("../backend/data/" + username + ".portfolio");
    if (!file) return false;

    file << "CASH," << cash << "\n";
    for (const auto& [sym, h] : holdings) {
        file << sym << "," << h.shares << "," << h.avgBuyPrice << "\n";
    }
    return true;
}


