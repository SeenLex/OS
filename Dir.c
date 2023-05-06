#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


void directory_options_selector(char *option, char *filename) {
    if (strcmp(option, "-n") == 0) {
        printf("Directory name: %s\n", filename);
    } else if (strcmp(option, "-d") == 0) {
        struct stat sb;
        if (stat(filename, &sb) == -1) {
            perror("stat");
            return;
        }
        printf("Directory size: %lld bytes\n", (long long)sb.st_size);
    } else if (strcmp(option, "-a") == 0) {
        print_access_rights(filename);
    } else if (strcmp(option, "-c") == 0) {
        int count = 0;
        DIR *dir;
        struct dirent *entry;

        dir = opendir(filename);
        if (dir == NULL) {
            perror("opendir");
            return;
        }

        while ((entry = readdir(dir)) != NULL) {
            struct stat sb;
            char path[256];
            snprintf(path, sizeof(path), "%s/%s", filename, entry->d_name);
            if (stat(path, &sb) == -1) {
                perror("stat");
                continue;
            }
            if (S_ISREG(sb.st_mode)) {
                const char *ext = strrchr(entry->d_name, '.');
                if (ext != NULL && strcmp(ext, ".c") == 0) {
                    count++;
                }
            }
        }

        closedir(dir);
        printf("Total number of files with .c extension: %d\n", count);
    } else {
        printf("Invalid option: %s\n", option);
    }
}