#include "Utils.hpp"

namespace utils {

unique_id GenerateTimestampID() {
  return (unique_id) std::chrono::duration_cast<std::chrono::nanoseconds>(
    std::chrono::system_clock::now().time_since_epoch() + std::chrono::high_resolution_clock::now().time_since_epoch())
    .count();
}


}
