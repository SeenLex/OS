#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


void regular_file_options_selector(char *option, char *filename) {
    if (strcmp(option, "-n") == 0) {
        printf("File name: %s\n", filename);
    } else if (strcmp(option, "-d") == 0) {
        struct stat sb;
        if (stat(filename, &sb) == -1) {
            perror("stat");
            return;
        }
        printf("File size: %lld bytes\n", (long long)sb.st_size);
    } else if (strcmp(option, "-h") == 0) {
        struct stat sb;
        if (stat(filename, &sb) == -1) {
            perror("stat");
            return;
        }
        printf("Hard link count: %ld\n", (long)sb.st_nlink);
    } else if (strcmp(option, "-m") == 0) {
        struct stat sb;
        if (stat(filename, &sb) == -1) {
            perror("stat");
            return;
        }
        printf("Time of last modification: %s", ctime(&sb.st_mtime));
    } else if (strcmp(option, "-a") == 0) {
        print_access_rights(filename);
    } else if (strcmp(option, "-l") == 0) {
        // Create symbolic link (requires input)
    //     char target[256];
    //     printf("Enter the target path for the symbolic link: ");
    //     scanf("%255s", target);
    //     if (symlink(target, filename) == -1) {
    //         perror("symlink");
    //     } else {
    //         printf("Symbolic link created successfully.\n");
    //     }
    } else {
        printf("Invalid option: %s\n", option);
    }
}