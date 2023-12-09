package(default_visibility = ["//visibility:public"])


cc_library(
    name="basic_string_util",
    srcs=["basic_string_util.h"],
    deps=[
        "@com_google_absl//absl/log:check",
    ]
)

cc_library(
    name="bert_tokenizer",
    srcs=[
        "bert_tokenizer.h",
        "bert_tokenizer.cc",
    ],
    deps=[
        ":basic_string_util",
        "@com_google_absl//absl/log:log",
        "@utf8proc",
        "@utfcpp",
    ],

)
