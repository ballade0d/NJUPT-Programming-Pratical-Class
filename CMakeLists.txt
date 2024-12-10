cmake_minimum_required(VERSION 3.16)

project(demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
)

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        MainWindow.h MainWindow.cpp
        LoginWidget.h LoginWidget.cpp

        resource.qrc

        HomeWidget.h HomeWidget.cpp
        CalendarWindow.h CalendarWindow.cpp
        EditWindow.h EditWindow.cpp
        WordEditDialog.h
        WordAddDialog.h

        LearnWindow.h LearnWindow.cpp
        MultipleChoiceWindow.h MultipleChoiceWindow.cpp
        SpellingWindow.h SpellingWindow.cpp

)

# Link Qt and SQL
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)
