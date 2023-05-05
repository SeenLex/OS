#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

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
                break;
            // case S_IFLNK:
            //     printf("Symbolic link\n");
            //     printf("Options:\n");
            //     printf("-n: Name\n");
            //     printf("-l: Delete symbolic link\n");
            //     printf("-d: Size of symbolic link\n");
            //     printf("-t: Size of target file\n");
            //     printf("-a: Access rights\n");
            //     break;
            case S_IFDIR:
                printf("Directory\n");
                printf("Options:\n");
                printf("-n: Name\n");
                printf("-d: Size\n");
                printf("-a: Access rights\n");
                printf("-c: Total number of files with .c extension\n");
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


int main(int argc, char const *argv[]){
    
    
    print_file_info(argc, argv);
    return 0;
}
