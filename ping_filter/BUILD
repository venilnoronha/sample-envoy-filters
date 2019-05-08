package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_cc_test",
)

load(
    "@envoy_api//bazel:api_build_system.bzl",
    "api_proto_library",
)

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        ":ping_config",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)

envoy_cc_library(
    name = "ping_filter_lib",
    srcs = ["ping_filter.cc"],
    hdrs = ["ping_filter.h"],
    repository = "@envoy",
    deps = [
        "@envoy//include/envoy/network:connection_interface",
        "@envoy//include/envoy/network:filter_interface",
        "@envoy//source/common/buffer:buffer_lib",
        "@envoy//source/common/common:assert_lib",
        "@envoy//source/common/common:logger_lib",
    ],
)

api_proto_library(
    name = "ping_filter_proto",
    srcs = ["ping_filter.proto"],
)

envoy_cc_library(
    name = "ping_config",
    srcs = ["ping_config.cc"],
    hdrs = ["ping_config.h"],
    repository = "@envoy",
    deps = [
        ":ping_filter_lib",
        ":ping_filter_proto_cc",
        "@envoy//include/envoy/registry:registry",
        "@envoy//include/envoy/server:filter_config_interface",
        "@envoy//source/extensions/filters/network:well_known_names",
        "@envoy//source/extensions/filters/network/common:factory_base_lib",
    ],
)
