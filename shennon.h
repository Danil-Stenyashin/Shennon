#ifndef SHENNON_H
#define SHENNON_H

#include <string>
#include <vector>
#include <map>

struct Symbol {
    char byte;
    double probability;
    std::string code;
};

void analyzeFile(const std::string& filename, std::vector<Symbol>& symbols);
void buildShannonCodes(std::vector<Symbol>& symbols);
void writeDictionary(const std::string& filename, const std::vector<Symbol>& symbols);
std::vector<Symbol> readDictionary(const std::string& filename);

void encodeFile(const std::string& input_file, 
               const std::string& output_file,
               const std::string& dict_file);

void decodeFile(const std::string& input_file,
               const std::string& output_file,
               const std::string& dict_file);

#endif
