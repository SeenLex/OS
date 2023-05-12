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
#include "Reg.c"
#include "Sym.c"
#include "Dir.c"

void print_file_info(int noOfFiles, char *path[]){
    
    struct stat sb;
    char options[2];

    for(int i = 1; i <= noOfFiles; i++){          
        if(stat(path[i], &sb) == -1){
            
            perror("stat");
            exit(EXIT_FAILURE);
        }
        
        printf("\n");
        printf("---> Menu <---\n\n");
        printf("File name: %s\n", path[i]);
        printf("File type: ");
        
        if(S_ISREG(sb.st_mode)){
           
            printf("Regular file\n\n");
            printf("Options:\n");
            printf("\t-n: Name\n");
            printf("\t-d: Size\n");
            printf("\t-h: Hard link count\n");
            printf("\t-m: Time of last modification\n");
            printf("\t-a: Access rights\n");
            printf("\t-l: Create symbolic link (requires input)\n");

            printf("Enter option: ");
            scanf("%s", options);
            printf("\n");
            regular_file_options_selector(options, path[i]);
        
        } else if(S_ISLNK(sb.st_mode)){
            
            printf("Symbolic link\n");
            printf("Options:\n");
            printf("-n: Name\n");
            printf("-l: Delete symbolic link\n");
            printf("-d: Size of symbolic link\n");
            printf("-t: Size of target file\n");
            printf("-a: Access rights\n");

            printf("Enter option: ");
            scanf("%s", options);
           
            symlink_options_selector(options, path[i]);

        } else if(S_ISDIR(sb.st_mode)){
            
            printf("Directory\n");
            printf("\nOptions:\n");
            printf("-n: Name\n");
            printf("-d: Size\n");
            printf("-a: Access rights\n");
            printf("-c: Total number of files with .c extension\n");

            printf("\nEnter option: ");
            scanf("%s", options);
            printf("\n");
            directory_options_selector(options, path[i]);   

        } else{
            
            printf("Unknown\n");
            exit(EXIT_FAILURE);
        }
    }
}