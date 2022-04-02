cmake_minimum_required(VERSION 3.16.3)
project(Game)
add_executable(${PROJECT_NAME}
    src/main.c
    src/log.c
    src/data.c
    src/parser.c
    src/render.c
)
target_link_libraries(${PROJECT_NAME}
    ncurses
    pthread
)

