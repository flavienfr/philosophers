# include <unistd.h>	//write usleep
# include <stdlib.h>	//malloc EXIT_
# include <sys/time.h>	//time
# include <pthread.h>	//thread
# include <stdio.h>		//printf
# include <errno.h>
# include <string.h>

#include <fcntl.h> //O_CREAT
//#include <sys/stat.h>        /* Pour les constantes « mode » */
#include <semaphore.h>
#include <signal.h>

pthread_t   th_1;
sem_t       *test;

void    signal_handler(int signo)
{
	if (signo == SIGINT)
    {
        printf("  <-Fin\n");
        sem_close(test);
        sem_unlink("0");//012
        exit(1);
    }
}

void    *routine(void *arg)
{
    while (1)
    {
        sem_wait(test);
        printf("Salut\n");
        //sem_post(test);
    }
}

int     main()
{
    //sem_unlink("0");
    signal(SIGINT, signal_handler);
    if (!(test = sem_open("0", O_CREAT, 0777, 2)))
    {
        printf("%s\n", strerror(errno));
        sem_close(test);
        sem_unlink("0");
        return (EXIT_FAILURE);
    }
    if (pthread_create(&th_1, NULL, routine, NULL))
		return (EXIT_FAILURE);
    pthread_join(th_1, NULL);
    return (0);
}