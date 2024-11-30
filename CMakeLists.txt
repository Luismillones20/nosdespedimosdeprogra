cmake_minimum_required(VERSION 3.27)
project(progra3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(nosdespedimosdeprogra
        menu.cpp
        proyecto_final.cpp
        Trie.cpp
        Memento.h
        Historial.h
        Archivo.h
        Movie.h)
