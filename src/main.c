#include <ncurses.h>
#include <stdlib.h>

void mode_quit(){
    refresh();
    endwin();
    exit(0);
}


void mode_scan(){
    
}


void mode_move(){

}


void mode_step(){

}


int main(void) {
    
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








