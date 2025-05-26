#include "shennon.h"
#include <fstream>
#include <bitset>
#include <map>
#include <stdexcept>

void encodeFile(const std::string& input_file, const std::string& output_file, 
               const std::string& dict_file) {
    std::vector<Symbol> symbols;
    analyzeFile(input_file, symbols);
    buildShannonCodes(symbols);
    writeDictionary(dict_file, symbols);

    std::map<char, std::string> code_map;
    for (const auto& symbol : symbols) {
        code_map[symbol.byte] = symbol.code;
    }

    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);
    if (!in || !out) {
        throw std::runtime_error("Error opening files for encoding");
    }

    char byte;
    std::string buffer;
    while (in.get(byte)) {
        buffer += code_map[byte];
        
        while (buffer.size() >= 8) {
            std::bitset<8> bits(buffer.substr(0, 8));
            out.put(static_cast<char>(bits.to_ulong()));
            buffer = buffer.substr(8);
        }
    }

    if (!buffer.empty()) {
        while (buffer.size() < 8) buffer += '0';
        std::bitset<8> bits(buffer);
        out.put(static_cast<char>(bits.to_ulong()));
    }
    if (!buffer.empty()) {
        buffer += "1";
        while (buffer.size() < 8) {
            buffer += "0";
        }
        std::bitset<8> bits(buffer);
        out.put(static_cast<char>(bits.to_ulong()));
    }
}

void decodeFile(const std::string& input_file, const std::string& output_file, 
               const std::string& dict_file) {
    auto symbols = readDictionary(dict_file);
    buildShannonCodes(symbols);

    std::map<std::string, char> code_map;
    for (const auto& symbol : symbols) {
        code_map[symbol.code] = symbol.byte;
    }

    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);
    if (!in || !out) {
        throw std::runtime_error("Error opening files for decoding");
    }

    std::string bit_string;
    char byte;
    while (in.get(byte)) {
        std::bitset<8> bits(byte);
        bit_string += bits.to_string();
    }
    size_t last_one = bit_string.rfind('1');
    if (last_one != std::string::npos) {
        bit_string.resize(last_one); 
    }

    std::string current_code;
    size_t pos = 0;
    while (pos < bit_string.length()) {
        current_code += bit_string[pos++];
        if (code_map.count(current_code)) {
            out.put(code_map[current_code]);
            current_code.clear();
        }
    }
}
