qt5_add_resources(mark_SRCS resources.qrc)
add_executable(mark main.cpp ${mark_SRCS})
target_link_libraries(mark Qt5::Qml Qt5::Gui)

