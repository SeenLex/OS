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
#include "Validation.c"
#include "ComputeScore.c"

int pid_options;
int pid_extension;

void count_lines(char filename[]){

    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Failed to open the file '%s'.\n", filename);
        return;
    }

    int line_count = 1;
    char ch;
    
    while((ch = fgetc(file)) != EOF){
        if(ch == '\n'){
            line_count++;
        }
    }

    fclose(file);
    printf("The file '%s' has %d lines.\n", filename, line_count);
}

void verify_extension(char filename[]){

    if((pid_extension = fork()) == 0){
        char *extension = strrchr(filename, '.');
        if(extension && strcmp(extension, ".c") == 0){
            printf("The file '%s' has the '.c' extension.\n", filename);
            get_score(filename);
        } else{
            count_lines(filename);
        }
        exit(pid_extension);
    }
}

void wait_for_process(int pid){
    
    int status;
    
    sleep(0.5);
    waitpid(pid, &status, 0);
    printf("\nThe process with PID %d has ended with the exit code %d.", pid, status);
}

void print_access_rights(char *path){
    
    struct stat sb;

    if(stat(path, &sb) == -1){
        perror("stat");
        return;
    }

    printf("User:\n");
    printf("Read - %s\n", (sb.st_mode & S_IRUSR) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWUSR) ? "yes" : "no");
    printf("Exec - %s\n", (sb.st_mode & S_IXUSR) ? "yes" : "no");

    printf("\nGroup:\n");
    printf("Read - %s\n", (sb.st_mode & S_IRGRP) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWGRP) ? "yes" : "no");
    printf("Exec - %s\n", (sb.st_mode & S_IXGRP) ? "yes" : "no");

    printf("\nOthers:\n");
    printf("Read - %s\n", (sb.st_mode & S_IROTH) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWOTH) ? "yes" : "no");
    printf("Exec - %s\n\n", (sb.st_mode & S_IXOTH) ? "yes" : "no");
}

void regular_file_options_selector(char *filename){
    char option[2];
    char* valid_options[] = {"-n", "-d", "-h", "-m", "-a", "-l"};
    printf("Enter option: ");
    scanf("%s", option);
    printf("\n");
    if(validation(option,valid_options, 6)>0){

        if(strcmp(option, "-l") == 0){
                
            char symlinkName[256];
            
            printf("Enter the name of the symbolic link: ");
            scanf("%255s", symlinkName);
            
            if(symlink(filename, symlinkName) == -1){
                perror("symlink");
            
            }
            verify_extension(filename);
        }else{
            verify_extension(filename);
        }

        if((pid_options = fork()) == 0){
            if(strcmp(option, "-n") == 0){
                printf("\nThe name of the file is: %s\n\n", filename);
            
            } else if(strcmp(option, "-d") == 0){
                
                struct stat sb;
                
                if(stat(filename, &sb) == -1){
                    perror("stat");
                    return;
                }
                
                printf("\nFile size: %lld bytes\n\n", (long long)sb.st_size);
        
            } else if(strcmp(option, "-h") == 0){
                
                struct stat sb;
                
                if(stat(filename, &sb) == -1){
                    perror("stat");
                    return;
                }
                
                printf("\nHard link count: %ld\n\n", (long)sb.st_nlink);
            
            } else if(strcmp(option, "-m") == 0){
                
                struct stat sb;
                
                if(stat(filename, &sb) == -1){
                    perror("stat");
                    return;
                }
            
                printf("\nTime of last modification: %s\n\n", ctime(&sb.st_mtime));
            
            } else if(strcmp(option, "-a") == 0){
                
                print_access_rights(filename);
            } 
            
            exit(pid_options);
        }
        
        wait_for_process(pid_options);
        wait_for_process(pid_extension);
        printf("\n");
        } else{
            printf("Invalid option: %s\n\n", option);
            printf("Regular file\n\n");
            printf("Options:\n");
            printf("\t-n: Name\n");
            printf("\t-d: Size\n");
            printf("\t-h: Hard link count\n");
            printf("\t-m: Time of last modification\n");
            printf("\t-a: Access rights\n");
            printf("\t-l: Create symbolic link (requires input)\n");

            regular_file_options_selector(filename);
    }
}