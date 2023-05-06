#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_OPTIONS 10

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

// void compile_c_file(const char *path) {
//     pid_t pid = fork();

//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     } else if (pid == 0) {
//         char *argv[] = {"gcc", "-Wall", "-Wextra", "-pedantic", path, "-o", "out", NULL};
//         execvp("gcc", argv);
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status)) {
//             int exit_status = WEXITSTATUS(status);
//             if (exit_status != 0) {
//                 printf("Compilation failed.\n");
//                 exit(EXIT_FAILURE);
//             }
//         } else if (WIFSIGNALED(status)) {
//             printf("Compilation terminated by signal.\n");
//             exit(EXIT_FAILURE);
//         }
//     }

//     pid = fork();

//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     } else if (pid == 0) {
//         char *argv[] = {"gcc", "-Wall", "-Wextra", "-pedantic", "-fsyntax-only", path, NULL};
//         int fd = open("out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd == -1) {
//             perror("open");
//             exit(EXIT_FAILURE);
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//         execvp("gcc", argv);
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status)) {
//             int exit_status = WEXITSTATUS(status);
//             if (exit_status == 0) {
//                 printf("Compilation successful.\n");
//             } else if (exit_status > 0 && exit_status <= 3) {
//                 printf("Compilation completed with errors.\n");
//             } else {
//                 printf("Compilation terminated by signal.\n");
//             }

//             int num_warnings = 0;
//             int num_errors = 0;

//             FILE *fp = fopen("out", "r");
//             if (fp == NULL) {
//                 perror("fopen");
//                 exit(EXIT_FAILURE);
//             }

//             char *line = NULL;
//             size_t len = 0;
//             ssize_t nread;

//             while ((nread = getline(&line, &len, fp)) != -1) {
//                 if (strstr(line, "warning")) {
//                     num_warnings++;
//                 } else if (strstr(line, "error")) {
//                     num_errors++;
//                 }
//             }

//             fclose(fp);

//             printf("Number of warnings: %d\n", num_warnings);
//             printf("Number of errors: %d\n", num_errors);
//         } else if (WIFSIGNALED(status)) {
//             printf("Compilation terminated by signal.\n");
//             exit(EXIT_FAILURE);
//         }
//     }
// }


int main(int argc, char *argv[]){
    
    //compile_c_file(argv[1]);
    print_file_info(argc, argv);
    return 0;
}
