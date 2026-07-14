include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(LostFoundTests
    test_ReportManager.cpp
    test_UserManager.cpp
    test_MatchingEngine.cpp

    ../src/models/ReportManager.cpp
    ../src/models/UserManager.cpp
    ../src/models/MatchingEngine.cpp
)

target_include_directories(LostFoundTests PRIVATE ../src/models)
target_link_libraries(LostFoundTests PRIVATE Qt6::Widgets gtest_main)

include(GoogleTest)
gtest_discover_tests(LostFoundTests)
