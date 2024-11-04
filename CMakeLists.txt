cmake_minimum_required(VERSION 3.29)
project(fruit_samurai C)

set(CMAKE_C_STANDARD 11)

add_executable(fruit_samurai main.c
        RenderWindow.h
        RenderWindow.c
        Entity.h
        Entity.c
        Vector2f.c
        Vector2f.h
        Utils.h
        Utils.c)
target_link_libraries(fruit_samurai -lSDL2 -lSDL2_image -lSDL2_ttf)