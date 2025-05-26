#include "shennon.h"
#include <fstream>
#include <bitset>
#include <map>
#include <stdexcept>
#include <vector>

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
        throw std::runtime_error("File opening error");
    }

 
    in.seekg(0, std::ios::end);
    size_t file_size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<char> file_data(file_size);
    in.read(file_data.data(), file_size);


    std::string bit_buffer;
    for (char byte : file_data) {
        bit_buffer += code_map[byte];
        

        while (bit_buffer.size() >= 8) {
            std::bitset<8> bits(bit_buffer.substr(0, 8));
            out.put(static_cast<char>(bits.to_ulong()));
            bit_buffer = bit_buffer.substr(8);
        }
    }


    if (!bit_buffer.empty()) {

        bit_buffer += "1"; 
        while (bit_buffer.size() < 8) {
            bit_buffer += "0";
        }
        std::bitset<8> bits(bit_buffer);
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
        throw std::runtime_error("File opening error");
    }


    in.seekg(0, std::ios::end);
    size_t file_size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<char> encoded_data(file_size);
    in.read(encoded_data.data(), file_size);


    std::string bit_string;
    for (char byte : encoded_data) {
        bit_string += std::bitset<8>(byte).to_string();
    }


    size_t last_one = bit_string.rfind('1');
    if (last_one != std::string::npos) {
        bit_string.resize(last_one);
    }


    std::string current_code;
    for (char bit : bit_string) {
        current_code += bit;
        if (code_map.count(current_code)) {
            out.put(code_map[current_code]);
            current_code.clear();
        }
    }
}
