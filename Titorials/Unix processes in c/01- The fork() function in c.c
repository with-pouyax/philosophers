/* 
01- to use fork() function in c, we need to include unistd.h,
consider that unistd.h is a linux specific library, so in windows
we use other functions to create processes.
02- if we add a fork() before printf, Hello world will be printed twice.
03- what fork() does is that it creates a new process, and the new process
is a copy of the parent process, so the new process will have the same code
and data as the parent process, but the new process will have a different
process id.
4- so I can save process id of the new process in a variable.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    fork();
    printf("Hello world\n");
    return 0;
}
/* 4- so I can save process id of the new process in a variable.
   5- now I gonna recieve for example:
   Hello world from id: 0
   Hello world from id: 4222
   why ?
   - everything before the line of fork() is going to be executed once.
   - from the fork() line onwards, everything is going to be executed twice,
   once per process.
   - so we're gonna have two separate IDs, being returned from fork().,
   one gonna be the main process with ID 4222, and the other one gonna be
    the child process with a different ID 0.
    - the ID of the child process is always 0, and it returns -1 if 
    the fork() function fails.
    - every process in linux, windows, everywhere has a process ID.
    - so I can add a condition to show me, I am in the child process.
 */

int main(int argc, char *argv[])
{
    int id = fork();
    printf("Hello world from id: %d\n", id);
    if (id == 0)
    {
        printf("I am the child process\n");
    }
    else
    {
        printf("I am the main process\n");
    }
    return 0;
}

// what would happend if I called fork() twice ?
// now how many times Hello world will be printed ?
// 4 times.
// why ?
// because the first fork() will create a child process,
// and the second fork() will create a child process for 
// the child process, so we will have 4 processes in total.
//the way we can calculate the number of processes is 2^n,
// so if we call fork() 3 times, we will have 8 processes.
// but how can we create only 3 processes ?

int main(int argc, char *argv[])
{
    fork();
    fork();

    printf("Hello world\n");
    return 0;
}

//using a if conditon to create only 3 processes. we are saying
//that check if we are not in the child process, then create a new
//process.

int main(int argc, char *argv[])
{
    int id = fork();
    if (id != 0)
        fork();

    printf("Hello world\n");
    return 0;
}
