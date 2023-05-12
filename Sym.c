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
int pid_permission;

void change_permissions(char *filename){

    if((pid_permission = fork()) == 0){

        if(lchmod(filename, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP) == -1){
            perror("lchmod");
        
        } else{
            printf("Permissions of symbolic link '%s' changed successfully.\n", filename);
        }
    }     
}

void wait_for_process_sym(int pid){
    
    sleep(1);
    int status;
    waitpid(pid, &status, 0);
    printf("\nThe process with PID %d has ended with the exit code %d.", pid, status);
}

void print_access_rights_sym(char *path){
    
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

void symlink_options_selector(char *option, char *filename){
    
    if((pid_options = fork()) == 0){
        if(strcmp(option, "-n") == 0){
            printf("File name: %s\n", filename);
        
        } else if(strcmp(option, "-d") == 0){
            
            struct stat sb;
            
            if(stat(filename, &sb) == -1){
                perror("lstat");
                return;
            }
            
            printf("Size of symbolic link: %lld bytes\n", (long long)sb.st_size);
        
        } else if(strcmp(option, "-l") == 0){
            if(unlink(filename) == -1){
                perror("unlink");
            
            } else{
                printf("Symbolic link deleted successfully.\n");
            }
        
        } else if(strcmp(option, "-t") == 0){
            
            struct stat sb;
            
            if(stat(filename, &sb) == -1){
                perror("lstat");
                return;
            }
            
            if(S_ISLNK(sb.st_mode)){
                
                char target[256];
                ssize_t targetSize = readlink(filename, target, sizeof(target) - 1);
                
                if(targetSize == -1){
                    perror("readlink");
                
                } else{
                    target[targetSize] = '\0';
                    struct stat target_sb;
                    
                    if(stat(target, &target_sb) == -1){
                        perror("stat");
                    
                    } else{
                        printf("Size of target file: %lld bytes\n", (long long)target_sb.st_size);
                    }
                }
            
            } else{
                printf("Not a symbolic link.\n");
            }
        
        } else if(strcmp(option, "-a") == 0){
            print_access_rights_sym(filename);
        
        } else{
            printf("Invalid option: %s\n", option);
        }
        
        exit(pid_options);
    }

    wait_for_process_sym(pid_options);
    wait_for_process_sym(pid_permission);
}