/* Write a C program that creates two threads, each of which increments a shared variable mails by 100,000. After both threads complete their execution,
 the program should print the final value of mails. Ensure that the program correctly handles the thread creation and joining. */

 #include <stdio.h>
 #include <pthread.h>
#include <unistd.h>

int mail = 0;
pthread_mutex_t mutex;

void *routine()
{
    int i = 0;
    while(i < 1000000)
    {
        mail++;
        i++;
    }
    return(NULL);
}

 int main (void)
 {
    pthread_t   t1;
    pthread_t   t2;

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex, NULL);

    printf ("%d\n", mail);
    
 }

 // next time do it without global variables.