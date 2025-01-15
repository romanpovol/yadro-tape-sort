#ifndef TAPE_H
#define TAPE_H

#include <cstdint>
#include <fstream>
#include "config.h"

namespace tape {

struct tape {
public:
    explicit tape(
        const std::string &filename,
        tape_config::config config
    );

    tape(const tape &) = delete;
    tape(tape &&) noexcept = default;
    tape &operator=(const tape &) = delete;
    tape &operator=(tape &&) noexcept = default;

    ~tape();

    std::int32_t read();
    void write(std::int32_t value);

    void move_right();
    void move_left();
    void rewind_tape();

    bool is_end();
    bool is_start();

    void move_head(std::streamoff offset);

    static tape create_tmp_tape(const tape_config::config &config);

    std::string get_tape_name();

    /*
     * When interacting with temporary tapes, the data prefix may be relevant,
     * so we keep the current data size.
     */
    void set_actual_size(std::size_t actual_size);

private:
    std::fstream filestream_;
    std::string filename_;
    std::size_t offset_;
    std::size_t actual_size_;
    tape_config::config config_;
};

}  // namespace tape

#endif  // TAPE_H