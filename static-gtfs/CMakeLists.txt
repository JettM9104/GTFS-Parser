cmake_minimum_required(VERSION 3.16)
project(MyGTFSApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT: set your Qt install path on macOS
set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.10.1/macos")

# Find Qt modules (Widgets is enough for basic GUI)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(MyGTFSApp
    maingui.cpp
)

# Include your headers
target_include_directories(MyGTFSApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link Qt to your executable
target_link_libraries(MyGTFSApp PRIVATE Qt6::Widgets)