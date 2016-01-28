cmake_minimum_required(VERSION 2.8)

project(INTERFACE_BDDM CXX)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Test)
find_library(libvlc
             NAMES vlc
             HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_AUTOMOC ON)

set(interface_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/Analyser.cpp
    src/VLCPlayer.cpp
    src/VLCHandler.cpp
)

set(interface_HEADERS
    src/mainwindow.h
    src/Common.h
    src/Analyser.h
    src/VLCPlayer.h
    src/VLCHandler.h
)

set(interface_UI
    src/mainwindow.ui)

qt5_wrap_ui(UI_HEADERS ${interface_UI})
add_executable(interfaceBDDM ${interface_SOURCES} ${interface_HEADERS} ${UI_HEADERS} ${interface_UI})
target_link_libraries(interfaceBDDM ${Qt5Core_QTMAIN_LIBRARIES} ${libvlc})
include_directories(interfaceBBDM "include" ${CMAKE_CURRENT_SOURCE_DIR})
qt5_use_modules(interfaceBDDM Core Widgets Test)
