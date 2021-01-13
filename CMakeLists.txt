cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cuda-playground LANGUAGES CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cuda-playground main.cc)
