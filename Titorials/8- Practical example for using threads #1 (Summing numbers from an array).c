/* 
- lets do an easy example regarding of passing arguments
to threads in c.
- instead of having 10 threads, we will have 2 threads.
- what I want the program do is :
- to sume up the numbers in the primes array.
- so it is easy, we have to send the first half to the first
thread, and the second half to the second thread.
- so we will get two sums in main, and we will sum them up.
- so first we change the ecerthing from last video code
to 2, because we need 2 threads now.
but we cant pass to it now the i, because I want to pass
the first thred indexes 0, 1, 2, 3, 4, and the second thread
5, 6, 7, 8, 9. 
 */

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void *routine(void *arg)
{
    int index = *(int*) arg;
    int sum = 0;
    for (int j = 0; j < 5; j++)
    {
        sum += primes[index + j];
    }
    *(int*) arg = sum;
    return arg;
}

int global_sum = 0;
int main(int argc, char *argv[])
{
    pthread_t th[2];
    int i;
    for (i = 0; i < 2; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a))
            perror("failed to create thread");
    }
    for (i = 0; i < 2; i++)
    {
        int *r;
        if (pthread_join(th[i],(void **) &r))
            perror("failed to join thread");
        global_sum += *r;
        free(r);
    }
    printf("The sum is %d\n", global_sum);
    return 0;
}