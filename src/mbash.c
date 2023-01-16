#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define MAXLI 2048


char cmd[MAXLI];
char path[MAXLI];
int pathidx;

//Pre-declaration of functions
void mbash();
void run();
void welcome();
void input();
void output();
void checkExit();


int main(int argc, char** argv) {
    run();
    return 0;
}


void run()
{
    welcome();

    while (1) {
        input();
        output(cmd);
    }
}


void welcome()
{
    printf("-----------------\n");
    printf("Welcome to mbash!\n\n");
    //printf("Type 'exit' to exit.\n");
    printf("-----------------\n");
}

void input()
{
    printf("mbash> ");
    fgets(cmd, MAXLI, stdin);
    cmd[strlen(cmd) - 1] = '\0';
}


void output() {
    checkExit();
    printf("<< %s\n", cmd);
    system(cmd);

}

void checkExit() {
    if (strcmp(cmd, "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
}
