# Description:
#   NASM is a portable assembler in the Intel/Microsoft tradition.

licenses(["notice"])  # MIT

filegroup(
    name = "license",
    srcs = ["license.txt"],
)

cc_library(
    name = "rapidjson",
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)