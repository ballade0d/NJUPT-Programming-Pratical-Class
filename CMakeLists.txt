cmake_minimum_required(VERSION 3.16)

project(demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        src/main.cpp
)

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/windows/MainWindow.h src/windows/MainWindow.cpp
        src/widgets/LoginWidget.h src/widgets/LoginWidget.cpp

        resource/resource.qrc

        src/widgets/HomeWidget.h src/widgets/HomeWidget.cpp
        src/windows/CalendarWindow.h src/windows/CalendarWindow.cpp
        src/windows/EditWindow.h src/windows/EditWindow.cpp
        src/windows/WordEditDialog.h
        src/windows/WordAddDialog.h

        src/windows/LearnWindow.h src/windows/LearnWindow.cpp
        src/windows/MultipleChoiceWindow.h src/windows/MultipleChoiceWindow.cpp
        src/windows/SpellingWindow.h src/windows/SpellingWindow.cpp

)

# Link Qt and SQL
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)
