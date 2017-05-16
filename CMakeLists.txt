cmake_minimum_required(VERSION 2.8)

project (latino-redis)

#add_subdirectory(hiredis)
include_directories(hiredis)
include_directories(hiredis/src)
#include_directories(include)

add_library(latino-redis SHARED src/latino-redis.c)

link_libraries(hiredis latino)

target_link_libraries(latino-redis hiredis latino)
#optional install in /usr/lib
install(FILES liblatino-redis.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
