# Environment variables
set(ENV{BOARD} native_posix_64)

set(ENV{ZEPHYR_TOOLCHAIN_VARIANT} cross-compile)
set(ENV{CROSS_COMPILE} host)

# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(my_zephyr_app)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
target_sources(app PRIVATE src/main.c)