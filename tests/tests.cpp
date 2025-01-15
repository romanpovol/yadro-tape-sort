#include <memory>
#include "doctest.h"
#include "tape.h"
#include "utils.h"

constexpr char CONFIG_BIG_RAM[] = "../tests/config_big_ram.yaml";
constexpr char CONFIG_SMALL_RAM[] = "../tests/config_big_ram.yaml";
constexpr char INPUT_TAPE[] = "../tests/input";
constexpr char OUTPUT_TAPE[] = "../tests/output";

TEST_CASE("empty input") {
    auto data = generate_array(0);
    check_sort(INPUT_TAPE, OUTPUT_TAPE, CONFIG_BIG_RAM, data);
}

TEST_CASE("ram > data") {
    auto data = generate_array(10);
    check_sort(INPUT_TAPE, OUTPUT_TAPE, CONFIG_BIG_RAM, data);
}

TEST_CASE("ram < data") {
    auto data = generate_array(1024);
    check_sort(INPUT_TAPE, OUTPUT_TAPE, CONFIG_SMALL_RAM, data);
}

TEST_CASE("big test") {
    for (size_t test = 0; test < 5; test++) {
        auto data = generate_array(10000);
        check_sort(INPUT_TAPE, OUTPUT_TAPE, CONFIG_SMALL_RAM, data);
    }
}

TEST_CASE("tape read-write") {
    tape_config::config config(CONFIG_SMALL_RAM);
    auto current_tape =
        std::make_unique<tape::tape>(tape::tape::create_tmp_tape(config));

    CHECK(current_tape->is_start());

    std::vector<std::int32_t> data = generate_array(10);
    for (auto &val : data) {
        current_tape->write(val);
    }

    current_tape->rewind_tape();
    CHECK(current_tape->is_start());

    size_t position = 0;
    while (position < data.size() && !current_tape->is_end()) {
        std::int32_t current_val = current_tape->read();
        CHECK_EQ(current_val, data[position]);
        position++;
    }

    CHECK_EQ(position == data.size(), current_tape->is_end());
}

TEST_CASE("tape moves") {
    tape_config::config config(CONFIG_SMALL_RAM);
    auto current_tape =
        std::make_unique<tape::tape>(tape::tape::create_tmp_tape(config));
    std::int32_t current_val = 0;

    CHECK(current_tape->is_start());

    std::vector<std::int32_t> data = generate_array(5);
    for (auto &val : data) {
        current_tape->write(val);
    }

    current_tape->rewind_tape();
    CHECK(current_tape->is_start());

    current_val = current_tape->read();
    current_tape->move_left();
    CHECK(current_tape->is_start());
    CHECK_EQ(current_val, data[0]);

    current_tape->move_right();
    current_tape->move_right();
    current_val = current_tape->read();
    current_tape->move_left();
    CHECK_EQ(current_val, data[2]);

    current_tape->move_right();
    current_tape->move_left();
    current_val = current_tape->read();
    current_tape->move_left();
    CHECK_EQ(current_val, data[2]);

    current_tape->move_right();
    current_tape->move_right();
    current_val = current_tape->read();
    CHECK_EQ(current_val, data[4]);
    CHECK(current_tape->is_end());

    current_tape->rewind_tape();
    CHECK(current_tape->is_start());
}
