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
    *(int *)arg = sum;
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