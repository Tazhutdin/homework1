# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(LibraryManagement)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаем исполняемый файл из этих исходников
add_executable(library
    src/main.cpp
    src/Book.cpp
    src/Library.cpp
    src/User.cpp
)

# Говорим компилятору, где искать заголовочные файлы (.h)
include_directories(src)