/* 
- we gonna make very basic mutex called mutexFuel
- we gonna initialize it
- we gonna create 8 threads
- in routine function we gonna fillup the gas station
- we gonna lock the fuel
- now if we run this we will get:
    - increamented fuel to : 100
    - increamented fuel to : 150
    - increamented fuel to : 200
    - increamented fuel to : 250
    - increamented fuel to : 300
    - increamented fuel to : 350
    - increamented fuel to : 400
    - increamented fuel to : 450
    - Fuel: 450 
 */


#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* arg)
{
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("increamented fuel to : %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);}

int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&threads[i], NULL, &routine, NULL) != 0)
        {
            perror("Failed to create the thread");
        }
    }
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Failed to join the thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    pthread_mutex_destroy(&mutexFuel);
    return 0;
}

/* 
- a very simple example of a deadlock is when you locke
a mutex twice
- if for some reason I lock the mutex twice i same thread
- if we lunch this we will get:
    we notice the programm is never finish its execution
- because the lock has already been taken and, if you try
to take it second time, you are gonna left with nothing.
- and pthread doesn't know that, the lock is owned by same
thread.
- so lock a mutex twice is gonna block everything.
 */

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* arg)
{
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("increamented fuel to : %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);}

int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&threads[i], NULL, &routine, NULL) != 0)
        {
            perror("Failed to create the thread");
        }
    }
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Failed to join the thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    pthread_mutex_destroy(&mutexFuel);
    return 0;
}

/* 
- but what about if we have two mutexes ?
- now suppose we always want to fill the water, in such
a way that we have same amount of water as fuel.
- but I have to lock also the water mutex.
- now if I try to lunch it, we will get:
    - increamented fuel to : 200 set water to : 200
    - increamented fuel to : 250 set water to : 250
    - increamented fuel to : 300 set water to : 300
    - increamented fuel to : 350 set water to : 350
    - increamented fuel to : 400 set water to : 400
    - increamented fuel to : 450 set water to : 450
- nice, it works.
- the issue is that, if we have multiple locks, locked
in different order, we can have a deadlock.
 */


pthread_mutex_t mutexFuel;
int fuel = 50;
pthread_mutex_t mutexWater;
int water = 10;

void* routine(void* arg)
{
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexWater);
    fuel += 50;
    water = fuel;
    printf("increamented fuel to : %d\n set water to : %d\n", fuel, water);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexWater);
}


int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_mutex_init(&mutexWater, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&threads[i], NULL, &routine, NULL) != 0)
        {
            perror("Failed to create the thread");
        }
    }
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Failed to join the thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    printf("Water: %d\n", water);
    pthread_mutex_destroy(&mutexFuel);
    pthread_mutex_destroy(&mutexWater);
    return 0;
}

/* 
- suppose for whatever reason, we have a condition
where we lock the fuel first and then the water.
- but this is a very big mistake,if I try to run this
if the first thread runs the first line of if statement,
and the second thread runs the first line of else statement,
we will get a deadlock.
why ?
- so the first thread locks the fuel mutex
- the second thread locks the water mutex
- so now the first mutex is waiting for the water mutex
but the water mutex is acquired, so the first thread is
keep waiting for the water mutex to be released.
-but now second thread is waiting for the fuel mutex to be
released, but the fuel mutex is acquired by the first thread
so the second thread is keep waiting for the fuel mutex to
be released.
- now we have a situation where both threads are waiting and
none of them can proceed. this is what a deadlock is.
- if we add sleep(1) in the routine, we can hit the deadlock
every time and not just refresh until we hit it.
 */

pthread_mutex_t mutexFuel;
int fuel = 50;
pthread_mutex_t mutexWater;
int water = 10;

void* routine(void* arg)
{
    if (rand() % 2 == 0)
    {
        pthread_mutex_lock(&mutexFuel);
        sleep(1);
        pthread_mutex_lock(&mutexWater);
    }
    else
    {
        pthread_mutex_lock(&mutexWater);
        sleep(1);
        pthread_mutex_lock(&mutexFuel);
    }
           
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexWater);
    fuel += 50;
    water = fuel;
    printf("increamented fuel to : %d\n set water to : %d\n", fuel, water);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexWater);
}


int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_mutex_init(&mutexWater, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&threads[i], NULL, &routine, NULL) != 0)
        {
            perror("Failed to create the thread");
        }
    }
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Failed to join the thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    printf("Water: %d\n", water);
    pthread_mutex_destroy(&mutexFuel);
    pthread_mutex_destroy(&mutexWater);
    return 0;
}