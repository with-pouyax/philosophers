// we have global array of prime numbers, create 10 threads, each taking a unique prime from the primes array and print it to screen.


#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *routine(void * arg)
{
    sleep(1);
    int *i = arg; //here we made a pointer called i, an by int *i = arg, we are casting arg to an integer pointer. somehow we cast it to an integer pointer.
    printf("%i, ", primes[*i]);
    if (arg)
         free(arg);
    
}


int main ()
{
    pthread_t th[10];
    for(int i = 0; i < 10; i++)
    {
        int *a = malloc(sizeof(int));
        if(!*a)
            return 1;
        *a = i;
        pthread_create(&th[i], NULL, &routine, a);
    }
    for(int i = 0; i < 10; i++)
    {
        pthread_join(th[i], NULL);
    }
}