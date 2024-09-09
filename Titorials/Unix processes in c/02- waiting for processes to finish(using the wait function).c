/*  -we want to see how actually wait for processes to finish
    -let's first create a simple program.
    -I want to call fork() only once, and I want to print all numbers
    from 1 to 10.
    -first, I want to print first 5 numbers in the child process, and
    then the other 5 numbers in the main process.
    -to do that, first I intialize n, which is going to be the number
    that I start at.
    -then we have to check if we are in the main process because
    in main it will start from 6 to 10, and if we are in the child
    process, it will start from 1 to 5.
    -then we execute a for loop, that goes through all the numbers
    and prints them. 
    -everytime we print something to standard output, it has an internal buffer, so if
    we print a lot of things very quickly, it is not going to automatically
    print them on the course, it is going to get all the input and then
    print it all at once.
    -so in our case, I want to print them exactly when I call printf(), so we have to use
    the fflush() function, so now everytime we print a number or a digit, it will shown immediately,
    on the terminal.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }

    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    return 0;
}

// -now if I run this program, the result will be: 
// 1 6 2 7 3 8 4 9 5 10
// why ? 
// because at the beginning, we have fork(), which splits the process into two processes,
// now both execute every single statement from now on, almost
// at the same time, so the first process will print 1, then the second process
// will print it's first number, which is 6, then the first process will print 2
// and second process will print 7, and so on.
// -so now, how can we recie first the first 5 numbers, and then the other 5 numbers ?
// -to do that, we have to use the wait() function.
// -what the wait() function does is that it stops the execution until
// a child process finishes executing.
// -so right now, we have two processes, we have the main process and the child process,
// and they are almost executing at the same time, so what we can do
// is in main process, we can say wait for a child process to finish,
// and then print your numbers as well.
// -so what is gonna happend is that the main process will wait and
// the child process will go forward and print the first 5 numbers,
// finish it's execution, and get it's memory freed, and then the main process 
// will continue and print the other 5 numbers.
//- if we put wait() here, that gonna be problematic, because
// we have to wait only in the main process, but now wait() is in both processes,
// -here in main process, we do have a child process to wait for, but
// in the child process, we don't have any child process to wait for.
// -so we just gonna get stuck there. 

int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    wait();
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    return 0;
}
// -in order to fix that, we will write it like this:
// -what we are saying here is that if we are in the main process,
// then wait for a child process to finish, and then print the numbers.
// -if we are in the child process, then just print the numbers.
// -printf("\n"); is not executed once, but twice.

int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    printf("\n");
    return 0;
}

// to avoid that we also a if condition to check if we are in the main process

int main(int argc, char *argv[])
{
    int id = fork();
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    if (id == 0){
        printf("\n");
    }
    return 0;
}
