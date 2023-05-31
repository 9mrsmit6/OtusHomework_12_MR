cmake_minimum_required(VERSION 3.5)

project(OtusHomework_12_MR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( Threads )

add_executable(OtusHomework_12_MR
    Utils/BlockManager.hpp
    Distributor.hpp
    Map/Map.hpp
    Shaffle/Shaffle.hpp

    main.cpp
)


target_link_libraries(OtusHomework_12_MR ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS OtusHomework_12_MR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
