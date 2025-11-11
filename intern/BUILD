load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "intern",
    hdrs = ["intern.h"],
    deps = [
        "//intern/internal:hash_set",
        "//intern/internal:intern_helpers",
        "//intern/internal:rwlock",
    ],
)

cc_test(
    name = "intern_test",
    size = "small",
    srcs = ["intern_test.cc"],
    deps = [
        ":intern",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
