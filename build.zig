const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "cirsi",
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(exe);
    exe.linkLibC();

    const include = std.build.LazyPath.relative("./include");
    exe.addIncludePath(include);

    exe.addCSourceFiles(&.{
        "src/main.c",
        "src/device.c",
        "src/universal.c",
        "src/world.c",
    }, &.{
        "-Wall",
        "-Wextra",
        "-Werror",
    });
}
