cmake_minimum_required(VERSION 2.8)

project(pi_sdl_test)

set(src_pi_sdl_test main_pi_sdl_test.cpp)

set( CMAKE_CXX_FLAGS "-g" )
set( CMAKE_C_FLAGS "-g"  )

find_package( OpenCV REQUIRED )
find_package( SDL REQUIRED )

add_executable( pi_sdl_test ${src_pi_sdl_test} )
include_directories( /usr/local/include ${SDL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIR} )
target_link_libraries( pi_sdl_test ${SDL_LIBRARY} ${OpenCV_LIBS} )



