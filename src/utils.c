
int is_integer(char* input){
    while (*input){
        if(!isdigit(*input))
            return 0;
        input++;
    }
    return 1;
}


/* return array [{1=error,2=now,3=timer}, hour, minute]
 * 
*/
void split_time(char* input, int result[]){
    result[0] = 1;
    result[1] = 0;
    result[2] = 0;
    char * hour;
    char * minute;
    hour = strtok( input, ":" );
    minute = strtok(NULL, ":" );
    if (strcmp(hour, "now")==0){
        result[0] = 2;
        return;
    }
    if (minute == NULL ||
        hour == NULL ||
        !is_integer(minute) ||
        !is_integer(hour) ||
        atoi(minute) < 0 ||
        atoi(minute) > 59 ||
        atoi(hour) < 0 ||
        atoi(hour) > 23){
        return;
    }

    result[0] = 3;
    result[1] = atoi(minute);
    result[2] = atoi(hour);
}

void time_formatting(double input, char * output){
    int hours = input / 60 / 60;
    int minutes = (int)(input / 60) % 60;
    int seconds = (int)input % 60;
    snprintf(output, 50, "%i:%02i:%02i", hours, minutes, seconds);
}
