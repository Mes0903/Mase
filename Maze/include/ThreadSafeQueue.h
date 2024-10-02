#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <optional>

template <typename T>
class ThreadSafeQueue {
public:
  void enqueue(T value)
  {
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(std::move(value));
  }

  // 正常來說應該要用 condition_variable 的，但我們情況特殊所以長這個怪怪的樣子
  std::optional<T> dequeue()
  {
    std::lock_guard<std::mutex> lock(mtx_);
    if (queue_.empty()) {
      return std::nullopt;
    }

    T value = std::move(queue_.front());
    queue_.pop();
    return value;
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.empty();
  }

private:
  std::queue<T> queue_;
  mutable std::mutex mtx_;
};

#endif