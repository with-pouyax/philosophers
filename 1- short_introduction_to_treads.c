/*
1- to use tread we need to include the pthread.h header file.
2- we also have to link the pthread library using the -pthread option
as one of compiler flags, because the pthread library is not included by default.
3- now we can create a thread.
4- when we make a thread, it will start executing the function, whatever function
we pass to it.
5- we wrote routine function.
6- we created a thread t1 using the pthread_t data type
because pthread_t is a data type that is used to identify a thread
and we will use it to sore the thread id.
7- we used pthread_create function to create a thread.
    a. the first argument is the thread id.
    b. the second argument is the thread attribute.
       these atributes are used to specify the thread behavior.
       here we passed NULL, and it means we are using the default attributes.
    c. the third argument is the function that the thread will execute.
        to pass a function as an argument, we have to pass it as a pointer.
    d. the fourth argument is the argument to the function.
        here we passed NULL because we are not passing any argument to the function.
8- one last thing to do is we have to wait for the thread to finish its execution.
   to do this, we call pthread_join function. and this the way to wait for the thread to finish.
    the first argument is the thread id, and the second argument is the
    pointer to the location where the return value of the thread will be stored.
    since we are not returning anything from the thread, we passed NULL.
9- now if we run the program, we will see the output "test from threads".
10- but how do I know that this is in parallel?

//OoOoOoOoOoOoOo

*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//code 1
/* void *routine()
{
    printf("test from threads\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1;

    pthread_create(&t1, NULL, &routine, NULL);
    pthread_join(t1, NULL);

    return 0;
} */


/* 
11- we can create more than one thread. (here I will wrire code 2)
12- we initialized another thread t2.
13- we created another thread using pthread_create function.
14- now we have two threads running in parallel.
    it means that the two threads are running at the same time
    calling the routine function.
15- if we run the program, we will see the output "test from threads" twice.
16- but how do I know that this is in parallel?    
17- we can add extra messages in the routine function. using
    sleep function in between the messages.
18- now if we run the program, we expect to see the first message
    "test from threads" and then after 3 seconds, we expect to see the second message.
17- but what really we see twice first message and twice second message.
    it means that the two threads are running in parallel.
18- here my treads are returning an integer value, so I can do error checking.
         
 */

void *routine()
{
    printf("test from threads\n");
    sleep(3);
    printf("End of thread\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_t t2;

    if (pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if(pthread_create(&t2, NULL, &routine, NULL))
        return 2;

    if(pthread_join(t1, NULL))
        return 3;
    if(pthread_join(t2, NULL))
        return 4;
    return 0;
}