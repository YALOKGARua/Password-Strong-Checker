cmake_minimum_required(VERSION 3.12)
project(PasswordChecker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v4.0.0
)

FetchContent_MakeAvailable(ftxui)

set(SOURCES
    ConfigManager.cpp
    Logger.cpp
    main.cpp
    PasswordChecker.cpp
    Utils.cpp
)

set(HEADERS
    ConfigManager.hpp
    Logger.hpp
    PasswordChecker.hpp
    Utils.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)