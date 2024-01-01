package(
    default_visibility = ["//visibility:public"],
)


cc_library(
    name="ort_session",
    srcs=[
        "ort_session.cc",
    ],
    hdrs=[
        "ort_session.h",
    ],
    deps=[
        "@com_google_absl//absl/log:log",
        "@ort_linux//:ort_linux",

    ]
)