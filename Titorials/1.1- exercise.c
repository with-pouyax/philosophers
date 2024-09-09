//Write a C program that creates and runs a thread using the POSIX thread (pthread) library. The thread should execute a function that prints "test from threads\n" to the console. Ensure the program waits for the thread to finish before exiting.

/* #include <pthread.h>
#include <stdio.h>

void *routine()
{
    printf("test from thread \n");
    return NULL;
}

int main (void)
{
    pthread_t   t1;

    pthread_create(&t1, NULL, &routine, NULL);
    pthread_join(t1, NULL);
} */

// why we gave t1 to pthread_create as a address but we gave it as a value to pthread_join?
// because pthread_create takes the address of the thread id, because it will store the thread id in that address.
// but pthread_join takes the value of the thread id, because it will use the value of the thread id to wait for the thread to finish.
// what exactly pthread_create does?
// pthread_create creates a thread and starts executing the function that we passed to it. and it will store the thread id in the address that we passed to it,
// which is here t1. we need this thread id to wait for the thread to finish.
// what exactly pthread_join does?
// pthread_join waits for the thread to finish. it takes the value of the thread id, which is t1, and it waits for the thread to finish. and once the thread finishes, it will return
// and the program will continue executing the next line of code. without pthread_join, the program will exit before the thread finishes. and the thread will be terminated. 
// to explain better, if we remove pthread_join from the code, the program will exit before the thread finishes, and we will not see the output "test from threads". because  it will
// take some time for the thread to create and start executing the function. and the program will exit before the thread finishes. so we need pthread_join to wait for the thread to finish. 



/* 
Write a C program that creates two threads. Each thread should print a message, sleep for 3 seconds, and then print another message.
Make sure to handle any errors that occur during thread creation and joining. The main thread should wait for both threads to complete before it exits.
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *routine()
{
    printf("Helle from thread\n");
    sleep (1);
    printf("end of thread\n");
    return NULL;
}

int main (void)
{
    pthread_t t1;
    pthread_t t2;

    if(pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if(pthread_create(&t2, NULL, &routine, NULL))
        return 2;
    if(pthread_join(t1, NULL))
        return 3;
    if(pthread_join(t2, NULL))
        return 4;
}

// it is printing each twice because we have two threads. and each thread is printing the message twice.