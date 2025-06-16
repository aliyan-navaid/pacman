#ifndef MAP_H
#define MAP_H


#include <iostream>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <map>

#include "constants.h"
#include "debug.h"


struct map {
    std::int16_t data[HEIGHT][WIDTH] = {0};
    std::map< std::pair<int, int>, std::pair<int, int>> teleportations { 
        {{1,3}, {3,1}}
    };
};


void generate_map(map* map_) {
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
        map_->data[3][1] = TELEPORT;
    }
}

bool outofmap(std::uint16_t position, int axis){
    if (axis==X) return (position>WIDTH-1 || position<0 );
    else return (position<0 || position>HEIGHT-1); 
}

bool obstruction(std::uint16_t x, std::uint16_t y, map* map_) {
    return (map_->data[x][y] == WALL);
}

bool is_valid(int x, int y, map* map_) {
    return !( outofmap(x, X) || outofmap(y, Y) || obstruction(x, y, map_) );
}

bool is_teleport(int x, int y, map* map_) {
    auto it = map_->teleportations.find({x, y});
    return !(it==map_->teleportations.end());
}

std::pair<int, int> teleport_coordinates(int x, int y, map* map_) {
    return map_->teleportations.find({x,y})->second;
}

map* get_map() {
    static map* map_ = []() {
        map* m = new map;
        generate_map(m);

        return m;
    }();

    return map_;
}

void set_map(map* map_, int x, int y, int symbol) {
    map_->data[y][x] = symbol;
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
    else if (code==TELEPORT) {
        return "0";
    }
    return NULL;
}

void display_map(map* map_) {
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            const char* symbol = decode_symbol(map_->data[i][j]);
            write( STDOUT_FILENO, symbol, strlen(symbol) ) ;
        }
        write(STDOUT_FILENO, "\n", 1);
    }
}

void destroy_map() {
    delete get_map();
    DEBUG_PRINT("Map Destroyed");
    
}

#endif /*MAP_H*/