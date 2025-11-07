load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

package(default_visibility = ["//intern:__pkg__"])

cc_library(
    name = "intern_helpers",
    srcs = ["intern_helpers.c"],
    hdrs = ["intern_helpers.h"],
)

cc_test(
    name = "intern_helpers_test",
    size = "small",
    srcs = ["intern_helpers_test.cc"],
    deps = [
        ":intern_helpers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "hash_set",
    hdrs = ["hash_set.h"],
)

cc_test(
    name = "hash_set_test",
    size = "small",
    srcs = ["hash_set_test.cc"],
    deps = [
        ":hash_set",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
