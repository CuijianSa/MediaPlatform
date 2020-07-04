add_executable(log_example log_example.cc)
include_directories(${CMAKE_SOURCE_DIR}/utils/log/include)
target_link_libraries(log_example util_log)