#include "shennon.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file> <dictionary_file>\n";
        return 1;
    }

    try {
        decodeFile(argv[1], argv[2], argv[3]);
        std::cout << "File decoded successfully!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
