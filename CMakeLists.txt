cmake_minimum_required(VERSION 3.5)
project(runtime_license_audit)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_GNU_SOURCE ")

set(SOURCE_FILES license_audit.c)

add_library(runtime-license-audit SHARED ${SOURCE_FILES})

set(TEST_FILES test.c)
set(TESTLIB_FILES testlib.c)

add_library(testlib SHARED ${TESTLIB_FILES})
add_executable(runtime-license-audit-test ${TEST_FILES})
target_link_libraries(runtime-license-audit-test dl)