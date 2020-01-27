include(ui/CMakeLists.txt)
include(text/CMakeLists.txt)
include(image/CMakeLists.txt)
include(audio/CMakeLists.txt)

set(mark_SRC
    main.cpp
    ${UI_SRC}
    ${TEXT_SRC}
    ${IMAGE_SRC}
    ${AUDIO_SRC}
)

add_executable(mark ${mark_SRC})

target_link_libraries(mark Qt5::Widgets)

install(TARGETS mark DESTINATION bin)
