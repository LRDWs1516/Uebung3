cmake_minimum_required(VERSION 2.6)

project("LRD_Uebung3")

add_definitions(-std=c++11)

add_executable(lrd-ue3 sternsensor_Main.cpp catalog.h catalog.cpp catalogmanipulation.cpp catalogmanipulation.h bildmanipulation.cpp bildmanipulation.h)
