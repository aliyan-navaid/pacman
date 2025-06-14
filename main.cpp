#include "map.h"
#include "player.h"
#include <signal.h>


void cleanup(int sig) {
    set_nonblocking_input(false);
    destroy_map();
    destroy_player();
    write( STDOUT_FILENO, CURSOR_UNHIDE, 6);

    exit(0);
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
    display_map(map, player);
    while(1) {
        if ( update_player(player, map) )
            display_map(map, player);
    }
      
    return EXIT_SUCCESS;
}