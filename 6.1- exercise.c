#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *roll_dice(void *arg)
{
    int *result = malloc(sizeof(int));
    int value = rand() % 6 + 1;
    *result = value;
    return((void*)result);
}

int main(void)
{
    int *res;
    srand(time(NULL));
    pthread_t th;

    if (pthread_create(&th, NULL, &roll_dice, NULL))
        return 1;
    if (pthread_join(th, (void **)&res))
        return 2;
    printf("%d\n", *res);
}