#ifndef PLAYER_H
#define PLAYER_H

//#define DEBUG - Use compiler flag -D instead

#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "map.h"
#include "debug.h"
#include "constants.h"


struct player {
    std::uint16_t position[2];
    int symbol;
};


void set_nonblocking_input(bool enable) {
    static termios termios_old, termios_new;
    if (enable) {
        tcgetattr(STDIN_FILENO, &termios_old);
        termios_new = termios_old;
        termios_new.c_cflag;

        termios_new.c_lflag &= ~(ICANON | ECHO);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        tcsetattr(STDIN_FILENO, TCSANOW, &termios_new);
        return;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &termios_old);
}

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


int read_input(player* player_) {
    char key;
    if (read(STDIN_FILENO, &key, 1) >= 0 ) {        
        if (key=='d' && player_->position[X]<WIDTH-1) {
            player_->position[X] += 1;
        }
        else if (key == 'a'  && player_->position[X]>0) {
            player_->position[X] -= 1;
        }
        else if (key == 'w'  && player_->position[Y]>0) {
            player_->position[Y] -= 1;
        }
        else if (key == 's' && player_->position[Y]<HEIGHT-1) {
            player_->position[Y] += 1;
        }

        return 1;

    }
    return 0;
}

int update_player(player* player_) {
    if ( read_input(player_) ) {
        return 1;
    }
    return 0;
}

void destroy_player() {
    delete get_player();    
    DEBUG_PRINT("Player Destroyed");
}

#endif /*PLAYER_H*/