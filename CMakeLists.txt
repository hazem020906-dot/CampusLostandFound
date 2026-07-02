cmake_minimum_required(VERSION 3.21)
project(CampusLostAndFound)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(CampusLostAndFound
    src/main.cpp

    # Models
    src/models/ItemReport.hpp
    src/models/ReportManager.hpp
    src/models/ReportManager.cpp
    src/models/User.hpp
    src/models/UserManager.hpp
    src/models/UserManager.cpp

    # UI
    src/ui/LoginWindow.hpp
    src/ui/LoginWindow.cpp
    src/ui/StudentDashboard.hpp
    src/ui/StudentDashboard.cpp
    src/ui/Gate4Dashoboard.hpp
    src/ui/Gate4Dashoboard.cpp
)

target_link_libraries(CampusLostAndFound PRIVATE Qt6::Widgets)
