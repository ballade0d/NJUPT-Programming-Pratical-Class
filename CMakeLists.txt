cmake_minimum_required(VERSION 3.16)

project(demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

set(PROJECT_SOURCES
        src/main.cpp
)

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/dialog/WordEditDialog.h
        src/dialog/WordAddDialog.h
        src/dialog/BookAddDialog.h
        src/dialog/BookDeleteDialog.h

        src/widgets/LoginWidget.h src/widgets/LoginWidget.cpp
        src/widgets/HomeWidget.h src/widgets/HomeWidget.cpp

        src/windows/MainWindow.h src/windows/MainWindow.cpp

        src/windows/recite/MultipleChoiceWindow.h src/windows/recite/MultipleChoiceWindow.cpp
        src/windows/recite/SpellingWindow.h src/windows/recite/SpellingWindow.cpp
        src/windows/recite/SettlementWindow.h src/windows/recite/SettlementWindow.cpp

        src/windows/CalendarWindow.h src/windows/CalendarWindow.cpp
        src/windows/EditWindow.h src/windows/EditWindow.cpp
        src/windows/LearnWindow.h src/windows/LearnWindow.cpp
        src/windows/RecordBookWindow.h src/windows/RecordBookWindow.cpp

        resource/resource.qrc
        src/entity/Book.h
        src/mapper/BookMapper.h
        src/entity/Word.h
        src/mapper/WordMapper.h
        src/mapper/RecordMapper.h
        src/mapper/CheckInMapper.h
        src/mapper/UserMapper.h
        src/entity/User.h
)

# Link Qt and SQL
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql Qt6::Charts)
