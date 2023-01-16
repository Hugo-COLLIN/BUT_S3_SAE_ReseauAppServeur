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


int main(int argc, char** argv) {
  run();
  return 0;
}

void run()
{
    while (1) {
        printf("mbash>> ");
        fgets(cmd, MAXLI, stdin);
        mbash(cmd);
    }
}

void mbash() {
  printf("<< %s", cmd);
  system(cmd);
}
