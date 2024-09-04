/* 
- at first glance threads and processes are look similar,
- in both we execute some code at the same time.
- with threads, we have a function that we want to run in parallel.
- with processes, we have a forked process that we want to run in parallel.
-to be able to understand this I have to watch processes playlist.
-s so we need two files, one for processes and one for threads.
- so now in tread file, as result I will see two messages "test from threads" and "End of thread".
- and in process file, as result I will see twice "Hello from processes".

 */


void *routine()
{
    printf("test from threads\n");
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_t t2;

    if (pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&t2, NULL, &routine, NULL))
        return 1;
    if (pthread_join(t1, NULL))
        return 2;
    if (pthread_join(t2, NULL))
        return 2;
    return 0;
}


//==============================================================================

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    printf("Hello from processes\n");
    if (pid != 0)
    {
        wait(NULL);
    }
    return 0;
}

/* 
- so now let's say that I want first in process file print the process ID's. 
- so now if I lunch it, I will see two different process ID's.
 */

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    printf("process ID: %d\n", getpid());
    if (pid != 0)
    {
        wait(NULL);
    }
    return 0;
}
/* 
- what happens if I do the same in threads file?
- I will notic that we get the same process ID.
- why is that?
- first because processes can contain multiple threads, but a thread cannot contain multiple processes.
- second, regarding the address space, if you remember buy each fork
we used to say when we are forking, we are duplicating all the variables into the 
child process, and we can modify them individually.
- but with threads, there is a big difference. we have all the variables in the same
place, so all threads can access all the variables and they all have a common address space.
 */

void *routine()
{
    printf("process ID: %d\n", getpid());
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_t t2;

    if (pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&t2, NULL, &routine, NULL))
        return 1;
    if (pthread_join(t1, NULL))
        return 2;
    if (pthread_join(t2, NULL))
        return 2;
    return 0;
}
/* 
to understand better.
- we add x
- we initialize x to 2.
- we increment x by 1 in the child process.
- we use sleep function to make the main process wait for 2 seconds.
- and then we print the value of x.
- so now if I run the program, I will get once x = 2 and once x = 3.
 */

int main(int argc, char *argv[])
{
    int x = 2;
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    if (pid == 0)
    {
        x++;
    }
    sleep(2);
    printf("x = %d\n", x);
    if (pid != 0)
    {
        wait(NULL);
    }
    return 0;
}

/* 
- now if I do the same in threads file:
- from this new code, I will get x = 3 twice.
- why is that?
- because threads share the same address space.
- that is one of the bigest diffrences between processes and threads.
- other diffrence is trheads are sharing the file handlers but processes are not.
- actually, threads are sharing all system resources, while processes are duplicating them.


 */

int x = 2;

void *routine()
{
    x++;
    sleep(2);
    printf("x = %d\n", x);
}

void *routine2()
{
    sleep(2);
    printf("x = %d\n", x);
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_t t2;

    if (pthread_create(&t1, NULL, &routine, NULL))
        return 1;
    if (pthread_create(&t2, NULL, &routine2, NULL))
        return 1;
    if (pthread_join(t1, NULL))
        return 2;
    if (pthread_join(t2, NULL))
        return 2;
    return 0;
}