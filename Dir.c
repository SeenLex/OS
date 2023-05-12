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

int pid_options;
int pid_create;

void create_textfile(char *dirname){

    if((pid_create = fork()) == 0){
        
        char filename[256];
        
        snprintf(filename, sizeof(filename), "%s/%s_file.txt", dirname, dirname);

        FILE *file = fopen(filename, "w");
        if(file == NULL){
            printf("Failed to create the text file '%s'.\n", filename);
        }

        fprintf(file, "This is a sample text file created for directory '%s'.\n", dirname);
        fclose(file);
        printf("Text file '%s' created successfully.\n", filename);

        exit(pid_create);
    }
    
}

void wait_for_process_dir(int pid){
    
    sleep(0.5);
    int status;
    waitpid(pid, &status, 0);
    printf("\nThe process with PID %d has ended with the exit code %d.", pid, status);
}


void print_access_rights_dir(char *path){
   
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

void directory_options_selector(char *option, char *filename){
    
    create_textfile(filename);
    
    if((pid_options = fork()) == 0){
        
        if(strcmp(option, "-n") == 0){
            printf("\nDirectory name: %s\n\n", filename);
        
        } else if(strcmp(option, "-d") == 0){
            
            struct stat sb;
            
            if(stat(filename, &sb) == -1){
                perror("stat");
                return;
            }
            
            printf("\nDirectory size: %lld bytes\n\n", (long long)sb.st_size);
        
        } else if(strcmp(option, "-a") == 0){
            print_access_rights_dir(filename);
        
        } else if(strcmp(option, "-c") == 0){
            
            int count = 0;
            DIR *dir;
            struct dirent *entry;

            dir = opendir(filename);
            if(dir == NULL){
                perror("opendir");
                return;
            }

            while((entry = readdir(dir)) != NULL){
                
                struct stat sb;
                char path[500];
                
                snprintf(path, 500,"%s/%s", filename, entry->d_name);
                
                if(stat(path, &sb) == -1){
                    perror("stat");
                    continue;
                }
                
                if(S_ISREG(sb.st_mode)){
                    const char *ext = strrchr(entry->d_name, '.');
                    
                    if(ext != NULL && strcmp(ext, ".c") == 0){
                        count++;
                    }
                }
            }

            closedir(dir);
            printf("\nTotal number of files with .c extension: %d\n\n", count);
        
        } else{
           
            printf("\nInvalid option: %s\n\n", option);
            directory_options_selector(option, filename);
        }

        exit(pid_options);
    }

    wait_for_process_dir(pid_options);
    wait_for_process_dir(pid_create);
}