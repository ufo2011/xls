load("//xls/build_rules:build_defs.bzl", "dslx_test")

package(
    default_visibility = ["//xls:xls_internal"],
    licenses = ["notice"],
)

cc_binary(
    name = "fft_reference",
    srcs = ["fft_reference.cc"],
    data = [
        "test_data/img_in",
        "test_data/img_twid_in",
        "test_data/real_in",
        "test_data/real_twid_in",
    ],
    deps = [
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "//xls/common:init_xls",
        "//xls/common/file:filesystem",
        "//xls/common/file:get_runfile_path",
        "//xls/common/logging",
        "//xls/common/status:status_macros",
    ],
)

dslx_test(
    name = "fft",
    srcs = ["fft.x"],
    # IR optimization uses too much memory.
    # TODO(https://github.com/google/xls/issues/354): 2021-03-23 Fix this.
    convert_ir = False,
    tags = ["optonly"],
    deps = [
        "test_data/dslx_test_data.x",
        "//xls/modules:apfloat_add_2.x",
        "//xls/modules:apfloat_mul_2.x",
        "//xls/modules:fpadd_2x32.x",
        "//xls/modules:fpmul_2x32.x",
        "//xls/modules:fpsub_2x32.x",
    ],
)
