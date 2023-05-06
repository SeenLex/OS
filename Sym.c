#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


// void symlink_options_selector(char *option, char *filename) {
//     if (strcmp(option, "-n") == 0) {
//         printf("File name: %s\n", filename);
//     } else if (strcmp(option, "-d") == 0) {
//         struct stat sb;
//         if (lstat(filename, &sb) == -1) {
//             perror("lstat");
//             return;
//         }
//         printf("Size of symbolic link: %lld bytes\n", (long long)sb.st_size);
//     } else if (strcmp(option, "-l") == 0) {
//         if (unlink(filename) == -1) {
//             perror("unlink");
//         } else {
//             printf("Symbolic link deleted successfully.\n");
//         }
//     } else if (strcmp(option, "-t") == 0) {
//         struct stat sb;
//         if (lstat(filename, &sb) == -1) {
//             perror("lstat");
//             return;
//         }
//         if (S_ISLNK(sb.st_mode)) {
//             char target[256];
//             ssize_t targetSize = readlink(filename, target, sizeof(target) - 1);
//             if (targetSize == -1) {
//                 perror("readlink");
//             } else {
//                 target[targetSize] = '\0';
//                 struct stat target_sb;
//                 if (stat(target, &target_sb) == -1) {
//                     perror("stat");
//                 } else {
//                     printf("Size of target file: %lld bytes\n", (long long)target_sb.st_size);
//                 }
//             }
//         } else {
//             printf("Not a symbolic link.\n");
//         }
//     } else if (strcmp(option, "-a") == 0) {
//         print_access_rights(filename);
//     } else {
//         printf("Invalid option: %s\n", option);
//     }
// }