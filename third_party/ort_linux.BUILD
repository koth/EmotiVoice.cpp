package(default_visibility = ["//visibility:public"])



cc_library(
  name = "ort_linux",
  hdrs = glob(["include/*.h"]),
  includes=[
    "include/",
  ],
  linkopts=[
    "-Lexternal/ort_linux/lib", 
    "-lonnxruntime",
  ],
  deps = [

  ],
)