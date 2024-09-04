/* 
-here we want to solve the race condition problem.
- we need to know what is mutex and how to use it.
- mutex is sort of a lock around a sections of code.
- so what we can do is to initialize a variable called luck,
and set it to 1 whenevr we are doing something and set it to 0
when we are done, and before al this check if luck is already 1.
- if the luck is already 1, then we should wait until luck is 0.
- in this way wat would happen is that if thread tries to actually
increment the mails variable, well it certainly will increment it
but it will also set the luck to 1, so if the second thread tries
comes in at the same time, and tries to increment that, it is gonna
stop that, so only after we are done with the incrementind, it is
gonna reset to zero so the second thread could come in and read, increment
and write in the write order.
- how do we wait until luck is 0?
- we dont have to do that, we can use a mutex.
 */


int mails = 0;
int luck = 0;

void *routine(void *arg)
{
    if (luck == 1)
    {
       // wait until luck is 0 
    }
    for (int i = 0; i < 1000000; i++)
    {
        luck = 1;
        mails++;
        luck = 0;
    }
}

int main (int argc, char *argv[])
{
    pthread_t p1;
    pthread_t p2;

    if (pthread_create(&p1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&p2, NULL, &routine, NULL))
        return 2;
    if (pthread_join(p1, NULL))
        return 3;
    if (pthread_join(p2, NULL))
        return 4;
    printf("mails = %d\n", mails);
    return 0;
}

/* 
- instead of using luck variable, we can use a mutex of type pthread_mutex_t.
- in the main function, we have to initialize the mutex using pthread_mutex_init function.
- the first argument is the mutex variable, and the second argument is the mutex attribute.
- we passed NULL because we are using the default attributes.
- and of with an init comes a destroy, so we have to destroy the mutex to
free the memory that was allocated for the mutex.
- so we call pthread_mutex_destroy function.
- the first argument is the mutex variable.
- the second argument is the mutex attribute.
- we passed NULL because we are using the default attributes.
- now we have mutex available to us, we havnt used it yet.
-so instead of using luck variable, we use pthread_mutex_lock to
lock the mutex and pthread_mutex_unlock to unlock the mutex.
- so now if we lunch the program, we will get the expected result of 2000000.
- so what it is doing is, it is protecting the mails++;
- it protecting it against other threads to executing it at the same time.
- it is also working with more than two threads, so if we haver 5 
threads here instead of 2, it will still work. ant print 5000000.
- we have to mention is that race conditions can only on mlti core processors, so
if you have a single core processor, it will be very unlikely to see a race condition.



 */

int mails = 0;
pthread_mutex_t mutex;

void *routine(void *arg)
{
    for (int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main (int argc, char *argv[])
{
    pthread_t p1;
    pthread_t p2;
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&p1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&p2, NULL, &routine, NULL))
        return 2;
    if (pthread_join(p1, NULL))
        return 3;
    if (pthread_join(p2, NULL))
        return 4;
    pthread_mutex_destroy(&mutex, NULL);
    printf("mails = %d\n", mails);
    return 0;
}