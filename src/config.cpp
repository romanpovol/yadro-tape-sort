#include "config.h"

namespace tape_config {

config::config(const std::string &config_file) {
    config_ = YAML::LoadFile(config_file);
    ram_size_ = config_["ram_size"].as<std::size_t>();
    read_delay_ =
        std::chrono::milliseconds(config_["read_delay"].as<unsigned>());
    write_delay_ =
        std::chrono::milliseconds(config_["write_delay"].as<unsigned>());
    move_delay_ =
        std::chrono::milliseconds(config_["move_delay"].as<unsigned>());
    rewind_delay_ =
        std::chrono::milliseconds(config_["rewind_delay"].as<unsigned>());
}

std::size_t config::ram_size() const {
    return ram_size_;
}

std::chrono::milliseconds config::read_delay() const {
    return read_delay_;
}

std::chrono::milliseconds config::write_delay() const {
    return write_delay_;
}

std::chrono::milliseconds config::move_delay() const {
    return move_delay_;
}

std::chrono::milliseconds config::rewind_delay() const {
    return rewind_delay_;
}

}  // namespace tape_config
