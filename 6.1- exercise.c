#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *roll_dice(void *arg)
{
    (void)arg;
    int *res = malloc(sizeof(int));
    if (!res)
        return (NULL);
    int value = rand() % 6 + 1;
    *res = value;
    return ((void *)res); // we cast it to void* because pthread_join prototype is int pthread_join(pthread_t thread, void **retval);
    // so we have to return a void pointer
}

int main(void)
{
    int *res;
    srand(time(NULL));
    pthread_t   th;

    if (pthread_create(&th, NULL, &roll_dice, NULL))
    {
        if (res)
            free(res);
        return (1);
    }
    if (pthread_join(th, (void **)&res)) // we cast it to void** because pthread_join prototype is int pthread_join(pthread_t thread, void **retval);
      {
        if (res)
            free(res);
        return (1);
    }
    printf("%d\n", *res);
    free(res);
}