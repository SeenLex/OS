#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

int validation(char* options, char* valid_options[], int num_valid_options){

    int is_valid_option = 0;
    int i = 0;
    while(i < num_valid_options && !is_valid_option){
        if(strstr(options, valid_options[i])){
            is_valid_option = 1;
            return 1;
        }
        i++;
    }
    return -1;

    if(!is_valid_option){
        printf("Invalid option!");
    }
}