cmake_minimum_required(VERSION 2.6)

project("LRD_Uebung3")

add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package (Threads)
add_executable(lrd-ue3 sternsensor_Main.cpp rotationsmatrix.h rotationsmatrix.cpp catalog.h catalog.cpp catalogmanipulation.cpp catalogmanipulation.h PointArray.cpp PointArray.h Image.cpp Image.h Points.cpp Points.h Color.cpp Color.h vectorCatalog.h vectorCatalog.cpp)
target_link_libraries (lrd-ue3 ${CMAKE_THREAD_LIBS_INIT})
