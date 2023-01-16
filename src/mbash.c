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
void execute();


int main(int argc, char** argv) {
    run();
    return 0;
}


void run()
{
    welcome();

    while (1) {
        capture();
        processing();
    }
}


void welcome()
{
    printf("-----------------\n");
    printf("Welcome to mbash!\n\n");
    //printf("Type 'exit' to exit.\n");
    printf("-----------------\n");
}

void capture()
{
    printf("mbash> ");
    fgets(cmd, MAXLI, stdin);
    cmd[strlen(cmd) - 1] = '\0';
}


void processing() {
    checkExit();
    printf("<< %s\n", cmd);
    //system(cmd);
    interpreter();

}

void checkExit() {
    if (strcmp(cmd, "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
}

void interpreter()
{
    //Analyze the command
    int count = 0; //number of arguments
    int loop;
    for (loop = 0 ; loop < strlen(cmd) ; ++ loop)
    {
        if (cmd[loop] == ' ') ++ count; //counting the number of spaces
    }

    char *arg_list[count+2]; //array of arguments
    char *p = strdup(cmd); //duplicates cmd string

    identifyTokens(arg_list, p, count);

    execute(arg_list);

    int increment=0;
    while (arg_list[increment] != NULL)
    {
        free(arg_list[increment]);
        increment ++;
    }
    free(p);
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

void execute(char *arg_list[])
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