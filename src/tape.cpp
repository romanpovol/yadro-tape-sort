#include "tape.h"
#include <unistd.h>
#include <thread>

namespace tape {

tape::tape(const std::string &filename, const tape_config::config &config)
    : filestream_(
          filename,
          std::ios_base::in | std::ios_base::out | std::ios::binary
      ),
      filename_(filename),
      offset_(0),
      config_(config) {
}

tape::~tape() {
    filestream_.close();
}

std::int32_t tape::read() {
    std::this_thread::sleep_for(config_.read_delay());

    std::int32_t value = 0;
    filestream_.read(reinterpret_cast<char *>(&value), sizeof(value));
    move_head(-static_cast<std::streamoff>(sizeof(value)));

    move_right();

    return value;
}

void tape::write(std::int32_t value) {
    std::this_thread::sleep_for(config_.write_delay());

    filestream_.write(reinterpret_cast<char *>(&value), sizeof(value));
    move_head(-static_cast<std::streamoff>(sizeof(value)));

    move_right();
}

void tape::move_right() {
    std::this_thread::sleep_for(config_.move_delay());

    offset_++;
    move_head(static_cast<std::streamoff>(sizeof(std::int32_t)));
}

void tape::move_left() {
    std::this_thread::sleep_for(config_.move_delay());

    offset_--;
    move_head(-static_cast<std::streamoff>(sizeof(std::int32_t)));
}

void tape::rewind_tape() {
    std::this_thread::sleep_for(config_.rewind_delay());

    offset_ = 0;
    filestream_.clear();
    filestream_.seekg(offset_, std::ios_base::beg);
    filestream_.seekp(offset_, std::ios_base::beg);
}

bool tape::is_end() {
    return filestream_.peek() == EOF;
}

bool tape::is_start() {
    return filestream_.tellg() == 0;
}

void tape::move_head(std::streamoff offset) {
    filestream_.seekg(offset, std::ios_base::cur);
}

tape tape::create_tmp_tape(const tape_config::config &config) {
    char tmp_tape_filename[] = "/tmp/temp-tapeXXXXXX";
    int fd = mkstemp(tmp_tape_filename);
    if (fd == -1) {
        throw std::runtime_error(
            "Error while create file for tmp tape: " +
            std::string(tmp_tape_filename)
        );
    }

    close(fd);

    return tape(tmp_tape_filename, config);
}

std::string tape::get_tape_name() {
    return filename_;
}

}  // namespace tape