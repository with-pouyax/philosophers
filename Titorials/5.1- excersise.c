#include <stdio.h>
#include <pthread.h>

int mail = 0;
pthread_mutex_t mutex;

void *routine()
{
    for(int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mail++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t th[4];

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, NULL))
            return 1;
    }
     for (int i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL))
            return 1;
    }
    pthread_mutex_destroy(&mutex);
    printf("mails = %d", mail);
    
}
// -next time do it norminette friendly.