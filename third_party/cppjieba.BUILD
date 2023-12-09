package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cppjieba",
    srcs = glob([
        "include/cppjieba/*.h",
    ]) ,
    includes = [
        "include",
    ],
    deps=[
        "@limonp"
    ]
)
filegroup(
    name = "cppjieba_dict",
    srcs = glob([
        "dict/*.utf8",
    ]),
)
