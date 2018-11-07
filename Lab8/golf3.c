#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  int firstchild = fork();
  int second, third, forth;
  if( firstchild == 0)
  {
    // Do work here to fork to create target pstree diagram
    
    second = fork();
    if(second == 0){
        third = fork();
        if(third != 0){
            fork();
        }else{
            fork();
        }
        
    }else{
        forth = fork();
        
        if (forth != 0) {
            fork();
        }else{fork();}
        
    }
    
    //fork();
    // Put all calls to fork() above this line, and don't modify below.

    sleep(2); /* required so that execve will run before the process quits, don't do this sort of thing in real code */
    return 0;
  }
  else 
  {

    // parent process calls execve on pstree

    char pid[20]; // This is where we will put the pid argument to the pstree execution
    char* args[] = {"/usr/bin/pstree", "-p" , pid, NULL};
    char* environ[] = {NULL};

    sprintf(pid, "%u", firstchild);
    execve( args[0], args, environ);
  }
}
