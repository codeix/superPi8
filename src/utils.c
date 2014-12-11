#include <sys/timeb.h>
#include <sys/statvfs.h>


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

char* time_formatting(double input, char * output){
    int hours = input / 60 / 60;
    int minutes = (int)(input / 60) % 60;
    int seconds = (int)input % 60;
    snprintf(output, 50, "%i:%02i:%02i", hours, minutes, seconds);
    return output;
}

char* readable_fs(double size, char * output) {
    int i = 0;
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    snprintf(output, 50, "%.*f %s", i, size, units[i]);
    return output;
}

char* disk_left(char * output, char * path){
    struct statvfs buf;
    if (!statvfs(path, &buf)) {
        unsigned long long free;
        free = (long long)buf.f_bsize * (long long)buf.f_bavail;
        readable_fs((double)free, output);
    }
    return output;
}

double timestamp_mili(){
    struct timeb tmb;
    ftime(&tmb);
    return ((double)tmb.time) + ((double)tmb.millitm) / 1000.0;
}

