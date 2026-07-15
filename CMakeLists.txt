cmake_minimum_required(VERSION 3.21)
project(CampusLostAndFound)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)




add_executable(CampusLostAndFound
    src/main.cpp
    # Client networking
    src/client/NetworkClient.hpp
    src/client/NetworkClient.cpp



    # Models
    src/models/ItemReport.hpp
    src/models/ReportManager.hpp
    src/models/ReportManager.cpp
    src/models/User.hpp
    src/models/UserManager.hpp
    src/models/UserManager.cpp

    # Models - Feedback, Messaging, Reward features (Jana)
    src/models/Feedback.hpp
    src/models/FeedbackManager.hpp
    src/models/FeedbackManager.cpp
    src/models/Message.hpp
    src/models/MessageManager.hpp
    src/models/MessageManager.cpp
    src/models/RewardOffer.hpp
    src/models/RewardManager.hpp
    src/models/RewardManager.cpp

    # Models - Matching engine (Jana)
    src/models/MatchingEngine.hpp
    src/models/MatchingEngine.cpp

    # UI
    src/ui/LoginWindow.hpp
    src/ui/LoginWindow.cpp
    src/ui/StudentDashboard.hpp
    src/ui/StudentDashboard.cpp
    src/ui/Gate4Dashoboard.hpp
    src/ui/Gate4Dashoboard.cpp

    # UI - Feedback, Messaging, Statistics, Reward features (Jana)
    src/ui/FeedbackWindow.hpp
    src/ui/FeedbackWindow.cpp
    src/ui/MessagingWindow.hpp
    src/ui/MessagingWindow.cpp
    src/ui/StatisticsWindow.hpp
    src/ui/StatisticsWindow.cpp
    src/ui/RewardWindow.hpp
    src/ui/RewardWindow.cpp
)
add_executable(LostFoundServer
    src/server/main.cpp
    src/server/Server.hpp
    src/server/Server.cpp

    src/models/ItemReport.hpp
    src/models/ReportManager.hpp
    src/models/ReportManager.cpp

    src/models/User.hpp
    src/models/UserManager.hpp
    src/models/UserManager.cpp

    src/models/MatchingEngine.hpp
    src/models/MatchingEngine.cpp
)

target_link_libraries(CampusLostAndFound PRIVATE
    Qt6::Widgets
    Boost::system
    nlohmann_json::nlohmann_json
    Threads::Threads
)

target_link_libraries(LostFoundServer PRIVATE
    Qt6::Core
    Boost::system
    nlohmann_json::nlohmann_json
    Threads::Threads
)

# Unit tests
option(BUILD_TESTS "Build the GoogleTest unit test suite" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
