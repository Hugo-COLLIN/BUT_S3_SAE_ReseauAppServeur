#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <errno.h>
#include <glob.h>

#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
int pathidx;

//Functions pre-declarations
void mbash();
void run();
void welcome();
void capture();
void prompt();
void checkExit();
void interpreter();
void identifyTokens(char *arg_list[], char *c, int count);
void executeCmd();
int nbArguments();
void freeTokens(char *arg_list[], char *c);
void execute(char *arg_list[]);
int cdF(char *args[]);
int helpF(char *args[]);
int exitF(char *args[]);

/*
  List of builtin commands
 */
char *inbuiltStr[] = {
  "cd",
  "help",
  "exit"
};

int (*inbuiltF[]) (char **) = {
  &cdF,
  &helpF,
  &exitF
};

int inbuiltLength() {
  return sizeof(inbuiltStr) / sizeof(char *);
}

/**
 * Main function
 */
int main(int argc, char** argv) {
    run();
    return 0;
}


void run()
{
    welcome();

    while (1) {
        interpreter();
    }
}


void welcome()
{
    printf("\n-----------------\n");
    printf("Welcome to mBash!\n");
    printf("Hugo COLLIN - 16/01/2023\n\n");
    printf("Type 'exit' to exit mBash.\n");
    printf("-----------------\n");
}

void execute(char *arg_list[])
{
  if (arg_list[0] == NULL) {
    // An empty command was entered.
    return;
  }

  for (int i = 0; i < inbuiltLength(); i++) {
    if (strcmp(arg_list[0], inbuiltStr[i]) == 0) {
      (*inbuiltF[i])(arg_list);
      return;
    }
  }
  executeCmd(arg_list);
}

//-----------------

void interpreter()
{
    capture();

    int count = nbArguments();
    char *arg_list[count+2]; //array of arguments
    char *c = strdup(cmd); //duplicates cmd string

    identifyTokens(arg_list, c, count);
    execute(arg_list);
    freeTokens(arg_list, c);
}

void capture()
{
    printf("\nmBash-%i> ", getpid());
    fgets(cmd, MAXLI, stdin);
    cmd[strlen(cmd) - 1] = '\0';
}

int nbArguments()
{
    int count = 0; //number of arguments
    for (int loop = 0 ; loop < strlen(cmd) ; ++ loop)
    {
        if (cmd[loop] == ' ') ++ count; //counting the number of spaces
    }
    return count;
}

void identifyTokens(char *arg_list[], char *c, int count)
{
    char *tmp = strtok(c, " "); //splits the duplicated string using spaces
    int increment = 0; //index used to browse the arg_list array

    while (tmp != NULL)
    {
        char *c = strstr(tmp, "*");
        if (c == NULL)
        {
            arg_list[increment] = strdup(tmp);
            increment ++;
        }
        else
        {
            glob_t g;
            int resGlob = glob(tmp, 0, NULL, &g);
            if (resGlob == 0)
            {
                for (int loop = 0 ; loop < g.gl_pathc ; ++ loop)
                {
                    arg_list[increment] = strdup(g.gl_pathv[loop]);
                    increment ++;
                }
            }
            globfree(&g);
        }
        tmp = strtok(NULL, " ");
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

void freeTokens(char *arg_list[], char *c)
{
    int increment=0;
    while (arg_list[increment] != NULL)
    {
        free(arg_list[increment]);
        increment ++;
    }
    free(c);
}


/*
  Builtin functions implementation.
*/
int cdF(char *args[])
{
  if (args[1] == NULL) {
    fprintf(stderr, "Expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mBash");
    }
  }
  return 1;
}

int helpF(char *args[])
{
  printf("mBash - Hugo COLLIN\n");
  printf("Type a command, then press enter.\n");
  printf("The following commands are in-built:\n");

  for (int i = 0; i < inbuiltLength(); i++) {
    printf("  %s\n", inbuiltStr[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int exitF(char *args[])
{
  exit(EXIT_SUCCESS);
}
