#include <iostream>
#include <thread>
#include <chrono>

#include "Async.hpp"
#include "MessageLoop.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

auto fetchStuff() -> std::any {
  std::this_thread::sleep_for(std::chrono::seconds(4));
  return std::any{std::string(R"({body: "json from internet"})")};
}

auto fetchStuffFast() -> std::any {
  std::cout << "    [Async task] getting stuff fast " << std::this_thread::get_id() << std::endl;
  return std::any{std::string(R"({body: "fast json from internet"})")};
}

int main() {
  std::cout << "[Application] starting " << std::this_thread::get_id() << std::endl;
  UsageExample();
  UsageExample();
  UsageExampleWithUnsubscribe();
  auto asyncTaskId = UsageExampleWithUnsubscribe2();

  MessageLoop::Instance().AddTask(fetchStuff).Then([](const std::any& result) {
    auto json = std::any_cast<std::string>(result);
    printResult(json);
  });

  MessageLoop::Instance().AddTask(fetchStuffFast).Then([](const std::any& result) {
    auto json = std::any_cast<std::string>(result);
    printResult(json);
  });

  while (true) {
    std::cout << "[Main loop] tick" << std::this_thread::get_id() << std::endl;
    MessageLoop::Instance().Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  return 0;
}











#pragma clang diagnostic pop