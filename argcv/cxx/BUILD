# The MIT License (MIT)
#
# Copyright (c) 2017 Yu Jing <yu@argcv.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "gtest_ext",
  # srcs = [
  #   "empty.cc",
  # ],
  hdrs = [
    "test/gtest_ext.h"
  ],
  deps = [
      "//external:gtest_main",
  ],
)

cc_library(
  name = "argcv_concurrent",
  srcs = glob(
    [
      "concurrent/*.c",
      "concurrent/*.cc",
    ],
    exclude = [
      "concurrent/*_test.cc",
    ],
  ),
  hdrs = glob(
    [
      "concurrent/*.h",
    ],
    exclude = [
      "concurrent/*_test.h",
    ],
  )
)


cc_library(
  name = "argcv_nio",
  srcs = glob(
    [
      "nio/*.c",
      "nio/*.cc",
    ],
    exclude = [
      "nio/*_test.cc",
    ],
  ),
  hdrs = glob(
    [
      "nio/*.h",
    ],
    exclude = [
      "nio/*_test.h",
    ],
  ),
  deps = [
    "//external:glog",
  ],
)

cc_test(
  name = "argcv_concurrent_test",
  srcs = glob([
    "concurrent/*_test.cc",
    "concurrent/*_test.h",
  ]),
  copts = [
    "-Wno-unused-function",
  ],
  deps = [
    "//external:glog",
    "//external:gtest_main",
    ":argcv_concurrent",
    ":gtest_ext",
  ],
)

cc_test(
  name = "argcv_nio_test",
  srcs = glob([
    "nio/*_test.cc",
    "nio/*_test.h",
  ]),
  deps = [
    "//external:glog",
    "//external:gtest_main",
    ":argcv_nio",
    # ":gtest_ext",
  ],
)
