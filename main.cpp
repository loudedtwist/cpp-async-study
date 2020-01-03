#include <iostream>
#include <thread>
#include <chrono>

#include "Async.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void fetchStuff(int howMuch) {
  std::cout << "Fetching " << howMuch << " stuff." << std::endl;
}

auto loopAround(unsigned forHowLong) {
  for (int i = 0; i < forHowLong; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Looping around " << i << " in thread " << std::this_thread::get_id() << "..." << std::endl;
  }
  return forHowLong;
}

int main() {
  std::cout << "Hello, World!" << std::endl;
  std::thread workerThread(&fetchStuff, 2);
  std::thread workerThread2(&fetchStuff, 3);

  auto value = 10;
  std::thread workerThread22([&value]() {++value;});
  std::thread workerThread223([&value]() {++value;});
  workerThread22.join();
  workerThread223.join();


  workerThread.join();
  workerThread2.join();

  std::cout << "Done doing stuff" << std::endl;

  std::future<unsigned> resultFuture = std::async(std::launch::async, loopAround, 5);
  std::cout << "Started loop from thread " << std::this_thread::get_id() << std::endl;

  while (true) {
    std::cout << "tick" << std::endl;
    if (async::is_ready(resultFuture)) {
      auto result = resultFuture.get();
      std::cout << "Looped for " << result  << " seconds" << std::endl;
      return 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return 0;
}











#pragma clang diagnostic pop