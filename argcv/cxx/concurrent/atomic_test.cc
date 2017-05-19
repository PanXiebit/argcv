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
#include "argcv/cxx/concurrent/atomic.h"

#include <cstdio>

#include "argcv/cxx/test/gtest_ext.h"
#include "gtest/gtest.h"

using namespace argcv::concurrent;  // NOLINT(build/namespaces)

TEST(atomic, plus_one) {
  atomic<uint32_t> apple(0);
  EXPECT_EQ(0, AtomicFetchAdd<uint32_t>(&apple, 1));
  EXPECT_EQ(1, AtomicFetchAdd<uint32_t>(&apple, 1));
  EXPECT_EQ(2, AtomicFetchAdd<uint32_t>(&apple, 1));
  printf("score: %u", AtomicFetchAdd<uint32_t>(&apple, 1));
}

TEST(ticket_lock, basic_test) {
  TicketLock tl;
  ASSERT_USECS(10, tl.Lock());
  ASSERT_USECS(10, tl.Unlock());
  ASSERT_USECS(10, tl.Lock());
  ASSERT_USECS(10, tl.Unlock());
}
