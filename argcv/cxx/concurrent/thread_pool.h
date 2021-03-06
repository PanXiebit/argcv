/**
 * Copyright (c) 2012 Jakob Progsch, Václav Zeman
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 **/
#ifndef ARGCV_CXX_CONCURRENT_THREAD_POOL_H_
#define ARGCV_CXX_CONCURRENT_THREAD_POOL_H_

#include <condition_variable>  // NOLINT(build/c++11)
#include <functional>
#include <future>  // NOLINT(build/c++11)
#include <memory>
#include <mutex>  // NOLINT(build/c++11)
#include <queue>
#include <stdexcept>
#include <thread>  // NOLINT(build/c++11)
#include <utility>
#include <vector>

namespace argcv {
namespace concurrent {
class ThreadPool {
 public:
  explicit ThreadPool(size_t);
  template <class F, class... Args>
  auto Enqueue(F&& f, Args&&... args)  // NOLINT(build/c++11)
      -> std::future<typename std::result_of<F(Args...)>::type>;
  ~ThreadPool();

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers_;
  // the task queue
  std::queue<std::function<void()>> tasks_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

// add new work item to the pool
template <class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args)  // NOLINT(build/c++11)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(
      std::forward<F>(f), std::forward<Args>(args)...));  // NOLINT(build/c++11)

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow enqueueing after stopping the pool
    if (stop_) throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}

}  // namespace concurrent
}  // namespace argcv

#endif  //  ARGCV_CXX_CONCURRENT_THREAD_POOL_H_
