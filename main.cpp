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

        int new_position[2] = {player_->position[X], player_->position[Y]};
        if (key=='d') {
            new_position[X] += 1;
        }
        else if (key == 'a') {
            new_position[X] -= 1;
        }
        else if (key == 'w') {
            new_position[Y] -= 1;
        }
        else if (key == 's') {
            new_position[Y] += 1;
        }

        if (is_valid(new_position[X], new_position[Y], map_)) {
            if (is_teleport(new_position[X], new_position[Y], map_)) {
                int temp_x = teleport_coordinates(new_position[X], new_position[Y], map_).first;
                int temp_y = teleport_coordinates(new_position[X], new_position[Y], map_).second;

                new_position[X] = temp_x;
                new_position[Y] = temp_y;
            }
            set_player(player_, new_position[X], new_position[Y]);
        }
        
        set_map(map_, player_->position[X], player_->position[Y], player_->symbol);
        
        return 1;

    }
    return 0;
}

int main() {
    /* GAME ENTITIES */
    map* map = get_map();
    player* player = get_player();

    struct sigaction sa;
    sa.sa_handler = cleanup;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if ( sigaction(SIGINT, &sa, nullptr) < 0) { // CTRL+C
        write(STDERR_FILENO, "Signal Handler Failed\n", 22);
        return -1;
    }  
    
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