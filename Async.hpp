#pragma once

#include <future>

namespace async {

template<typename R>
bool is_ready(std::future<R> const& f) {
  return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

}