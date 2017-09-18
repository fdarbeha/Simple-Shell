//
//  main.c
//  Shell
//
//  Created by fatemeh darbehani on 2016-01-26.
//  Copyright © 2016 DEVELOPED BY SFU COMPUTER ENGINEERING STUDENTS. All rights reserved.
//
// No Command not handled! //
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//Function Protocols
int shellLoop();
char** tokenizeLine(char* cmd);
int launchCmd(char** args);
//Global Variables
time_t raw_time;

int main(int argc, char** argv)
{
    //Set up Phase
    
    time(&raw_time);
    printf("Shell started on %s", asctime(localtime(&raw_time)));
    
    shellLoop();
    
    //Clean Phase
    
    return 0;
}

int shellLoop()
{
    do{
        time(&raw_time);
        char* current_time = asctime(localtime(&raw_time));
        current_time[strlen(current_time) - 1] = '\0';
        printf("[fdarbehani][%s]# ", current_time);
        
        char* buffer = NULL;
        size_t bufferSize = 0;
        getline(&buffer,&bufferSize, stdin);
            
        char** tokens = tokenizeLine(buffer);
        //------------------ Launch the command -------------------//
        if (strcmp(tokens[0], "exit") == 0){
            free(tokens);
            free(buffer);
            return 0;
        }
        else if (strcmp(tokens[0], "cd") == 0){
            chdir(tokens[1]);
            free(tokens);
            free(buffer);
            continue;
        }
            
        launchCmd(tokens);
        free(tokens);
        free(buffer);
        
    }
    while(1);
    return 0;
}
//-------------------------------------------------------------//
char** tokenizeLine(char* cmd)
{
    long bufsize = strlen(cmd);
    int i = 0;
    char** tokens = malloc(bufsize * sizeof(char*));
    if (!tokens) {
        fprintf(stderr, "Shell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    char* token;
    
    token = strtok(cmd, " \n\r\t\a");
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \n\r\t\a");
    }
    tokens[i] = (char *)0;
    return tokens;
}
//------------------------------------------------------------//
int launchCmd(char** args)
{
    pid_t pid, wpid;
    int status;
    
    pid = fork();
    if (pid == 0)// Child process
    {
        if (execvp(args[0], args) == -1)
            perror("Shell");
    }
    else if (pid < 0)// Error forking
        perror("Shell");
    else// Parent process
    {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}