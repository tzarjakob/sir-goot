cmake_minimum_required(VERSION 3.20.0)
project(Game)
add_executable(${PROJECT_NAME}
    src/main.c
    src/aux.c
    src/data.c
    src/parser.c
)
target_link_libraries(${PROJECT_NAME}
    ncurses
    pthread
)

