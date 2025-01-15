#ifndef TAPE_SORT_UTILS_H
#define TAPE_SORT_UTILS_H

#include <algorithm>
#include <string>

void write_to_tape(
    const std::string &path_to_tape,
    std::vector<std::int32_t> &data
);

void clear_tape(const std::string &path_to_tape);

std::vector<std::int32_t> read_from_tape(const std::string &path_to_tape);

std::vector<std::int32_t> generate_array(int len);

void check_sort(
    const std::string &input_file,
    const std::string &output_file,
    const std::string &config_file,
    std::vector<std::int32_t> &data
);

#endif  // TAPE_SORT_UTILS_H
