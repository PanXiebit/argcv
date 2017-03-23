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
#include <vector>

namespace argcv {
namespace concurrent {
class thread_pool {
 public:
  explicit thread_pool(size_t);
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args)  // NOLINT(build/c++11)
      -> std::future<typename std::result_of<F(Args...)>::type>;
  ~thread_pool();

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers;
  // the task queue
  std::queue<std::function<void()> > tasks;

  // synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};
}  // namespace concurrent
}  // namespace argcv

#endif  //  ARGCV_CXX_CONCURRENT_THREAD_POOL_H_
