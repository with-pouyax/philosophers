/* 
- in our last code we had like 5 treads and a routime, as you noticed, it is a
lot of duplicated code which is not efficient.
- how can I create multiple threads in a loop?
- so instead of p1, p2, p3, p4, p5, we can use an array of threads.
- now that we have created threads, we can call join in the same for loop.
- it is looks fine at first glance, 
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
    pthread_t th[4];
    int i;
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < 4; i++)
    {
        if (pthread_create(th + i, NULL, &routine, NULL)) //instead th + i, we can use &th[i]
            return 1;
    if (pthread_join(th[i], NULL))
        return 2;

    }
    pthread_mutex_destroy(&mutex, NULL);
    printf("mails = %d\n", mails);
    return 0;
}

/* 
- lets print some messages to see what is happening.
- we know after that we return from the pthread_join, like with wait function call,
in processess, we know that the thread has finished its execution.
- so we can print a message after the pthread_join that the thread has finished.
- if I run this now, the result will be :
Thread 0 has been started
Thread 0 has finished
Thread 1 has been started
Thread 1 has finished
Thread 2 has been started
Thread 2 has finished
Thread 3 has been started
Thread 3 has finished
mails = 4000000
- so we see threads are running one after the other, and at no point more than one thread is running.
- this is not propper, because we used threads so things can run in parallel.
- so why is this happening?
- that has to be with the way the loop is working.
- for i = 0, we are creating a thread, and we are also joining the thread.
- so the first tread gets created, and then we wait for it to finish, and only
then we create the second thread.

 */

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
    pthread_t th[4];
    int i;
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < 4; i++)
    {
        if (pthread_create(th + i, NULL, &routine, NULL))
            return 1;
    printf("thread %d has been started\n", i);
    if (pthread_join(th[i], NULL))
        return 2;
    printf("thread %d has finished\n", i);

    }
    pthread_mutex_destroy(&mutex, NULL);
    printf("mails = %d\n", mails);
    return 0;
}

/* 
- so the correct way to do this is to create all the threads first and then join them.
- now if we lunch this, we will see:
thread 0 has been started
thread 1 has been started
thread 2 has been started
thread 3 has been started
thread 0 has finished
thread 1 has finished
thread 2 has finished
thread 3 has finished
mails = 4000000
- so now we see that all the threads are running in parallel.
- consider that this is not the exact order of the threads, this is just
the way we print the messages.
 */

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
    pthread_t th[4];
    int i;
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < 4; i++)
    {
        if (pthread_create(th + i, NULL, &routine, NULL))
            return 1;
    printf("thread %d has been started\n", i);

    }
    for (i = 0; i < 4; i++)
    {
    if (pthread_join(th[i], NULL))
        return 2;
    printf("thread %d has finished\n", i);
        
    }
    pthread_mutex_destroy(&mutex, NULL);
    printf("mails = %d\n", mails);
    return 0;
}