/* 
- now we will see how to pass arguments to threads in c
- in certain point, you might also want to do something based on the thread creation order. how can you do that with pthreads API?
- first lets start with creating a problem.
- imagine we have a global array of prime numbers.
- the problem is : create 10 threads, each taking a unique prime from the primes array and print it to screen.
- first using loop, we will create 10 threads
- then using other loop, we will join the threads, that means we are waiting for the threads to finish.
- then we wll create a routine function, and this time I will pass it a void pointer as an argument.
- then using printf I want to print primes[], but primes of what ? 
- so in main first I have to pass pthread_create a right argument, so I pass i as reference.
- then in routine I can say int index = *arg, but we cant pass it like that and we have to dereference it.
-  we have to actually cast it first to an integer pointer and then dereference it.
-  so now if I run this, what I see is : 
3 5 11 13 13 17 17 29 29 0
- so what is happening here?
- if we add a sleep, it will be even worse. I will get :
0 0 0 0 0 0 0 0 0 0
- so what is happening here?
 */


int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void *routine(void *arg)
{
    sleep(1);
    int index = *(int*) arg;
    printf("The prime number is %d\n", primes[index]);
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t th[10];
    int index = *(int*) arg;
    for (i = 0; i < 10; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, &i))
            perror("failed to create thread");
    }
    for (i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL))
            perror("failed to join thread");
    }
    return 0;
}

/* 
- the issue is we are passing the memory address, instead of the values.
- since we are passing the memory address, the value at those memory addresses could change in the meantime that they are trying
to executing.
- so between the time the tread is created and the tread got started, the value inside the address that we are referencing has changed.
- for example here : if (pthread_create(&th[i], NULL, &routine, &i)), when we called pthread first time, we passed an address to i.
and i at that time was 0, so if right after that, the tread that we have created starts executing, index gonna be alright, because
i is still 0, and every time, the same should happen.
- but the problem is that if we for example get first pthread_create, and i is 0 for that pthread_create, but the thread is not started yet,
and then the second thread is created, so i is getting incremented here, so if i gett incremented here, then we gonna create another thread
and passing the adress to i, and now both threads gonna have the same i value, if they started right after second thread has started executing.
- the issue here is multi-threaded nature of how this i here does change irrelevent of what's executed in the thread.
- specially in the case I had 0 0 0 0 0 0 0 0 0 0, I can find out what is happening if I print out instead of the prime number, the index.
now you will see 10 10 10 10 10 10 10 10 10 10.
- why is this happening?
- we have here 10 pthread_create calls, so we 10 threads that get created, but they dont really start doing anything because they sleep
for 1 second, and in that one second all the threads are created, so  in pthread_create loop, i went from 0 to 9, and then all the threads
started executing, and they all got the same value of i, which is 10. and that is why we see 10 10 10 10 10 10 10 10 10 10. and the value
for all was 0 because after the last member array in memory, there is a 0. because there were 0 in the memory address that we were referencing.
why 0 ? it is garbage value, it is not initialized.
 */

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void *routine(void *arg)
{
    sleep(1);
    int index = *(int*) arg;
    printf("%d\n", index);
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t th[10];
    int index = *(int*) arg;
    for (i = 0; i < 10; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, &i))
            perror("failed to create thread");
    }
    for (i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL))
            perror("failed to join thread");
    }
    return 0;
}

/* 
- the most general way to solve this issue is to dynamically allocate memory for this i that we are passing to the thread, for each thread.
-  int *a = malloc(sizeof(int));
        *a = i;
- now by every iteration, we allocate separate memory for each thread
- but we cant free in this loop.
- imagine we free it at the end of for loop, it is what is happening :
- 1- we create a thread gets created, and sleep for 1 second.
- 2- then the allocated memory gets freed.
- 3- then the new thread that gets created, tries to dereference the memory that has been freed.
and everythings breaks
- we  have to free memory inside the thread function that we have created.
- now if we run this, we will see the correct output.
- and we donnt need sleep anymore.
- the result will be :
3 2 5 7 11 13 17 19 23 29
- as you can see the order is not the same as the order of the primes array, because the threads are running in parallel, but it only one
prime per thread. and it is what we expected.
 */


int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void *routine(void *arg)
{
    sleep(1);
    int index = *(int*) arg;
    printf("The prime number is %d\n", primes[index]);
    free(arg);
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t th[10];
    int i;
    for (i = 0; i < 10; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a))
            perror("failed to create thread");
    }
    for (i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL))
            perror("failed to join thread");
    }
    return 0;
}

/* 
- as homework, there is a much easier soloution to this problem, which has to do with the fact that we are passing the address.
- write it dow belloow.
 */


/* 
In the original problem, we encountered an issue because we were passing the memory address of the loop variable i to the threads.
This caused all threads to receive the same value of i after the loop had completed, due to the way memory addresses and the loop work.

One way to fix this was to dynamically allocate memory for each thread, but a simpler solution is to avoid passing the memory address
altogether and instead pass the value of i directly. Since we are working with integers, we can pass them as void* by casting them.
In the thread function, we can cast this void* back to an integer.

This approach avoids the need for dynamic memory allocation (malloc) and free, and works by casting the integer i to void* when calling
 pthread_create, and then casting it back to an integer in the thread function.
 */

