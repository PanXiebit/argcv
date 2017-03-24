/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Yu Jing <yu@argcv.com>
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
#include "argcv/cxx/concurrent/thread_pool.h"

#include <cstdio>

#include <future>  // NOLINT(build/c++11)
#include <vector>

#include "argcv/cxx/concurrent/atomic.h"
#include "argcv/cxx/test/gtest_ext.h"
#include "gtest/gtest.h"

using namespace argcv::concurrent;  // NOLINT(build/namespaces)

TEST(thread_pool, plus_one) {
  atomic<uint32_t> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size);
  bool all_done = true;
  std::vector<std::future<uint32_t> > results;
  for (size_t i = 0; i < task_size; i++) {
    results.emplace_back(pool.enqueue([i, thread_size, task_size, &apple] {
      printf("[test_case_threads] start: %zu of %zu\n", i, task_size);
      // usleep(i);
      uint32_t res = atomic_fetch_add<uint32_t>(&apple, (uint32_t)1);
      printf("[test_case_threads] end: %zu of %zu\n", i, task_size);
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ((uint32_t)4950, score);
  EXPECT_EQ((uint32_t)100, apple.load());
}
