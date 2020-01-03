#pragma once

#include <cstdint>
#include <chrono>

namespace utils {

using unique_id = uint64_t;

auto GenerateTimestampID() -> unique_id;

}
