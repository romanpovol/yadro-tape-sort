#include <iostream>
#include "sorter.h"

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        std::cerr << "Wrong number of arguments, expected <input-file> "
                     "<output-file> [config-file]"
                  << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    std::string config_file;
    if (argc == 4) {
        config_file = argv[3];
    } else {
        config_file = "config_big_ram.yaml";
    }

    tape_sorter::sorter::sort(input_file, output_file, config_file);
    return 0;
}