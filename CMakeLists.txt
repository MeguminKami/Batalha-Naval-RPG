cmake_minimum_required(VERSION 3.21)

project(BatalhaNavalRPG VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia Network)

set(APP_SOURCES
    src/app/main.cpp
    src/app/mainwindow.cpp
    src/app/mainwindow.h
    src/ui/mainwindow.ui

    src/audio/sounds.cpp
    src/audio/sounds.h

    src/game/board.cpp
    src/game/board.h
    src/game/character.cpp
    src/game/character.h
    src/game/cpu.cpp
    src/game/cpu.h
    src/game/objects.cpp
    src/game/objects.h
    src/game/passive.cpp
    src/game/passive.h
    src/game/player.cpp
    src/game/player.h
    src/game/position.h
    src/game/preview.cpp
    src/game/preview.h
    src/game/skill.cpp
    src/game/skill.h

    src/network/client.cpp
    src/network/client.h
    src/network/protocol.h
    src/network/server.cpp
    src/network/server.h

    assets/fonts/fonts.qrc
    assets/images/resource.qrc
    assets/sounds/sounds.qrc
)

qt_add_executable(BatalhaNavalRPG
    MANUAL_FINALIZATION
    ${APP_SOURCES}
)

target_include_directories(BatalhaNavalRPG PRIVATE
    src/app
    src/audio
    src/game
    src/network
)

target_link_libraries(BatalhaNavalRPG PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Network
)

set_target_properties(BatalhaNavalRPG PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS BatalhaNavalRPG
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

qt_finalize_executable(BatalhaNavalRPG)
