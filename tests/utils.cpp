#include "utils.h"
#include <fstream>
#include <random>
#include <vector>
#include "doctest.h"
#include "sorter.h"

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

void write_to_tape(
    const std::string &path_to_tape,
    std::vector<std::int32_t> &data
) {
    std::ofstream filestream(path_to_tape, std::ios::in | std::ios::app);
    filestream.write(
        reinterpret_cast<char *>(data.data()),
        data.size() * sizeof(std::int32_t)
    );
    filestream.close();
}

void clear_tape(const std::string &path_to_tape) {
    std::ofstream filestream(
        path_to_tape, std::ofstream::out | std::ofstream::trunc
    );
    filestream.close();
}

std::vector<std::int32_t> read_from_tape(const std::string &path_to_tape) {
    std::vector<std::int32_t> data;
    std::ifstream filestream(path_to_tape, std::ios::out | std::ios::binary);

    std::int32_t val = 0;
    while (filestream.peek() != EOF) {
        filestream.read(reinterpret_cast<char *>(&val), sizeof(std::int32_t));
        data.emplace_back(val);
    }

    filestream.close();

    return data;
}

std::vector<std::int32_t> generate_array(const int len) {
    std::vector<std::int32_t> data(len);
    for (auto &el : data) {
        el = rnd() % INT32_MAX;
    }
    return data;
}

void check_sort(
    const std::string &input_file,
    const std::string &output_file,
    const std::string &config_file,
    std::vector<std::int32_t> &data
) {
    clear_tape(input_file);
    clear_tape(output_file);

    write_to_tape(input_file, data);

    tape_sorter::sorter::sort(input_file, output_file, config_file);

    std::vector<std::int32_t> sorted_data = read_from_tape(output_file);

    std::sort(data.begin(), data.end());

    CHECK_EQ(data, sorted_data);
}