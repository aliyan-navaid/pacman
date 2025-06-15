#include "map.h"
#include "player.h"

#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define clear_screen() write(STDOUT_FILENO, "\033[2J\033[H", 7);


void set_nonblocking_input(bool enable) {
    static termios termios_old = []() {
        termios t;
        tcgetattr(STDIN_FILENO, &t);
        return t; 
    }();

    static termios termios_new;

    if (enable) {
        termios_new = termios_old;

        termios_new.c_lflag &= ~(ICANON | ECHO);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        tcsetattr(STDIN_FILENO, TCSANOW, &termios_new);
        return;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &termios_old);
}

void cleanup(int sig) {
    set_nonblocking_input(false);
    destroy_map();
    destroy_player();
    write( STDOUT_FILENO, CURSOR_UNHIDE, 6);

    exit(0);
}

int handle_input(map* map_, player* player_) {
    char key;
    if (read(STDIN_FILENO, &key, 1) >= 0 ) {
        map_->data[player_->position[Y]][player_->position[X]] = WALKABLE;

        int old_position[2] = {player_->position[X], player_->position[Y]};
        if (key=='d') {
            player_->position[X] += 1;
        }
        else if (key == 'a') {
            player_->position[X] -= 1;
        }
        else if (key == 'w') {
            player_->position[Y] -= 1;
        }
        else if (key == 's') {
            player_->position[Y] += 1;
        }

        if (!is_valid(player_->position[X], player_->position[Y], map_)) {
            player_->position[X] = old_position[X];
            player_->position[Y] = old_position[Y];
        }

        map_->data[player_->position[Y]][player_->position[X]] = player_->symbol;
            
        return 1;

    }
    return 0;
}

int main() {
    /* GAME ENTITIES */
    map* map = get_map();
    player* player = get_player();

    signal(SIGINT, cleanup);  // CTRL+C
    signal(SIGSEGV, cleanup);  // Segmentation Violation
    
    /* Set Terminal */    
    write( STDOUT_FILENO, CURSOR_HIDE, 6);
    set_nonblocking_input(true);

    /* GAME LOOP */
    display_map(map);
    while(1) {
        if (handle_input(map, player)) {
            clear_screen();
            display_map(map);
        }
    }
      
    return EXIT_SUCCESS;
}