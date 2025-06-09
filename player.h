#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "map.h"

#define X 0
#define Y 1

struct player {
    std::uint16_t position[2];
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

player* get_player() {
    static player player_ = []() {
        player p;
        p.position[X] = p.position[Y] = 1;
    
        return p;
    }();

    return &player_;
}


int read_input(player* player_) {
    char key;
    if (read(STDIN_FILENO, &key, 1) >= 0 ) {        
        if (key=='d') {
            player_->position[X] += 1;
        }
        else if (key == 'a') {
            player_->position[X] -= 1;
        }
        else if (key == 'w') {
            player_->position[Y] += 1;
        }
        else if (key == 's') {
            player_->position[Y] -= 1;
        }
        
        return 1;

    }
    return 0;
}

int update_player(player* player_, map* map_) {
    if ( read_input(player_) ) {
        map_->data[player_->position[Y]][player_->position[X]] = '0';
        return 1;
    }
    return 0;
}

void destroy_player() {
    delete get_player();
}

#endif /*PLAYER_H*/