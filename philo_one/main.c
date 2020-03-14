/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/14 19:52:58 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
**	struct timeval 
**	{
**	    time_t      tv_sec;		secondes 
**	    suseconds_t tv_usec;	microsecondes 
**	};
*/

int count[3] = {0, 0, 0 };

t_fork	*select_fork(t_fork *fork, int num)
{
	int i;

	i = -1;
	while (++i < num)
		fork = fork->next;
	return (fork);
}

void	*routine(void *arg)
{
	t_phi	*phi;
	t_inf	*inf;
	t_fork	*fork_1;
	t_fork	*fork_2;

	phi = arg;
	inf = phi->inf;
	fork_1 = select_fork(inf->fork_1, phi->num);
	fork_2 = select_fork(inf->fork_1, (phi->num + 1) % inf->nb_phi);
	//printf("is=%d\n", inf->fork_1->is_fork);
	while (1)
	{
		pthread_mutex_lock(&inf->mtx);
			printf ("phi=%d EAT\n", phi->num);
			//count[phi->num]++;
			pthread_mutex_lock(&fork_1->mtx);
				//fork_1->is_fork = 0;
			pthread_mutex_lock(&fork_2->mtx);
				//fork_2->is_fork = 0;
			count[phi->num]++;
		pthread_mutex_unlock(&inf->mtx);
		//lst_eat = clock
		usleep(inf->ms_eat);
		pthread_mutex_unlock(&fork_1->mtx);
		pthread_mutex_unlock(&fork_2->mtx);
		usleep(inf->ms_slp);
		printf("0=%d 1=%d 3=%d\n", count[0], count[1], count[2]);
	}
	return (NULL);
}

int		free_all(t_inf *inf, int ret)
{
	//if (inf->forks)
	//	free(inf->forks);
	return (ret);
}

t_fork	*new_fork(void)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(*fork))))
		return (NULL);
	fork->is_fork = 1;
	fork->mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	fork->next = NULL;
	return (fork);
}

int		create_fork(t_inf *inf)
{
	t_fork	*cur;
	int		i;

	i = 0;
	inf->fork_1 = new_fork();
	cur = inf->fork_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur->next = new_fork()))
			return (1);
		cur = cur->next;
	}
	return (0);
}

t_phi	*new_phi(t_inf *inf, int num)
{
	t_phi	*phi;

	if (!(phi = malloc(sizeof(*phi))))
		return (NULL);
	if (!(phi->thread = malloc(sizeof(pthread_t))))
		return (NULL);
	phi->num = num;
	phi->inf = inf;
	phi->next = NULL;
	return (phi);
}

int		create_phi(t_inf *inf)
{
	t_phi	*cur;
	int		i;

	i = 0;
	inf->phi_1 = new_phi(inf, i);
	cur = inf->phi_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur->next = new_phi(inf, i)))
			return (1);
		cur = cur->next;
	}
	return (0);
}

int		launch_phi(t_inf *inf)
{
	t_phi	*cur;

	cur = inf->phi_1;
	while (cur)
	{
		if (pthread_create(cur->thread, NULL, routine, cur))
			return (EXIT_FAILURE);
		cur = cur->next;
	}

	cur = inf->phi_1;
	while (cur)
	{
		if (pthread_join(*cur->thread, NULL))
			return (EXIT_FAILURE);
		cur = cur->next;
	}
	return (EXIT_SUCCESS);
}

int		main(int ac, char **av)
{
	t_inf	inf;

	if (ac != 5 && ac != 6)
		return (EXIT_FAILURE);
	inf.nb_phi = ft_atoi(av[1]);
	inf.ms_die = ft_atoi(av[2]);
	inf.ms_eat = ft_atoi(av[3]);
	inf.ms_slp = ft_atoi(av[4]);
	inf.nb_eat = ac == 6 ? ft_atoi(av[5]) : -1;
	// init mutex gbl
	if (inf.nb_phi <= 0 || inf.ms_die < 0 || inf.ms_eat < 0
		|| inf.ms_slp < 0 || (ac == 6 && inf.nb_eat < 0))
		return (EXIT_FAILURE);
	inf.mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	if (create_fork(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	if (create_phi(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	return (launch_phi(&inf));
}

	
/*test phi  2 fork
	pthread_mutex_lock(&inf->mtx);
	printf ("je suis phi=%d\n", phi->num);
	printf ("fork right=%d\n", phi->num);
	printf ("fork left=%d\n\n", (phi->num + 1) % inf->nb_phi);
	pthread_mutex_unlock(&inf->mtx);*/

/*time
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	//printf("timestamp\nseconde=%d microseconde=%d\n", (int)tv.tv_sec, tv.tv_usec);
*/
