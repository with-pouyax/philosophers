/* 
- imagine we have sort of mail box, and we have a few listeners that all they do is
increment the number of mails.
- at the end once we are listening, once the thread finishes their execution, 
I want to just print the number of mails.
- if we run this, we will get expected result 2.
 */

int mails = 0;

void *routine(void *arg)
{
    mails++;
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
-but what happens if we increment it multiple times in the same thread?
- well since each thread is going to increment mails variable 100 times.
we should expect to see 200 mails, and we will get expected result of 200.
 */



int mails = 0;

void *routine(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        mails++;
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
- now lets increase the number of iterations to 100000
- we gonna get the expected result of 200000.
 */

int mails = 0;

void *routine(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        mails++;
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
- now what if we increase the number even further to 1000000
- now instead of getting 2000000, we get1411192
- why is that happening?
- that my friends is called race condition
- lets first understand what is happening behind the scenes.
- what we have to do to increment a variable?
- we have to read the value in mails and then increment it by 1 and then write it back
into the memory inside the mails variable.
- this is all ok if we work with one thread as we did so far.
- but as soon as we introduce multiple threads, things get complicated.
- so what is happening is imagine mail = 23 and it starts with t1, so read will be 23, in increment it will be also 23
- and then it will write it in mail and will be 24. again t2 will read 24, increment = 24, write = 25.
- now lets look at other example:
- we start again with 23, t1 => reads = 23, increment = 23, write = 24
- now imagine that for some reason comuter desides to pause t2 and run t1 again.
- and run t1 7 times before t2 gets a chance to run.
- so on the 7th time, t1 reads 29, increment = 29, write = 30.
-now t2 gets a chance to run, it will read 23 because it was paused and it will not
know that t1 has already incremented the value. but increment will be 30 because
t1 write = 30, so t2 increment = 30, but the write gonna be 24 because it will increment
1 to t2 read value. so now we suddenly jumped from 30 to 24 and we lost 6 mails. which is
persentage of 6/30 = 20%. which is a big issue. We call this a race condition.
- but you might say mail++; is not  3 operations (read, increment, write).
- ok we can take a look at the assembly code.
- to do that we can use gcc -s main.c, now if we go to the main.s file, we can see
L3:
    movl    mails(%rip), %eax
    addl    $1, %eax
    movl    %eax, mails(%rip)
which is mail++, so it is 3 operations.
- it can come to mind that, why only after 1 million iterations, we get this issue?
- if you have only 100 iterations, your gonna create the first tread, and all these 100, iterations
gonna be finished before the second thread is created, so you dont have anythign
interfering with each other.
- but even with 100 it can happen, but the chances are very low.
 */

int mails = 0;

void *routine(void *arg)
{
    for (int i = 0; i < 1000000; i++)
    {
        mails++;
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