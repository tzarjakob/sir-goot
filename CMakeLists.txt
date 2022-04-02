cmake_minimum_required(VERSION 3.16.3)
project(Game)
add_executable(${PROJECT_NAME}
    src/main.c
    src/log.c
    src/data.c
    src/game_logic.c
    src/parsing/parser.c
    src/parsing/loading.c 
    src/parsing/aux.c
    src/render/screen.c 
    src/render/movements.c 
)
target_link_libraries(${PROJECT_NAME}
    ncurses
    pthread
)

