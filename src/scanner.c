#include <newt.h>
#include <stdlib.h>
#include <stdio.h>
   

void mode_scan(){
    newtComponent form, name_label, startby_label, time_label, name_entry, startby_entry, time_entry, button;
    char * name_value, startby_value, time_value;

    newtInit();
    newtCls();

    newtOpenWindow(20, 5, 80, 20, "Preparation to scan a super 8mm movie");

    name_label = newtLabel(1, 1, "Name of the Movie");
    name_entry = newtEntry(1, 2, "sample", 70, &name_value, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

    startby_label = newtLabel(1, 4, "Start movie by picture [default 0]");
    startby_entry = newtEntry(1, 5, "0", 70, &startby_value, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

    time_label = newtLabel(1, 7, "Start the scanner at e.g. \"8:20\" [default now]");
    time_entry = newtEntry(1, 8, "", 70, &time_value, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);


    button = newtButton(30, 12, "Run");
    form = newtForm(NULL, NULL, 0);
    newtFormAddComponents(form, name_label, name_entry, startby_label, startby_entry, time_label, time_entry, button, NULL);

    newtRunForm(form);
    newtFinished();
    newtFormDestroy(form);
}


void mode_move(){
    
}


void mode_step(){
    
}
