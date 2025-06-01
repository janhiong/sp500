#ifndef SP500_DATA_H
#define SP500_DATA_H

#include <string>
#include <vector>
#include <map>

// A single stock entry from CSV
struct StockEntry {
    std::string date;
    std::string symbol;
    double open;
    double close;
    long volume;
};

// Load all data from CSV into memory
void loadMarketData(const std::string& filepath);

// Get closing price for a symbol on a specific date
double getClosingPrice(const std::string& symbol, const std::string& date);

// Show all available symbols (once loaded)
std::vector<std::string> getAvailableSymbols();

#endif
