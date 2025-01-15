#include "sorter.h"
#include <algorithm>

namespace tape_sorter {

void sorter::read_data_from_tape(
    const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
    const std::unique_ptr<tape::tape> &source_tape,
    std::size_t ram_size
) {
    if (!source_tape->is_start()) {
        source_tape->rewind_tape();
    }

    for (const auto &tape : tmp_tapes) {
        if (!tape->is_start()) {
            tape->rewind_tape();
        }
    }

    std::size_t current_tmp_tape_number = 0;
    while (!source_tape->is_end()) {
        std::vector<std::int32_t> data;
        data.reserve(ram_size);
        while (!source_tape->is_end() && data.size() != ram_size) {
            data.emplace_back(source_tape->read());
        }

        std::sort(data.begin(), data.end());

        for (auto &val : data) {
            tmp_tapes[current_tmp_tape_number]->write(val);
        }

        current_tmp_tape_number =
            (current_tmp_tape_number + 1 == tmp_tapes.size()
                 ? 0
                 : current_tmp_tape_number + 1);
    }
}

void sorter::read_sorted_blocks(
    const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
    const std::unique_ptr<tape::tape> &source_tape
) {
    if (!source_tape->is_start()) {
        source_tape->rewind_tape();
    }

    for (const auto &tape : tmp_tapes) {
        if (!tape->is_start()) {
            tape->rewind_tape();
        }
    }

    std::vector <std::size_t> tape_sizes(tmp_tapes.size());

    std::size_t current_tmp_tape_number = 0;
    while (!source_tape->is_end()) {
        std::int32_t previous_value = source_tape->read();
        tmp_tapes[current_tmp_tape_number]->write(previous_value);
        tape_sizes[current_tmp_tape_number]++;

        while (!source_tape->is_end()) {
            std::int32_t current_value = source_tape->read();
            if (previous_value > current_value) {
                source_tape->move_left();
                break;
            }
            previous_value = current_value;
            tmp_tapes[current_tmp_tape_number]->write(previous_value);
            tape_sizes[current_tmp_tape_number]++;
        }

        current_tmp_tape_number =
            (current_tmp_tape_number + 1 == tmp_tapes.size()
                 ? 0
                 : current_tmp_tape_number + 1);
    }

    for (std::size_t tape_number = 0; tape_number < tape_sizes.size(); tape_number++) {
        tmp_tapes[tape_number]->set_actual_size(tape_sizes[tape_number]);
    }
}

bool sorter::merge_tmp_tapes(
    const std::vector<std::unique_ptr<tape::tape>> &tmp_tapes,
    const std::unique_ptr<tape::tape> &output_tape
) {
    enum class ValueType { kValidValue = 1, kInvalidValue = 0 };

    if (!output_tape->is_start()) {
        output_tape->rewind_tape();
    }

    for (const auto &tape : tmp_tapes) {
        if (!tape->is_start()) {
            tape->rewind_tape();
        }
    }

    std::vector<std::pair<std::int32_t, ValueType>> top_of_tapes(tmp_tapes.size());
    bool is_data_sorted = true;
    std::int32_t top_of_output_tape = 0;

    while (true) {
        bool was_update = false;
        std::size_t min_tape_number = 0;

        for (size_t tape_number = 0; tape_number < top_of_tapes.size();
             tape_number++) {
            if (top_of_tapes[tape_number].second == ValueType::kInvalidValue &&
                !tmp_tapes[tape_number]->is_end()) {
                top_of_tapes[tape_number] = {
                    tmp_tapes[tape_number]->read(), ValueType::kValidValue};
            }

            if (top_of_tapes[tape_number].second == ValueType::kValidValue &&
                (!was_update || top_of_tapes[tape_number].first <
                                    top_of_tapes[min_tape_number].first)) {
                min_tape_number = tape_number;
                was_update = true;
            }
        }

        if (!was_update) {
            break;
        }

        if (output_tape->is_start()) {
            top_of_output_tape = top_of_tapes[min_tape_number].first;
        }

        output_tape->write(top_of_tapes[min_tape_number].first);
        top_of_tapes[min_tape_number].second = ValueType::kInvalidValue;

        if (top_of_output_tape > top_of_tapes[min_tape_number].first) {
            is_data_sorted = false;
        }
        top_of_output_tape = top_of_tapes[min_tape_number].first;
    }

    return is_data_sorted;
}

void sorter::sort(
    const std::string &input_file,
    const std::string &output_file,
    const std::string &config_file,
    std::size_t
        number_tmp_tapes  // NB: ram size should be bigger than number_tmp_tapes
) {
    tape_config::config config(config_file);

    std::unique_ptr<tape::tape> input_tape =
        std::make_unique<tape::tape>(input_file, config);
    std::unique_ptr<tape::tape> output_tape =
        std::make_unique<tape::tape>(output_file, config);

    std::vector<std::unique_ptr<tape::tape>> tmp_tapes;
    tmp_tapes.reserve(number_tmp_tapes);
    for (std::size_t i = 0; i < number_tmp_tapes; i++) {
        tmp_tapes.push_back(
            std::make_unique<tape::tape>(tape::tape::create_tmp_tape(config))
        );
    }

    read_data_from_tape(tmp_tapes, input_tape, config.ram_size());

    while (true) {
        bool is_data_sorted = merge_tmp_tapes(tmp_tapes, output_tape);
        if (is_data_sorted) {
            return;
        }

        read_sorted_blocks(tmp_tapes, output_tape);
    }
}

}  // namespace tape_sorter