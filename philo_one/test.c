# include <unistd.h>	//write usleep
# include <stdlib.h>	//malloc EXIT_
# include <sys/time.h>	//time
# include <string.h>	//memset
# include <pthread.h>	//thread

# include <stdio.h>		//printf
# include <errno.h>

void	*routine2(void *qrg)
{
	printf("je suis dans le thread2\n");
	return(NULL);
}

void	*routine(void *arg)
{
	int err;
	pthread_t *thread_2;

	thread_2 = arg;
	err = pthread_create(thread_2, NULL, routine2, NULL);
	printf("creation=%s\n", strerror(err));

	printf("je suis dans le thread\n");
	return (NULL);
}

int main ()
{
	int err;
	pthread_t	thread_1;
	pthread_t	thread_2;
	
	err = pthread_create(&thread_1, NULL, routine, &thread_2);
	printf("creation=%s\n", strerror(err));
	//err = pthread_create(&thread_2, NULL, routine, NULL);
	//printf("creation=%s\n", strerror(err));
	usleep(1000000);
	err = pthread_join(thread_1, NULL);
	printf("join=%s\n", strerror(err));
	usleep(1000000);
	err = pthread_join(thread_2, NULL);
	printf("join=%s\n", strerror(err));
	return (0);
}
