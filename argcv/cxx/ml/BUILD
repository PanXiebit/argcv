cc_library(
  name = "util",
  srcs = glob(
    [
      "util/**/*.c",
      "util/**/*.cc",
    ],
    exclude = [
      "util/**/*_test.cc",
    ],
  ),
  hdrs = glob(
    [
      "util/**/*.h",
    ],
    exclude = [
      "util/**/*_test.h",
    ],
  ),
  deps = [
    "//external:glog",
  ],
)

cc_test(
  name = "util_test",
  srcs = glob([
    "util/**/*_test.cc",
    "util/**/*_test.h",
  ]),
  copts = [
    "-Wno-unused-function",
  ],
  deps = [
    "//external:glog",
    "//external:gtest_main",
    "//argcv/cxx:gtest_ext",
    ":util",
  ],
)