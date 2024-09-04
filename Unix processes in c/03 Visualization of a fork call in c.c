/* 
- so here the guy is trying to visualize the fork call in c, which is very  helpful.
- I cant make notes out of this, but come back to this video whenevr you get confused about fork() function.
 */

int main(int argc, char *argv[])
{
    int id = fork(); // it gonna make a child process from the main process. and that chile process is very important because it has exact same memory as as the main process.
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    if (id == 0){
        printf("\n");
    }
    return 0;
}
    