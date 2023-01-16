#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <errno.h>

#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
int pathidx;

//Functions pre-declarations
void mbash();
void run();
void welcome();
void capture();
void processing();
void checkExit();
void interpreter();
void identifyTokens(char *arg_list[], char *p, int count);
void executeCmd();
int nbArguments();
void freeTokens(char *arg_list[], char *p);


int main(int argc, char** argv) {
    run();
    return 0;
}


void run()
{
    welcome();

    while (1) {
        processing();
    }
}


void welcome()
{
    printf("-----------------\n");
    printf("Welcome to mbash!\n");
    printf("Hugo COLLIN - 16/01/2023\n\n");
    printf("Type 'exit' to exit mBash.\n");
    printf("-----------------\n");
}

void processing() {
    capture();
    interpreter();
}

void capture()
{
    printf("mbash-%i> ", getpid());
    fgets(cmd, MAXLI, stdin);
    cmd[strlen(cmd) - 1] = '\0';
}

void interpreter()
{
    checkExit();

    int count = nbArguments();
    char *arg_list[count+2]; //array of arguments
    char *p = strdup(cmd); //duplicates cmd string

    identifyTokens(arg_list, p, count);
    executeCmd(arg_list);
    freeTokens(arg_list, p);
}

void checkExit() {
    if (strcmp(cmd, "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
}

int nbArguments()
{
    int count = 0; //number of arguments
    int loop;
    for (loop = 0 ; loop < strlen(cmd) ; ++ loop)
    {
        if (cmd[loop] == ' ') ++ count; //counting the number of spaces
    }
    return count;
}

void identifyTokens(char *arg_list[], char *p, int count)
{
    char *tmp = strtok(p," "); //splits the duplicated string using spaces
    int increment = 0; //index used to browse the arg_list array

    while (tmp != NULL)
    {
        arg_list[increment] = strdup(tmp);
        increment ++;
        tmp = strtok(NULL," ");
    }
    arg_list[increment] = NULL;
}

void executeCmd(char *arg_list[])
{
    pid_t process = fork();
    if (process == 0) //parent process
    {
        int res = execvp(arg_list[0],arg_list);
        if (res == -1) printf("%s\n",strerror(errno));
        exit(0);
    }
    else //child process
    {
        wait(&process);
    }
}

void freeTokens(char *arg_list[], char *p)
{
    int increment=0;
    while (arg_list[increment] != NULL)
    {
        free(arg_list[increment]);
        increment ++;
    }
    free(p);
}