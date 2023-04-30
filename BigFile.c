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

void print_file_info(const char *path) {
    struct stat sb;

    if (stat(path, &sb) == -1) {
        perror("stat");
        return;
    }

    printf("Name: %s\n", path);

    if (S_ISREG(sb.st_mode)) {
        printf("Type: regular file\n");
        printf("Size: %ld bytes\n", sb.st_size);
        printf("Hard link count: %ld\n", sb.st_nlink);
        printf("Time of last modification: %s", ctime(&sb.st_mtime));
        printf("Access rights:\n");
        print_access_rights(path);
        printf("Create symbolic link (-l):\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("Type: symbolic link\n");
        printf("Name: %s\n", path);
        printf("Size of symbolic link: %ld bytes\n", sb.st_size);
        printf("Size of target file: %ld bytes\n", sb.st_size);
        printf("Access rights:\n");
        print_access_rights(path);
        printf("Delete symbolic link (-l):\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("Type: directory\n");
        printf("Name: %s\n", path);
        printf("Size: %ld bytes\n", sb.st_size);
        printf("Access rights:\n");
        print_access_rights(path);
        printf("Total number of files with the .c extension (-c):\n");
    } else {
        printf("Unknown file type: %s\n", path);
    }
}

void compile_c_file(const char *path) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *argv[] = {"gcc", "-Wall", "-Wextra", "-pedantic", path, "-o", "out", NULL};
        execvp("gcc", argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                printf("Compilation failed.\n");
                exit(EXIT_FAILURE);
            }
        } else if (WIFSIGNALED(status)) {
            printf("Compilation terminated by signal.\n");
            exit(EXIT_FAILURE);
        }
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *argv[] = {"gcc", "-Wall", "-Wextra", "-pedantic", "-fsyntax-only", path, NULL};
        int fd = open("out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp("gcc", argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status == 0) {
                printf("Compilation successful.\n");
            } else if (exit_status > 0 && exit_status <= 3) {
                printf("Compilation completed with errors.\n");
            } else {
                printf("Compilation terminated by signal.\n");
            }

            int num_warnings = 0;
            int num_errors = 0;

            FILE *fp = fopen("out", "r");
            if (fp == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }

            char *line = NULL;
            size_t len = 0;
            ssize_t nread;

            while ((nread = getline(&line, &len, fp)) != -1) {
                if (strstr(line, "warning")) {
                    num_warnings++;
                } else if (strstr(line, "error")) {
                    num_errors++;
                }
            }

            fclose(fp);

            printf("Number of warnings: %d\n", num_warnings);
            printf("Number of errors: %d\n", num_errors);
        } else if (WIFSIGNALED(status)) {
            printf("Compilation terminated by signal.\n");
            exit(EXIT_FAILURE);
        }
    }
}