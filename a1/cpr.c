/* ------------------------------------------------ ------------
File: cpr.c

Last name: Guled
Student number: 300021664

Description: This program contains the code for creation
 of a child process and attach a pipe to it.
	 The child will send messages through the pipe
	 which will then be sent to standard output.

Explanation of the zombie process
(point 5 of "To be completed" in the assignment):

It is a child process which has not yet been 
collected by the parent process but completed its execution state. 
By executing wait call, a parent process can to collect its child zombies.


------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

/* Prototype */
void createChildAndRead (int);

/* -------------------------------------------------------------
Function: main
Arguments: 
	int ac	- number of command arguments
	char **av - array of pointers to command arguments
Description:
	Extract the number of processes to be created from the
	Command line. If an error occurs, the process ends.
	Call createChildAndRead to create a child, and read
	the child's data.
-------------------------------------------------- ----------- */

int main (int ac, char **av)
{
   int processNumber;
    if (ac == 2)
    {
        if (sscanf(av[1], "%d", &processNumber) == 1)
        {
            createChildAndRead(processNumber);
        }
        else
            fprintf(stderr, "Cannot translate argument\n");
    }
    else
        fprintf(stderr, "Invalid arguments\n");
    return (0);
}


/* ------------------------------------------------ -------------
Function: createChildAndRead
Arguments: 
	int prcNum - the process number
Description:
	Create the child, passing prcNum-1 to it. Use prcNum
	as the identifier of this process. Also, read the
	messages from the reading end of the pipe and sends it to 
	the standard output (df 1). Finish when no data can
	be read from the pipe.
-------------------------------------------------- ----------- */

void createChildAndRead(int prcNum)
{
    char bufferChar[256]; //buffer of chars holds 256 spots
    
    int bufferLength; // our buffers length
 
 //case 1: if prcNum is 1 then print only output   
    if (prcNum == 1) 
    {
        bufferLength = sprintf(bufferChar, "Process %d begins", prcNum);
        
        write(STDOUT_FILENO, bufferChar, bufferLength);
        
        sleep(5);
        
        bufferLength = sprintf(bufferChar, "Process %d ends", prcNum);
        
        write(STDOUT_FILENO, bufferChar, bufferLength);
    }

// case 2: if prcNum is not 1, then print the processes in order
    else 
    {
        bufferLength = sprintf(bufferChar, "Process %d begins", prcNum);
        
        write(STDOUT_FILENO, bufferChar, bufferLength);
       
        int pf[2];
        
        if (pipe(pf) == -1 ) 
        {
            fprintf(stderr, "Error in formation of pipe\n");
            exit(EXIT_FAILURE);
        }


        if (fork() == 0) 
        {
            close(pf[0]);
            dup2(pf[1], STDOUT_FILENO);
            
            char args[20];
            sprintf(args, "%d", prcNum - 1);
            char* argv[] = {"cpr", args, NULL};
            execv("cpr", argv);
        }

        close(pf[1]);
     
        char buf[BUFSIZ];
        int numRead;
        
        while((numRead = read(pf[0], buf, BUFSIZ)) > 0) 
        {
            
            write(STDOUT_FILENO, buf, numRead);
        
        }

        
        close(pf[0]);
        sleep(10);
       
        bufferLength = sprintf(bufferChar, "Process %d ends", prcNum);
        write(STDOUT_FILENO, bufferChar, bufferLength);
    }
}


