/* 
-now, we gonna take a look at the pthread_mutex_trylock() function. and what it actually does.
- and what is the difference between it and the pthread_mutex_lock() function.
- first, we will create 4 threads, and we will see the behavior of pthread_mutex_lock() function.
- and inside routine, we simply lock a single mutex, by all the four threads, we have created.
- now we recieve 4 "Got lock" messages.
- all 4 threads are executing the same function, but one at a time, has rights to print "Got lock".
 */


pthread_mutex_t mutex;

void *routine(void *arg)
{
    pthread_mutex_lock(&mutex);
    printf("Got lock\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main()
{
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
    {
        if (pthread_create(&th[i], NULL, &sum, NULL) != 0)
        {
            perror("pthread_create error");
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("pthread_join error");
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}

/* 
- if I change the pthread_mutex_lock() function to pthread_mutex_trylock() function.
- this gonna be bit more tricky.
- because, the pthread_mutex_trylock() function, tries to lock the mutex, its not gonna guarantee that it will lock the mutex.
- but if you did , thats great, but if you didn't, you should not be executing the code in critical, therfore you should check the return value
of the pthread_mutex_trylock() function, allways.
-pthread_mutex_trylock() always returns 0, if it locks the mutex, and returns something else, if it didn't lock the mutex. 
- it is actually EBUSY, if the mutex is already locked.
-now if we run the code I will see:
Got lock
Didn't get lock
Didn't get lock
Didn't get lock
-we actually didn't 4 "Got lock" messages, we only get one got lock message, but the rest tried to lock the mutex, that failed and
executions continued on the else block, because the result was not 0, so it just printed "Didn't get lock". and that's the difference between
trylock and lock. so it just didn't get the lock.
- basically, one of them is instant, and the other one is going to waiting.
- so luck is going to be always waiting until the mutex is unlocked, but trylock is just going to say, okay, are you locked? 
if you are not locked, okay, I'm going to lock you and execute the code, but if you are locked, I just gonna return out of the function
with the number other than 0, and you will do whatever you want

 */

pthread_mutex_t mutex;

void *routine(void *arg)
{
    if(pthread_mutex_lock(&mutex) == 0)
    {
        printf("Got lock\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        printf("Didn't get lock\n");
    }
}


int main()
{
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
    {
        if (pthread_create(&th[i], NULL, &sum, NULL) != 0)
        {
            perror("pthread_create error");
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("pthread_join error");
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}