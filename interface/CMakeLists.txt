cmake_minimum_required(VERSION 2.8)

project(INTERFACE_BDDM CXX)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Test)
find_library(libvlc
             NAMES vlc)

set(CMAKE_AUTOMOC ON)

file(GLOB interface_SOURCES "src/*.cpp")
file(GLOB interface_HEADERS "src/*.h")
file(GLOB interface_UI      "src/*.ui")

qt5_wrap_ui(UI_HEADERS ${interface_UI})
add_executable(interfaceBDDM ${interface_SOURCES} ${interface_HEADERS} ${UI_HEADERS} ${interface_UI})
target_link_libraries(interfaceBDDM ${Qt5Core_QTMAIN_LIBRARIES} ${libvlc})
include_directories(interfaceBBDM ${CMAKE_CURRENT_SOURCE_DIR})
qt5_use_modules(interfaceBDDM Core Widgets Test)
