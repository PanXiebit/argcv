/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#ifndef ARGCV_CXX_CONCURRENT_ATOMIC_H_
#define ARGCV_CXX_CONCURRENT_ATOMIC_H_

#include <atomic>
#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

namespace argcv {
namespace concurrent {

// http://en.cppreference.com/w/cpp/language/type_alias
template <typename T>
using atomic = std::atomic<T>;  // typedef std::atomic<T> as atomic<T>

template <typename T>
T AtomicFetchAdd(atomic<T>* obj, T arg) {
  return obj->fetch_add(arg);
}

template <typename T>
T AtomicFetchSub(atomic<T>* obj, T arg) {
  return obj->fetch_sub(arg);
}

template <typename T>
T AtomicFetchOr(atomic<T>* obj, T arg) {
  return obj->fetch_or(arg);
}

template <typename T>
T AtomicFetchXor(atomic<T>* obj, T arg) {
  return obj->fetch_xor(arg);
}

class Spinlock {
 public:
  // ATOMIC_FLAG_INIT = {false}
  Spinlock() : locked_(false) {}
  void Lock() {
    while (locked_.test_and_set(std::memory_order_acquire)) {
      // wait
      std::this_thread::yield();
    }
  }
  bool TryLock() { return locked_.test_and_set(std::memory_order_acquire); }
  void Unlock() { locked_.clear(std::memory_order_release); }

 private:
  std::atomic_flag locked_;
};

class TicketLock {
 public:
  // ATOMIC_FLAG_INIT = {false}
  TicketLock() : ticket_(0), serve_(0) {}
  void Lock() {
    auto t = AtomicFetchAdd<uint32_t>(&ticket_, 1);
    while (t != serve_) {
      // wait
      std::this_thread::yield();
    }
  }
  void Unlock() { AtomicFetchAdd<uint32_t>(&serve_, 1); }

 private:
  atomic<uint32_t> ticket_;
  atomic<uint32_t> serve_;
};

}  // namespace concurrent
}  // namespace argcv

#endif  //  ARGCV_CXX_CONCURRENT_ATOMIC_H_
