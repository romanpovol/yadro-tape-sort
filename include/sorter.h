#ifndef SORTER_HPP_
#define SORTER_HPP_

#include "config.h"
#include "tape.h"

namespace tape_sorter {

struct sorter {
public:
    static void sort(
        const std::string &input_file,
        const std::string &output_file,
        const std::string &config_file,
        std::size_t number_tmp_tapes = 2
    );
};

}  // namespace tape_sorter

#endif  // SORTER_HPP_
