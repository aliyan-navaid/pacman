#ifndef MAP_H
#define MAP_H

#define clear_screen() write(STDOUT_FILENO, "\033[2J\033[H", 7);

#include <iostream>
#include <cstdint>
#include <cstring>
#include <unistd.h>

#include "constants.h"
#include "debug.h"
#include "player.h"


struct map {
    std::int16_t data[HEIGHT][WIDTH] = {0};
};


void generate_map(map* map_) {
/*
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Walls around border
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                map_->data[y][x] = WALL;
            }
            else if ((x % 8 == 0) || (y % 8 == 0)) {
                map_->data[y][x] = WALL;
            }
            else {
                map_->data[y][x] = WALKABLE;
            }
        }
    }
*/
}

map* get_map() {
    static map* map_ = []() {
        map* m = new map;
        generate_map(m);

        return m;
    }();

    return map_;
}

const char* decode_symbol(std::uint16_t code) {
    if (code==WALL) {
        return "■";
    }
    else if (code==WALKABLE) {
        return " ";
    }
    else if (code==PLAYER) {
        return "@";
    }
    return NULL;
}

void display_map(map* map_, player* player_) {
    clear_screen();
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            //std::cout << i << " " << j << " \n";
            if (player_->position[X]==j && player_->position[Y]==i) {
                write( STDOUT_FILENO, decode_symbol(player_->symbol), 1 ); 
                continue;   
            }
            write( STDOUT_FILENO, decode_symbol(map_->data[i][j]), strlen(decode_symbol(map_->data[i][j])) ) ;
        }
        write(STDOUT_FILENO, "\n", 1);
    }
}


void destroy_map() {
    delete get_map();
    DEBUG_PRINT("Map Destroyed");
}

#endif /*MAP_H*/