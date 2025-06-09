cmake_minimum_required(VERSION 3.14)
project(BinaryStringScanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(matplotplusplus)

# Найдем Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Список исходников
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    HelperFunctions.h
    HelperFunctions.cpp
    Params.h
    Params.cpp
    Generator.h
    Generator.cpp
    Reader.h
    Reader.cpp
    Writer.h
    Writer.cpp
)

# Создание исполняемого файла
add_executable(BinaryStringScanner ${SOURCES})

# Подключаем Qt и Matplot++
target_link_libraries(BinaryStringScanner
    PRIVATE
    Qt5::Widgets
    matplot
)

target_include_directories(BinaryStringScanner PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/matplotplusplus/source
    ${CMAKE_CURRENT_SOURCE_DIR}/matplotplusplus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/qt
)
