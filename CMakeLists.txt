cmake_minimum_required(VERSION 3.16)

project(TaskNote VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES

    main.cpp

    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui

    src/core/PageID.h
    src/core/PageManager.h
    src/core/PageManager.cpp

    src/pages/Calendar/Calendar.ui
    src/pages/Calendar/Calendar.h
    src/pages/Calendar/Calendar.cpp
    src/pages/Calendar/CalendarCellWidget.h
    src/pages/Calendar/CalendarCellWidget.cpp

    src/pages/ToDoList/ToDoList.ui
    src/pages/ToDoList/ToDoList.h
    src/pages/ToDoList/ToDoList.cpp

    src/pages/Memo/Memo.ui
    src/pages/Memo/Memo.h
    src/pages/Memo/Memo.cpp

    src/pages/Setting.ui
    src/pages/Setting.h
    src/pages/Setting.cpp

    src/data/DatabaseManager.cpp
    src/data/DatabaseManager.h
)

qt_add_executable(TaskNote
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    src/data/ScheduleItem.h
    src/pages/ToDoList/ToDoListItemWidget.h src/pages/ToDoList/ToDoListItemWidget.cpp
    src/core/ThemeMode.h

)

target_link_libraries(TaskNote PRIVATE Qt6::Widgets Qt6::Sql)

target_include_directories(TaskNote PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pages
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pages/Calendar
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pages/ToDoList
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pages/Memo
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
)

set_target_properties(TaskNote PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(TaskNote)