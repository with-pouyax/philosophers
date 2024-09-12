/*
- we have a function  here called roll_dice, what thread are going to do is they are going to roll a dice and return the result.
- and in main function I am expecting to print that result.
- first lets roll the dice in the main function.
*/

void *roll_dice(void *arg)
{

}

int main(int argc, char *argv[])
{
    pthread_t th;
    if (pthread_create(&th, NULL, &routine, NULL))
        return 1;
    if (pthread_join(th, NULL) != 0)
        return 2;
    return 0;
}

/* 
- first lets roll the dice in the main function.
-now we recieve random numbers between 1 and 6.
 */

void *roll_dice(void *arg)
{
    int value = rand() % 6 + 1; // this will generate a random number between 1 and 6
    printf("The dice rolled %d\n", value);
}

int main(int argc, char *argv[])
{
    srand(time(NULL)); // it will seed the random number generator
    pthread_t th;
    if (pthread_create(&th, NULL, &routine, NULL))
        return 1;
    if (pthread_join(th, NULL) != 0)
        return 2;
    return 0;
}

/* 
 now how can I get the value in thread function into the main function?
- the answere is about second argument of the pthread_join function.
- the prototype of the pthread_join function is:
int pthread_join(pthread_t thread, void **retval);
- void **retval is double pointer to void, simnply because:
- if you notice roll_dice function, we difined it as void pointer as the return type.
- that is not randomly chosen, that is basically what pthread_join is expecting and what it is going to use to send the result back.
- so what pthread_join do is taking a reference to pointer, or a pointer to pointer or double pointer, whatever you want to call it.
and it is going to set the value from within it to whatever value we want to return from the thread.
- so if I make an integer pointer in my main function and pass it to the pthread_join function as a reference, it gonna be a double pointer
to int, but remember void pointers are sort of considered as wildcard, they can be anything. so as long as we cast it to the correct type,
it will be fine.
- ok but, how can I get it from our thread function? because we need to return a void pointer. we cant just return an integer.
- we can cast a reference to pour value to a void pointer and return that and print it in the main function.
- but then we'll recieve segmentation fault
- why? I returend a reference to a value that is local to the thread function.
- by local I mean that it is on the stack of the thread function, and when the thread function finishes, that stack is going to be deallocated.

 */

void *roll_dice(void *arg)
{
    int value = rand() % 6 + 1; // this will generate a random number between 1 and 6
    printf("The dice rolled %d\n", value);
    return (void*) &value;
}

int main(int argc, char *argv[])
{
    int *res;
    srand(time(NULL)); // it will seed the random number generator
    pthread_t th;
    if (pthread_create(&th, NULL, &routine, NULL))
        return 1;
    if (pthread_join(th, (void**) &res) != 0)
        return 2;
    printf("The dice rolled %d\n", *res);
    return 0;
}

/* 
- so what we have to do is we have to dynamically allocate memory for that value.
- now it will print the correct value.
- but we have to free the memory that we allocated.
- it was not the best idea to allocate memory in the thread function, and free it in the main function. because in big programs, 
it is hard to keep track of all the memory that we allocated. in next file we will see a better way to do this.`

 
 */

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