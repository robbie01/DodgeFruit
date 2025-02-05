cmake_minimum_required(VERSION 3.16.3)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

project(DodgeFruit)

file(GLOB SOURCES src/*.cpp)

add_executable(DodgeFruit ${SOURCES})
set_target_properties(DodgeFruit PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
target_link_libraries(DodgeFruit m)

if(MSVC)
    target_compile_options(DodgeFruit PRIVATE /W4 /WX)
else()
    target_compile_options(DodgeFruit PRIVATE -W -Wall -Wextra -pedantic)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} --preload-file sprites/")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    set_target_properties(DodgeFruit PROPERTIES OUTPUT_NAME "index")
else()
    find_package(SDL2 REQUIRED)
    if(MINGW)
        target_link_libraries(DodgeFruit SDL2::SDL2-static -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
    elseif(TARGET SDL2::SDL2)
        target_link_libraries(DodgeFruit SDL2::SDL2)
    else()
        include_directories(${SDL2_INCLUDE_DIRS})
        target_link_libraries(DodgeFruit ${SDL2_LIBRARIES})
    endif()
endif()

file(COPY sprites DESTINATION ${CMAKE_CURRENT_BINARY_DIR})