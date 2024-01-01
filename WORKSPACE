workspace(name = "emotivoice_cpp")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")


# Bazel Skylib.
http_archive(
    name = "bazel_skylib",  # 2022-11-16T18:29:32Z
    sha256 = "a22290c26d29d3ecca286466f7f295ac6cbe32c0a9da3a91176a90e0725e3649",
    strip_prefix = "bazel-skylib-5bfcb1a684550626ce138fe0fe8f5f702b3764c3",
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/5bfcb1a684550626ce138fe0fe8f5f702b3764c3.zip"],
)


http_archive(
        name = "com_google_absl",
        urls = ["https://github.com/abseil/abseil-cpp/archive/20230802.1.tar.gz"],
        strip_prefix = "abseil-cpp-20230802.1",
)


http_archive(
        name = "gtest",
        urls = ["https://github.com/google/googletest/archive/v1.14.0.tar.gz"],
        strip_prefix = "googletest-1.14.0",
)



http_archive(
        name = "utfcpp",
        build_file = "@emotivoice_cpp//third_party:utfcpp.BUILD",
        strip_prefix = "utfcpp-3.1",
        sha256 = "ab531c3fd5d275150430bfaca01d7d15e017a188183be932322f2f651506b096",
        urls = [
            "https://github.com/nemtrif/utfcpp/archive/v3.1.tar.gz",
        ]
)


http_archive(
        name = "utf8proc",
        build_file = "@emotivoice_cpp//third_party:utf8proc.BUILD",
        strip_prefix = "utf8proc-2.4.0",
        sha256 = "b2e5d547c1d94762a6d03a7e05cea46092aab68636460ff8648f1295e2cdfbd7",
        urls = [
            "https://github.com/JuliaStrings/utf8proc/archive/v2.4.0.tar.gz",
        ]
)

http_archive(
        name = "cppjieba",
        build_file = "@emotivoice_cpp//third_party:cppjieba.BUILD",
        strip_prefix = "cppjieba-5.1.0",
        patch_cmds=[
            """cat >test/BUILD<<EOL
package(default_visibility = ["//visibility:public"])
cc_binary(
    name="demo",
    srcs=["demo.cpp"],
    deps=[
        '//:cppjieba',
    ],
    data=[
        '//:cppjieba_dict',
    ]
)
"""
        ],
        # sha256 = "",
        urls = [
            "https://github.com/yanyiwu/cppjieba/archive/v5.1.0.tar.gz",
        ]
)


http_archive(
        name = "limonp",
        build_file = "@emotivoice_cpp//third_party:limonp.BUILD",
        strip_prefix = "limonp-a269e34dc4948d5a9209e21a7887b52daa0d3e78",
        # sha256 = "",
        urls = [
            "https://github.com/yanyiwu/limonp/archive/a269e34dc4948d5a9209e21a7887b52daa0d3e78.tar.gz",
        ]
)

http_archive(
        name = "ort_linux",
        build_file = "@emotivoice_cpp//third_party:ort_linux.BUILD",
        strip_prefix = "onnxruntime-linux-x64-1.14.1",
        integrity = "sha256-AQWF9TTYIr8C1Ux8/FO1eqPrDPNPj7dQPuUZ7RjiXSk=",
        urls = [
            "https://github.com/microsoft/onnxruntime/releases/download/v1.14.1/onnxruntime-linux-x64-1.14.1.tgz",
        ]
)
