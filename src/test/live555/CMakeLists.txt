include_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/groupsock/include)
include_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/UsageEnvironment/include)

link_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/BasicUsageEnvironment)
link_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/groupsock)
link_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/liveMedia)
link_directories(${CMAKE_SOURCE_DIR}/third-parts/live555/live/UsageEnvironment)

aux_source_directory(. LIVE555_TEST_SRC)
add_executable(live ${LIVE555_TEST_SRC})

target_link_libraries(live liveMedia BasicUsageEnvironment groupsock UsageEnvironment ssl crypto)