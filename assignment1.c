#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //needed for fork() and getpid()
#include <sys/types.h>
#include <sys/wait.h> //needed for wait() function
#include <errno.h> 
#include <signal.h> //needed for signal function


#define NAME_LENGTH 4
#define NAME_WAIT_INTERVAL 2

void getNAME(char name[NAME_LENGTH +1]){
srand(getpid()+getppid());

name[0] = 49 + rand()%7;

for(int i = 1; i< NAME_LENGTH; i++){
name[i] = 48 + rand()%7;
}


name[NAME_LENGTH]= '\0';
}

void sigint_handler(int sig);
int main()
{
signal(SIGINT,sigint_handler);

while(1){
int pipefds[2];
char name[NAME_LENGTH + 1];
char buffer[NAME_LENGTH + 1];
char string [100];
char s[100];

pipe(pipefds);
pid_t pid = fork(); //declaring fork 

if(signal(SIGINT,sigint_handler)== SIG_ERR){
   perror("signal");
   exit(1);
}


if(pid == 0){ //calling child process
getNAME(name);
close(pipefds[0]);
write(pipefds[1],name, NAME_LENGTH +1);


printf("\n\nPlease enter your name:\n\n");
fgets(string, 100, stdin);
printf("\n\nYour name is :%s\n\n",string);

printf("Generating information in parent and sending to child.....\n");

sleep(NAME_WAIT_INTERVAL);

exit(EXIT_SUCCESS);

}

if(pid>0){ // calling for parent process
wait(NULL);

close(pipefds[1]);
read(pipefds[0],buffer,NAME_LENGTH +1);

printf("Child received information '%s' from parent...\n\n",buffer);

}
}

return EXIT_SUCCESS;//exiting program

}

void sigint_handler(int sig){
printf("****PROCESSED IS BEING INTERRUPTED !!!!****\n");

exit(1);
}


