#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <chrono>
#include "yaml-cpp/yaml.h"

namespace tape_config {

struct config {
private:
    YAML::Node config_;
    std::size_t ram_size_;
    std::chrono::milliseconds read_delay_{};
    std::chrono::milliseconds write_delay_{};
    std::chrono::milliseconds move_delay_{};
    std::chrono::milliseconds rewind_delay_{};

public:
    explicit config(const std::string &config_file);

    config(const config &) = default;
    config(config &&) = default;
    config &operator=(const config &) = default;
    config &operator=(config &&) = default;
    ~config() = default;

    std::size_t ram_size() const;

    std::chrono::milliseconds read_delay() const;

    std::chrono::milliseconds write_delay() const;

    std::chrono::milliseconds move_delay() const;

    std::chrono::milliseconds rewind_delay() const;
};

}  // namespace tape_config

#endif  // CONFIG_HPP_
