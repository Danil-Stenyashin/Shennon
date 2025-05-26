#include "shennon.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <map>
#include <stdexcept>

void analyzeFile(const std::string& filename, std::vector<Symbol>& symbols) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::map<char, int> freq;
    char byte;
    int total = 0;

    while (file.get(byte)) {
        freq[byte]++;
        total++;
    }

    for (const auto& [b, count] : freq) {
        symbols.push_back({b, static_cast<double>(count) / total, ""});
    }

    sort(symbols.begin(), symbols.end(), 
        [](const Symbol& a, const Symbol& b) { 
            return a.probability > b.probability; 
        });
}

void buildShannonCodes(std::vector<Symbol>& symbols) {
    double cumulative_prob = 0.0;

    for (auto& symbol : symbols) {
        int code_length = ceil(-log2(symbol.probability));
        double temp_prob = cumulative_prob;
        std::string code;

        for (int i = 0; i < code_length; ++i) {
            temp_prob *= 2;
            if (temp_prob >= 1.0) {
                code += '1';
                temp_prob -= 1.0;
            } else {
                code += '0';
            }
        }

        symbol.code = code;
        cumulative_prob += symbol.probability;
    }
}

void writeDictionary(const std::string& filename, const std::vector<Symbol>& symbols) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Error creating dictionary file: " + filename);
    }

    out << symbols.size() << "\n";
    for (const auto& symbol : symbols) {
        out << static_cast<int>(symbol.byte) << " " 
            << std::fixed << std::setprecision(15) << symbol.probability << "\n";
    }
}

std::vector<Symbol> readDictionary(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Error opening dictionary file: " + filename);
    }

    std::vector<Symbol> symbols;
    int count;
    in >> count;

    for (int i = 0; i < count; ++i) {
        Symbol symbol;
        int byte;
        in >> byte >> symbol.probability;
        symbol.byte = static_cast<char>(byte);
        symbols.push_back(symbol);
    }

    return symbols;
}
