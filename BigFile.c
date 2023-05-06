#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include "Reg.c"
#include "Sym.c"
#include "Dir.c"

#define MAX_OPTIONS 10

void print_access_rights(const char *path) {
    struct stat sb;

    if (stat(path, &sb) == -1) {
        perror("stat");
        return;
    }

    printf("User:\n");
    printf("Read - %s\n", (sb.st_mode & S_IRUSR) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWUSR) ? "yes" : "no");
    printf("Exec - %s\n", (sb.st_mode & S_IXUSR) ? "yes" : "no");

    printf("Group:\n");
    printf("Read - %s\n", (sb.st_mode & S_IRGRP) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWGRP) ? "yes" : "no");
    printf("Exec - %s\n", (sb.st_mode & S_IXGRP) ? "yes" : "no");

    printf("Others:\n");
    printf("Read - %s\n", (sb.st_mode & S_IROTH) ? "yes" : "no");
    printf("Write - %s\n", (sb.st_mode & S_IWOTH) ? "yes" : "no");
    printf("Exec - %s\n", (sb.st_mode & S_IXOTH) ? "yes" : "no");
}

void print_file_info(int noOfFiles, const char *path[]) {
    
    struct stat sb;
    char options[2];

    for(int i = 1; i <= noOfFiles; i++){
        if (stat(path[i], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
        }
        printf("File name: %s\n", path[i]);
        printf("File type: ");
        switch (sb.st_mode & S_IFMT) {
            case S_IFREG:
                printf("Regular file\n");
                printf("Options:\n");
                printf("-n: Name\n");
                printf("-d: Size\n");
                printf("-h: Hard link count\n");
                printf("-m: Time of last modification\n");
                printf("-a: Access rights\n");
                printf("-l: Create symbolic link (requires input)\n");
                
                printf("Enter option: ");
                scanf("%s", options);
                regular_file_options_selector(options, path[i]);
                break;
            // case S_IFLNK:
            //     printf("Symbolic link\n");
            //     printf("Options:\n");
            //     printf("-n: Name\n");
            //     printf("-l: Delete symbolic link\n");
            //     printf("-d: Size of symbolic link\n");
            //     printf("-t: Size of target file\n");
            //     printf("-a: Access rights\n");

                // printf("Enter option: ");
                // scanf("%s", options);
                // symlink_options_selector(options, path[i]);
            //     break;
            case S_IFDIR:
                printf("Directory\n");
                printf("Options:\n");
                printf("-n: Name\n");
                printf("-d: Size\n");
                printf("-a: Access rights\n");
                printf("-c: Total number of files with .c extension\n");

                printf("Enter option: ");
                scanf("%s", options);
                directory_options_selector(options, path[i]);
                break;
            default:
                printf("Unknown\n");
                exit(EXIT_FAILURE);
        }
    }
}


int main(int argc, char *argv[]){
    
    //compile_c_file(argv[1]);
    print_file_info(argc, argv);
    return 0;
}
