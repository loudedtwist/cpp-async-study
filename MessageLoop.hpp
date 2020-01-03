#pragma once

#include <any>
#include <functional>
#include <future>
#include <map>
#include <vector>
#include <utility>
#include <iostream>

#include "Async.hpp"
#include "Utils.hpp"

using MessageType = std::any;
using Future = std::future<MessageType>;
using Callback = std::function<void(MessageType)>;
using Task = std::function<MessageType(void)>;

struct HasCallback {
  virtual void AddCallback(utils::unique_id id, const Callback& callback) = 0;
  virtual void RemoveCallback(utils::unique_id id) = 0;
};

struct CallbackProvider {
public:
  CallbackProvider(const CallbackProvider&) = delete;
  CallbackProvider& operator=(const CallbackProvider&) = delete;

  CallbackProvider(utils::unique_id id, HasCallback& hasCallback)
  : id(id), _messageLoop(hasCallback) {}

  ~CallbackProvider() {
    if (_unsubscribeAfterDestroyed) {
      _messageLoop.RemoveCallback(id);
    }
  }

  /// Register callback for a task or future
  /// It will be called on the main thread
  /// @param callback
  /// @returns self
  auto Then(const Callback& callback) -> CallbackProvider {
    _messageLoop.AddCallback(id, callback);
    return CallbackProvider{id, _messageLoop};
  }

  /// If called after Then, this object will automatically unsubscribe
  /// from message loop callback after it's (CallbackProvider) destroyed.
  /// @returns self
  auto UnsubscribeWhenThisObjectDestroyed() -> CallbackProvider {
    return CallbackProvider{id, _messageLoop, true};
  }

  utils::unique_id id;

private:
  CallbackProvider(utils::unique_id id, HasCallback& hasCallback, bool unsubscribeAfterDestroyed)
  : id(id), _messageLoop(hasCallback), _unsubscribeAfterDestroyed(unsubscribeAfterDestroyed) {}

  HasCallback& _messageLoop;
  bool _unsubscribeAfterDestroyed{false};
};

/// Use message loop only from the main thread.
/// Add tasks or futures from main thread.
/// They will be executed in a different thread,
/// but the callback will called from the main thread again.
/// Task / Future that has no callback registered, will be also deleted.
/// Also it could be change to wait, until there is a recipient.
class MessageLoop : public HasCallback {
public:
  static auto Instance() -> MessageLoop& {
    static MessageLoop messageLoopGlobalStaticVariable;
    return messageLoopGlobalStaticVariable;
  };

  auto AddFuture(Future&& future) -> CallbackProvider {
    auto id = utils::GenerateTimestampID();
    futures.emplace(id, std::move(future));
    return CallbackProvider{id, *this};
  }

  auto AddTask(const Task& task) -> CallbackProvider {
    auto id = utils::GenerateTimestampID();
    futures.emplace(id, std::async(std::launch::async, task));
    return CallbackProvider{id, *this};
  }

  void AddCallback(utils::unique_id id, const Callback& callback) override {
    callbacks.emplace(id, callback);
  }

  void RemoveCallback(utils::unique_id id) override {
    callbacks.erase(id);
  }

  void Update() {
    std::cout << "[Message loop] tick " << std::this_thread::get_id() << " " << futures.size() << std::endl;
    std::vector<utils::unique_id> finishedFutures;

    for (auto& [id, future] : futures) {
      if (async::is_ready(future)) {
        if (callbacks.find(id) != callbacks.end()) {
          auto value = future.get();
          callbacks[id](value);
        }
        finishedFutures.push_back(id);
      }
    }

    for (const auto& finishedId : finishedFutures) {
      futures.erase(finishedId);
      callbacks.erase(finishedId);
    }
  }

private:
  std::map<utils::unique_id, Future> futures;
  std::map<utils::unique_id, Callback> callbacks;
};

auto FetchData() -> Future;
auto FetchDataUnsubscribe() -> std::any;
void printResult(const std::string& result);
void UsageExample();
void UsageExampleWithUnsubscribe();
auto UsageExampleWithUnsubscribe2() -> CallbackProvider;
