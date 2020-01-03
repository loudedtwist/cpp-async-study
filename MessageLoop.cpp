#include "MessageLoop.hpp"

// accept any Future or any
// create future
auto FetchData() -> Future {
  return std::async(std::launch::async, []() {
    auto threadId = std::this_thread::get_id();

    std::cout << "    [Async task] tick 1 " << threadId << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "    [Async task] tick 2 " << threadId << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "    [Async task] tick 3 " << threadId << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "    [Async task] tick 4 " << threadId << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    return std::any{std::string(R"({ "name":"John", "age":30, "car":null })")};
  });
}

auto FetchDataUnsubscribe() -> std::any {
  auto threadId = std::this_thread::get_id();
  std::cout << "        [Async task] unsubscribe task " << threadId << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return std::any{std::string(R"({ "task":"unsubscribe" })")};
}

void printResult(const std::string& result) {
  auto threadId = std::this_thread::get_id();
  std::cout << " _____________________" << std::endl;
  std::cout << "|                     |" << std::endl;
  std::cout << "|       RESULT        |" << std::endl;
  std::cout << "|                     |" << std::endl;
  std::cout << " ---------------------" << std::endl;
  std::cout << " got in thread << " << threadId << std::endl;
  std::cout << result << std::endl;
  std::cout << "" << std::endl;
}

void UsageExample() {
  MessageLoop::Instance().AddFuture(FetchData()).Then([](const std::any& result) {
    auto json = std::any_cast<std::string>(result);
    printResult(json);
  });
}

void UsageExampleWithUnsubscribe() {
  MessageLoop::Instance().AddTask(FetchDataUnsubscribe).Then([](const std::any& result) {
    auto json = std::any_cast<std::string>(result);
    printResult(json);
  }).UnsubscribeWhenThisObjectDestroyed();
}

auto UsageExampleWithUnsubscribe2() -> CallbackProvider {
  return MessageLoop::Instance().AddTask(FetchDataUnsubscribe).Then([](const std::any& result) {
    auto json = std::any_cast<std::string>(result);
    printResult("Should be in the console: " + json);
  }).UnsubscribeWhenThisObjectDestroyed();
}