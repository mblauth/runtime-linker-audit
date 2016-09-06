cmake_minimum_required(VERSION 3.5)
project(runtime_linker_audit)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_GNU_SOURCE")
set(SOURCE_FILES src/auditlib.c)

include_directories(SYSTEM /usr/include
                    include)

add_library(audit SHARED ${SOURCE_FILES})
target_link_libraries(audit dl)

set(TEST_FILES src/test.c)
set(TESTLIB_FILES src/testlib.c)

add_executable(auditor src/auditor.c src/rpm_checker.c)
target_link_libraries(auditor rpm hawkey)

add_library(testlib SHARED ${TESTLIB_FILES})
add_executable(runtime-linker-audit-test ${TEST_FILES})
target_link_libraries(runtime-linker-audit-test dl)