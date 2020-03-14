/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldforg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/14 18:05:20 by froussel         ###   ########.fr       */
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

void	*routine(void *arg)
{
	t_inf *inf;

	inf = arg;
	pthread_mutex_lock(&inf->fork_1->mutex);
	usleep(10000000);
	printf ("je suis mutex\n");
	pthread_mutex_unlock(&inf->fork_1->mutex);
	printf ("je suis pas mutex\n");
	return (NULL);
}

int		free_all(t_inf *inf, int ret)
{
	t_fork	*next;

	/*if (inf->fork_1)
	{
		while (inf->fork_1)
		{
			next = inf->fork_1->next;
			free(inf->fork_1);
			//pthread_mutex_destroy(inf->fork_1->mutex);
			inf->fork_1 = next;
		}
	}*/
	return (ret);
}

t_fork	*new_fork(void)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(*fork))))
		return (NULL);
	fork->is_fork = 1;
	fork->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	fork->next = NULL;
	return (fork);
}

int		create_fork(t_inf *inf)
{
	t_fork	*cur;
	int		i;

	i = 1;
	if (inf->nb_phi)
		inf->fork_1 = new_fork();
	cur = inf->fork_1;
	while (i < inf->nb_phi)
	{
		if (!(cur->next = new_fork()))
			return (1);
		cur = cur->next;
		i++;
	}
	return (0);
}

int		create_philo(t_inf *inf)
{
	int			i;
	pthread_t	*phi_gp;

	if (!(phi_gp = malloc(sizeof(pthread_t) * inf->nb_phi)))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < inf->nb_phi)
		if (pthread_create(&phi_gp[i], NULL, routine, inf))
			return (EXIT_FAILURE);
	i = -1;
	while (++i < inf->nb_phi)
		if (pthread_join(phi_gp[i], NULL))
			return (EXIT_FAILURE);
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
	if (inf.nb_phi < 0 || inf.ms_die < 0 || inf.ms_eat < 0
		|| inf.ms_slp < 0 || (ac == 6 && inf.nb_eat < 0))
		return (EXIT_FAILURE);
	if (create_fork(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	return (create_philo(&inf));
	

	/*time
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	//printf("timestamp\nseconde=%d microseconde=%d\n", (int)tv.tv_sec, tv.tv_usec);
	*/
	
	return (EXIT_SUCCESS);
}

pthread_mutex_lock(&inf->mtx);
		if (fork_1->is_fork && fork_1->is_fork)
		{
			printf ("phi=%d EAT\n", phi->num);
			count[phi->num]++;

			pthread_mutex_lock(&fork_1->mtx);
			fork_1->is_fork = 0;
			pthread_mutex_unlock(&fork_1->mtx);
			pthread_mutex_lock(&fork_2->mtx);
			fork_2->is_fork = 0;
			pthread_mutex_unlock(&fork_2->mtx);

			pthread_mutex_unlock(&inf->mtx);
			//lst_eat = clock
			usleep(inf->ms_eat);
			pthread_mutex_lock(&fork_1->mtx);
			fork_1->is_fork = 1;
			pthread_mutex_unlock(&fork_1->mtx);
			pthread_mutex_lock(&fork_2->mtx);
			fork_2->is_fork = 1;
			pthread_mutex_unlock(&fork_2->mtx);
			usleep(inf->ms_slp);

			pthread_mutex_lock(&inf->mtx);
			printf("0=%d 1=%d 3=%d\n", count[0], count[1], count[2]);
			pthread_mutex_unlock(&inf->mtx);
		}
		else
			pthread_mutex_unlock(&inf->mtx);