#include <newt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.c"



typedef struct Option {
    char* name;
    char* startby;
    char* time;
    char* length;
    int type;
} Option;


void mode_scan(){
    newtComponent form,
                  name_label, name_entry,
                  startby_label, startby_entry, 
                  time_label, time_entry,  
                  length_label, length_entry,
                  type_entry[2],
                  button_run, button_cancel;
    
    int rows, cols;
    int haserror = 0;
    Option option;

    while(1){
        newtCls();
        newtInit();
    
        if (haserror)
            newtPushHelpLine("Option are not valid");        

        newtGetScreenSize(&cols, &rows);
        newtOpenWindow((cols - 72)/2, 5, 72, 20, "Preparation to scan a super 8mm movie");

        name_label = newtLabel(1, 1, "Name of the Movie");
        name_entry = newtEntry(1, 2, "sample", 70, &option.name, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

        startby_label = newtLabel(1, 4, "Start movie by picture [default 0]");
        startby_entry = newtEntry(1, 5, "0", 70, &option.startby, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

        time_label = newtLabel(1, 7, "Start the scanner at e.g. \"8:20\" [default now]");
        time_entry = newtEntry(1, 8, "now", 70, &option.time, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

        length_label = newtLabel(1, 10, "Approximately length in meters");
        length_entry = newtEntry(1, 11, "60", 70, &option.length, NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT);

        type_entry[0] = newtRadiobutton(1, 13, "8mm", 1, NULL);
        type_entry[1] = newtRadiobutton(10, 13, "Super 8mm", 0, type_entry[0]);

        button_cancel = newtButton(19, 15, "Cancel");
        button_run = newtButton(44, 15, "Run");
        form = newtForm(NULL, NULL, 0);
        newtFormAddComponents(form, name_label, name_entry, startby_label, startby_entry,
                              time_label, time_entry, length_label, length_entry, NULL);

        int i;
        for (i = 0; i < 2; i++)
            newtFormAddComponent(form, type_entry[i]);

        newtFormAddComponents(form, button_cancel, button_run, NULL);

        struct newtExitStruct es; 
        newtFormRun(form, &es);
        newtFinished();
        
        for (i = 0; i < 2; i++)
            if (newtRadioGetCurrent(type_entry[0]) == type_entry[i])
                option.type = i;
        if (es.u.co == button_cancel){
            newtFormDestroy(form);
            return;
        }

        if (validate_user_input(option)){
            start_scanner(option);
            newtFormDestroy(form);
            return;
        }
        
        haserror = 1;
    }
}

int validate_user_input(Option option){
    if (strlen(option.name) < 1 || strlen(option.name) > 100)
        return 0;
    
    if (strlen(option.startby) < 1 || !is_integer(option.startby) || atoi(option.startby) < 0)
        return 0;

    if (strlen(option.length) < 1 || !is_integer(option.length) || atoi(option.length) < 1 || atoi(option.length) > 100)
        return 0;

    int time_result[3];
    split_time(option.time, time_result);

    if (time_result[0] == 1)
        return 0;

    return 1;
}

void start_scanner(Option option){

    int cols, rows;

    newtCls();
    newtInit();


    newtGetScreenSize(&cols, &rows);
    char title[120];
    strcpy(title, "Scanning: ");
    strcat(title, option.name);
    newtOpenWindow((cols - 72)/2, 5, 72, 20, title);

    newtComponent form = newtForm(NULL, NULL, 0);
    newtComponent scale_entry = newtScale(1, 17, 70, 10);

    newtComponent uptime_entry = newtTextbox(1, 1, 70, 10, NEWT_FLAG_WRAP);

    newtFormAddComponents(form, scale_entry, uptime_entry, NULL);


    time_t uptime = time(NULL);

    int i;
    for(i = 1; i< 10; i++){
        newtScaleSet(scale_entry, i);
        char hr_uptime[120];
        char output[50];
        time_t uptime_now = time(NULL);
        time_formatting((double)(uptime_now - uptime), output);
        snprintf(hr_uptime, 120, "Uptime: %s", output);
        newtTextboxSetText(uptime_entry, hr_uptime);
        newtDrawForm(form);
	    newtRefresh();
        sleep(1);
    }
    
    newtFormDestroy(form);
    newtFinished();
}

void mode_move(){
    
}


void mode_step(){
    
}
