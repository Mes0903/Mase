#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

/**
 * @file ThreadSafeQueue.h
 * @author 5568ke
 * @brief A thread-safe queue implementation using std::queue and std::mutex.
 * @version 0.1
 * @date 2024-10-14
 *
 * @copyright Copyright (c) 2024
 *
 */

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