/* 
we will have 2 threads.
- I have this array:
int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
- what we want the program do is :
- to sume up the numbers in the primes array.
we have to send the first half to the first
thread, and the second half to the second thread.
- so we will get two sums in main, and we will sum them up.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
int global = 0;

void *sum(void *arg)
{
    int sum;
    int *index;

    sum = 0;

    index = arg;
    for(int i = 0; i < 5; i++)
    {
        sum += primes[*index + i];
    }
    printf("index: %d\n", *index);
    printf("sum: %d\n", sum);
    *(int *)arg = sum; // here arg is a pointer to a void pointer, by (int *), we are casting it to a pointer to an integer
                       // and then we are dereferencing it and we access the value of the integer, then we assign the sum to it.
    return(arg);
}

int main()
{
    pthread_t th[2];
    int *r;

    for(int i = 0; i < 2; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        printf("i = %d :\n", *a);
        pthread_create(&th[i], NULL, &sum, a);
    }
    for(int i = 0; i < 2; i++)
    {
        pthread_join(th[i], (void **)&r);
        global += *r;
        free(r);
    }
    printf("%d\n", global);
}