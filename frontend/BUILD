package(
    default_visibility = ["//visibility:public"],
)


cc_library(
    name = "an2cn",
    srcs=[
        "an2cn.cc",
        "an2cn.h",
    ],
)


cc_library(
    name = "pinyin_finder",
    srcs=[
        "pinyin_finder.cc",
        "pinyin_finder.h",
    ],
    deps=[
        "//utils:basic_string_util",
        "@com_google_absl//absl/log:log",
        "@com_google_absl//absl/log:check",
    ]
)

cc_test(
    name = "an2cn_test",
    srcs = ["an2cn_test.cc"],
    deps = [
        ":an2cn",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "pinyin_finder_test",
    srcs = ["pinyin_finder_test.cc"],
    deps = [
        ":pinyin_finder",
        "@gtest//:gtest_main",
    ],
)