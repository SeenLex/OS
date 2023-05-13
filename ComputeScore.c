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

void get_score(char *filename){
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pscript = fork();
    if (pscript == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    char buf2[100];
    if(pscript == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        sprintf(buf2, "bash script.sh %s", filename);
        system(buf2);
        //execlp("script.sh", "script.sh", filename, NULL);
        //perror("execlp");
        exit(EXIT_FAILURE);
    
    } else{

        wait(NULL);
        close(pipefd[1]);

        char buffer[1024];
        ssize_t numRead;
        numRead = read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        if(numRead == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
    
    char buff[100];
    
    sprintf(buff, "bash script.sh %s", filename);
    system(buff);
    
    int firstValue;//errors
    int secondValue;//warnings
    
    FILE* file = fopen("info.txt", "r");
    if(file == NULL){
        printf("Failed to open the file.\n");
    }

    fscanf(file, "%d %d", &firstValue, &secondValue);
    fclose(file);

    
    int score = 0;
    
    if(firstValue == 0 && secondValue == 0){
        score = 10;
    }
    
    if(firstValue >= 1){
        score = 1;
    }
    
    if(firstValue == 0 && secondValue > 10){
        score = 2;
    }
    
    if(firstValue == 0 && secondValue <=10 ){
        score = 2 + 8 * (10 - secondValue) / 10;
    }



    FILE* file1 = fopen("grades.txt", "a");
    
    if(file1 == NULL){
        printf("Failed to open the file.\n");
    }

    fprintf(file1, "%s: %d\n", filename, score);
    fclose(file1);
    }
}
