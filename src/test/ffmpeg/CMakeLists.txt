ADD_DEFINITIONS("-g")


aux_source_directory(. FFMPEG_SRC)
include_directories(${PROJECT_SOURCE_DIR}/third-parts/ffmpeg/ffmpeg-4.2.3/ffmpeg_build/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/third-parts/ffmpeg/ffmpeg-4.2.3/ffmpeg_build/lib)
SET(FFMPEG_LIB -lavformat -lavcodec -lavdevice -lavfilter -lavutil -lswresample -lswscale -ldl -lz -lm -lpthread -lx264 -lx265 -lva -lX11 -lva-drm -lva-x11 -lvdpau)

add_executable(ffmpeg_rtmp ${FFMPEG_SRC})
target_link_libraries(ffmpeg_rtmp ${FFMPEG_LIB})