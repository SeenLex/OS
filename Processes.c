#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


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