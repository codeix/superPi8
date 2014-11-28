#include <ncurses.h>
#include <stdlib.h>
#include "scanner.h"

void mode_quit(){
    refresh();
    endwin();
    exit(0);
}


int main(void) {
    
    if(gpio_init()){
        mode_quit();    
    }

    WINDOW *win = initscr();			
    char usermode[10];
    bool error = 0;
    while(1){
        printw("Written by Samuel Riolo 2014\n");
        printw("Raspberry PI\n");
        printw("RPI GPIO\n\n");
        printw("Choose a mode: step, move, scan or quit\n");
        if (error)
            printw("Command not found! ");
        error = 0;        
        printw( "> ");
        getstr(usermode);

        if (strcmp(usermode, "scan")==0) {
            mode_scan();
        }
        else if (strcmp(usermode, "step")==0) {
            mode_step();
        }
        else if (strcmp(usermode, "move")==0) {
            printf("Press spacebar to move or ESC to come back to main menu");
            mode_move();
        }
        else if (strcmp(usermode, "quit")==0) {
            mode_quit();
        }
        else  {
            error = 1;
        }
        clear();

    }

    return 0;
}








