include_directories(${CMAKE_SOURCE_DIR}/third-parts/opencv-4.4.0/build/include/opencv4)

link_directories(${CMAKE_SOURCE_DIR}/third-parts/opencv-4.4.0/build/lib)

add_executable(show_image show_image.cpp)

SET(OpenCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_videoio)

target_link_libraries(show_image ${OpenCV_LIBS})