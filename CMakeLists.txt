cmake_minimum_required(VERSION 3.0)
project(marK)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set(mark_SRC
  src/main.cpp
  src/annotatorwidget.cpp
  src/mark.cpp
  src/polygon.cpp
  src/polygonclass.cpp
)

# Tell CMake to create the helloworld executable
add_executable(mark ${mark_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(mark Qt5::Widgets)

# Install the executable
install(TARGETS mark DESTINATION bin)
