/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
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
#include "examples/cxx/hello_world/hello_world.h"

#include <cstdio>

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

#include "boost/algorithm/is_palindrome.hpp"
#include "examples/cxx/hello_world/hello_world_lib.h"
#include "glog/logging.h"

namespace ba = boost::algorithm;

template <typename T>
bool funcComparator(const T& v1, const T& v2) {
  return v1 == v2;
}

struct functorComparator {
  template <typename T>
  bool operator()(const T& v1, const T& v2) const {
    return v1 == v2;
  }
};

// this example for boost::algorithm is based on project
// https://github.com/boostorg/algorithm
// located at:
// https://github.com/boostorg/algorithm/blob/develop/example/is_palindrome_example.cpp
void boost_example() {
  // You can this algorithm with iterators(minimum Bidirectional)
  std::vector<int> vec{1, 2, 1};
  if (ba::is_palindrome(vec.begin(), vec.end()))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  // Of course, you can use const iterators
  if (ba::is_palindrome(vec.cbegin(), vec.cend()))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  // Example with bidirectional iterators
  std::list<int> list{1, 2, 1};
  if (ba::is_palindrome(list.begin(), list.end()))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  // You can use custom comparators like functions, functors, lambdas
  auto lambdaComparator = [](int v1, int v2) { return v1 == v2; };
  auto objFunc = std::function<bool(int, int)>(lambdaComparator);

  if (ba::is_palindrome(vec.begin(), vec.end(), lambdaComparator))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  if (ba::is_palindrome(vec.begin(), vec.end(), funcComparator<int>))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  if (ba::is_palindrome(vec.begin(), vec.end(), functorComparator()))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  if (ba::is_palindrome(vec.begin(), vec.end(), objFunc))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  // You can use ranges
  if (ba::is_palindrome(vec))
    LOG(INFO) << "This container is palindrome";
  else
    LOG(INFO) << "This container is not palindrome";

  // You can use C-strings
  if (ba::is_palindrome("aba"))
    LOG(INFO) << "This C-string is palindrome";
  else
    LOG(INFO) << "This C-string is not palindrome";
}

void func() {
  for (int i = 0; i < 100; i++) {
    LOG_IF_EVERY_N(INFO, (i > 10), 10) << "Hello, World!"
                                       << " for loop at " << i << "th";
  }
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  boost_example();

  LOG(INFO) << "Hello, World! ~~~" << p1(argc);

  func();

  LOG(INFO) << "hello, info@glog";
  LOG(WARNING) << "hello, warning@glog";
  LOG(ERROR) << "hello, error@glog";

  // this will cause a crash
  // LOG(FATAL) << "hello, fatal@glog";
  return 0;
}
