#include "sp500_data.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

std::vector<StockEntry> allData;
std::map<std::string, std::map<std::string, StockEntry>> dataMap; // symbol -> date -> entry

void loadMarketData(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    getline(file, line);  // Skip header

    while (getline(file, line)) {
        std::istringstream ss(line);
        std::string date, symbol, openStr, closeStr, volumeStr;

        getline(ss, date, ',');
        getline(ss, symbol, ',');
        getline(ss, openStr, ',');
        getline(ss, closeStr, ',');
        getline(ss, volumeStr, ',');

        StockEntry entry;
        entry.date = date;
        entry.symbol = symbol;
        entry.open = std::stod(openStr);
        entry.close = std::stod(closeStr);
        entry.volume = std::stol(volumeStr);

        allData.push_back(entry);
        dataMap[symbol][date] = entry;
    }

    file.close();
    std::cout << "Loaded " << allData.size() << " entries from " << filepath << std::endl;
}

double getClosingPrice(const std::string& symbol, const std::string& date) {
    if (dataMap.count(symbol) && dataMap[symbol].count(date)) {
        return dataMap[symbol][date].close;
    }
    return -1.0; // not found
}

std::vector<std::string> getAvailableSymbols() {
    std::set<std::string> unique;
    for (const auto& entry : allData) {
        unique.insert(entry.symbol);
    }
    return std::vector<std::string>(unique.begin(), unique.end());
}
