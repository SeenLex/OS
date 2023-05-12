#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include "Menu.c"

#define MAX_OPTIONS 10

int main(int argc, char *argv[]){
    
    print_file_info(argc, argv);
    
    return 0;
}