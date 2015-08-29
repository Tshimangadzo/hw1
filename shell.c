#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#define O_RDONLY
#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80
#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"
#include <sys/stat.h>
#include <fcntl.h>


int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

//void concat(char[], char[]);
int cmd_help(tok_t arg[]);
int cmd_open(tok_t arg[]);


/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_open, "cd", "Go to new directory"}
};
int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}
int cmd_open(tok_t arg[]){


    if (arg[0] == NULL) {
	chdir(getenv("HOME"));
      } else
      {
     if (chdir(arg[0]) != 0) {
      perror("cmd");
     }
  }
  return 1;
}
int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{   
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */

}

/**
 * Add a process to our process list
 */
//void add_process(process* p)
//{
  /** YOUR CODE HERE */
//}

/**
 * Creates a process given the inputString from stdin
 */
//process* create_process(char* inputString)
//{
  /** YOUR CODE HERE */
//}
char * concat(char *s1, char *s2) {
  // int i, j;
 char * s = malloc(strlen(s1)+strlen(s2)+1);
s=strcpy(s,s1);
s=strcat(s,s2);
return s;
}
int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);/* user input string */
  tok_t *t,*PP;			/* tokens parsed from input */
  int lineNum = 0;int star;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  //pid_t cpid, tcpid, cpgid;
  //int fd;  /* new file descriptor */
  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);
  init_shell();
  char *Pph;
  char tshi[1000];
  lineNum=0;
int output;
char *ptrA, *ptrB;
    ptrA = strstr(s, "<");
    ptrB = strstr(s, ">");
  fprintf(stdout, "%d: %s: ", lineNum, getcwd(tshi, sizeof(tshi)));
  while ((s = freadln(stdin))){
    
     //The file end up here
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0) cmd_table[fundex].fun(&t[1]);
    else {
           pid = fork();
          //int current_out,current_in;
	     if (pid < 0)
		{
                    printf("cmd");
	           exit(1);
		}		
		else if (pid == 0){

                                if (ptrA) { 
					int fd0 = open(argv[0], O_RDONLY | O_CREAT, 0);
					      dup2(fd0, STDIN_FILENO);
                                             // fprintf(stdout,"%s",fd0);
					      close(fd0);
				}
				if (ptrB) {
					 int fd1 = creat(output, 0644);
					 dup2(fd1, STDOUT_FILENO);
					 close(fd1);
				}
					    
                Pph=getenv("PATH");
                PP = getToks(Pph);
                for(int i = 0;i<MAXTOKS && PP[i];i++){
                char *mine =concat(PP[i],"/");
                mine =concat(mine,t[0]);
		 if(access(mine,F_OK) != -1){
		     execve(mine,t, NULL); 
		   }
                  }  
		execvp(*t, t);
                perror(*t);
                exit(0);
 
	 }
       else{
                   while ((pid != wait(&star))); 
             }

       }
  fprintf(stdout, "%d: %s: ", ++lineNum, getcwd(tshi, sizeof(tshi)));
  }
  return 0;
}
