#ifndef SORTER_HPP_
#define SORTER_HPP_

#include "config.h"
#include "tape.h"

namespace tape_sorter {

struct sorter {
public:
    /*
     * main function that sorts the data from the tape
     */
    static void sort(
        const std::string &input_file,
        const std::string &output_file,
        const std::string &config_file,
        std::size_t number_tmp_tapes = 2
    );

    /*
     * function reads the data in blocks by the ram_size of the elements,
     * sorts them and writes them to temporary tapes.
     * First block is recorded on the 0'th tape, the n'th block on the (n%k)'th tape
     */
    static void read_data_from_tape(
        const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
        const std::unique_ptr<tape::tape> &source_tape,
        std::size_t ram_size
    );

    /*
     * function reads sorted blocks from the tape and
     * writes them to temporary tapes.
     * First block is recorded on the 0'th tape, the n'th block on the (n%k)'th tape
     */
    static void read_sorted_blocks(
        const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
        const std::unique_ptr<tape::tape> &source_tape
    );

    /*
     * function merge data from temporary tapes to output tapes
     * and checks if the data is sorted
     */
    static bool merge_tmp_tapes(
        const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
        const std::unique_ptr<tape::tape> &output_tape
    );
};

}  // namespace tape_sorter

#endif  // SORTER_HPP_
