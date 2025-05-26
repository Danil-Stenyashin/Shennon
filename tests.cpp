#include "shennon.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

void create_test_files() {

    fs::create_directory("test_data");


    {
        std::ofstream out("test_data/simple.txt");
        out << "abracadabra";
    }


    {
        std::ofstream out("test_data/random.bin", std::ios::binary);
        for (int i = 0; i < 1000; ++i) {
            char byte = static_cast<char>(rand() % 256);
            out.write(&byte, 1);
        }
    }
}

bool compare_files(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1 || !f2) return false;

    return std::equal(
        std::istreambuf_iterator<char>(f1),
        std::istreambuf_iterator<char>(),
        std::istreambuf_iterator<char>(f2)
    );
}

void test_simple_file() {
    encodeFile("test_data/simple.txt", "test_data/simple_encoded.bin", "test_data/simple_dict.txt");
    decodeFile("test_data/simple_encoded.bin", "test_data/simple_decoded.txt", "test_data/simple_dict.txt");

    assert(compare_files("test_data/simple.txt", "test_data/simple_decoded.txt"));
    std::cout << "Simple file test passed!\n";
}

void test_random_binary() {
    encodeFile("test_data/random.bin", "test_data/random_encoded.bin", "test_data/random_dict.txt");
    decodeFile("test_data/random_encoded.bin", "test_data/random_decoded.bin", "test_data/random_dict.txt");

    assert(compare_files("test_data/random.bin", "test_data/random_decoded.bin"));
    std::cout << "Random binary test passed!\n";
}

void test_dictionary_io() {
    std::vector<Symbol> symbols;
    analyzeFile("test_data/simple.txt", symbols);
    writeDictionary("test_data/test_dict.txt", symbols);
    
    auto read_symbols = readDictionary("test_data/test_dict.txt");
    
    assert(symbols.size() == read_symbols.size());
    for (size_t i = 0; i < symbols.size(); ++i) {
        assert(symbols[i].byte == read_symbols[i].byte);
        assert(std::abs(symbols[i].probability - read_symbols[i].probability) < 1e-9);
    }
    std::cout << "Dictionary IO test passed!\n";
}

void test_shannon_codes() {
    std::vector<Symbol> symbols = {
        {'a', 0.5, ""},
        {'b', 0.25, ""},
        {'c', 0.125, ""},
        {'d', 0.125, ""}
    };
    
    buildShannonCodes(symbols);
    
    assert(symbols[0].code.length() == 1);  
    assert(symbols[1].code.length() == 2); 
    assert(symbols[2].code.length() == 3); 
    assert(symbols[3].code.length() == 3);  
    
    std::cout << "Shannon codes test passed!\n";
}

int main() {
    try {
        create_test_files();
        
        test_simple_file();
        test_random_binary();
        test_dictionary_io();
        test_shannon_codes();
        
        std::cout << "All tests passed successfully!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
