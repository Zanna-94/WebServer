cmake_minimum_required(VERSION 3.3)
project(WebServer)

set(CMAKE_C_COMPILER "/usr/bin/gcc")

set(SOURCE_FILES lib/logging.c lib/lru-cache.c lib/io.c server.c lib/lock_fcntl.c sig_handler.c thread_job.c parser.c
        lib/ipc_sem.c constants.h http_response.c )

set(CMAKE_USE_PTHREADS_INIT true)
set(CMAKE_USE_PTHREADS_INIT ON)

find_package(Threads REQUIRED)

include_directories(lib)
include_directories(/usr/local/include)
include_directories(/usr/local/include/ImageMagick-7)

add_executable(server ${SOURCE_FILES})

target_link_libraries(server MagickWand-7.Q16HDRI)
target_link_libraries(server http_parser)
target_link_libraries(server Threads::Threads)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)