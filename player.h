#ifndef PLAYER_H
#define PLAYER_H

//#define DEBUG - Use compiler flag -D instead

#include <cstdint>
#include <unistd.h>
#include <fcntl.h>

#include "debug.h"
#include "constants.h"


struct player {
    std::uint16_t position[2];
    int symbol;
};

player* get_player(int initial_x=1, int initial_y=1) {
    static player* player_ = [&initial_x, &initial_y]() {
        player* p = new player;
        p->position[X] = initial_x; 
        p->position[Y] = initial_y;
        p->symbol = 2;

        return p;
    }(); // () invokes λ immediately

    return player_;
}

void set_player(player* player_, int x, int y) {
    player_->position[X] = x;
    player_->position[Y] = y;
}

void destroy_player() {
    delete get_player();    
    DEBUG_PRINT("Player Destroyed");
}

#endif /*PLAYER_H*/