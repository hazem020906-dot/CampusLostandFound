cmake_minimum_required(VERSION 3.21)
project(CampusLostAndFound)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(CampusLostAndFound
    src/main.cpp
    src/models/ItemReport.hpp
    src/models/ReportManager.hpp
    src/models/ReportManager.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
)

target_link_libraries(CampusLostAndFound PRIVATE Qt6::Widgets)
