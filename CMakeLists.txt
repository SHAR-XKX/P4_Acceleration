cmake_minimum_required(VERSION 3.4)
project(pybind_vid)
add_subdirectory(pybind11)
pybind11_add_module(pre_processing_acc pre_processing.cpp)
