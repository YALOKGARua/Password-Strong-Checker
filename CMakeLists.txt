cmake_minimum_required(VERSION 3.12)
project(PasswordChecker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(jsoncpp REQUIRED)

set(SOURCES
    ConfigManager.cpp
    Logger.cpp
    main.cpp
    MainWindow.cpp
    PasswordChecker.cpp
    TestSuite.cpp
    Utils.cpp
)

set(HEADERS
    ConfigManager.hpp
    Logger.hpp
    MainWindow.hpp
    PasswordChecker.hpp
    TestSuite.hpp
    Utils.hpp
)

set(UI_FILES
    MainWindow.xml
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    jsoncpp_lib
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
) 