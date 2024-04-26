cmake_minimum_required(VERSION 3.16)
project(TimeTrackingApp LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    #UI
    ui/TimeTrackingApp.ui
    ui/ProjectWidget.ui
    #SRC
    src/main.cpp
    src/TimeTrackingApp.cpp
    src/TimeTrackingApp.h
    src/ProjectWidget.cpp
    src/ProjectWidget.h
    src/ProjectData.cpp
    src/ProjectData.h
    src/Session.cpp
    src/Session.h
    src/utils.h
    src/utils.cpp
    src/AskSessionDataDialog.cpp
    src/AskSessionDataDialog.h
    #RESOURCES
    resources/Resources.qrc
)

qt_add_executable(TimeTrackingApp ${PROJECT_SOURCES})

if(WIN32)
    set_target_properties(TimeTrackingApp
        PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

target_link_libraries(TimeTrackingApp
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
