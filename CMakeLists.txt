cmake_minimum_required(VERSION 3.16)

project(demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate MySQL Connector/C++ (update path as necessary)
# set(MySQLConnectorCPP_INCLUDE_DIRS /opt/homebrew/Cellar/mysql-connector-c++/9.1.0/include)
# set(MySQLConnectorCPP_LIBRARIES /opt/homebrew/Cellar/mysql-connector-c++/9.1.0/lib/libmysqlcppconnx.dylib)
# include_directories(${MySQLConnectorCPP_INCLUDE_DIRS})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        LoginWidget.h LoginWidget.cpp
        LoginWidget.ui
        resource.qrc
        HomeWidget.ui
        HomeWidget.h
        HomeWidget.cpp
        CalendarWindow.cpp
        CalendarWindow.h
        EditWindow.cpp
        EditWindow.h

        WordEditDialog.h
        WordAddDialog.h
        LearnWindow.cpp
        LearnWindow.h
        MultipleChoiceWindow.cpp
        SpellingWindow.cpp
        MultipleChoiceWindow.h
        SpellingWindow.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET demo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(demo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(demo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt and MySQL Connector/C++
target_link_libraries(demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.demo)
endif()
set_target_properties(demo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(demo)
endif()
