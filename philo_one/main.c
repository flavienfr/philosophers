/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/18 20:33:47 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//protect all mutex woolim didn't

t_fork		*select_fork(t_fork *fork, int num)
{
	int i;

	i = -1;
	while (++i < num)
		fork = fork->next;
	return (fork);
}

int		get_time(t_inf *inf)
{
	int	ms;

	if (gettimeofday(&inf->time, NULL))
		return (0);
	inf->time.tv_sec = inf->time.tv_sec;
	inf->time.tv_sec -= inf->time_start;
	ms = (1000 * inf->time.tv_sec) + (inf->time.tv_usec / 1000);
	return (ms);
}

void	print_status(t_inf *inf, t_phi *phi, t_monit *monit, int status)
{
	pthread_mutex_lock(&inf->mtx_monit);
	if (!inf->end)
	{
	if (status == EAT)
	{
		monit->lst_eat = get_time(inf);
		printf("%d %d is eating\n", get_time(inf), phi->num + 1);
		if (inf->nb_eat)
		{
			if (++phi->nb_eat == inf->nb_eat)
				inf->time_eat++;
			if (inf->time_eat == inf->nb_phi)
				inf->end = 1;
		}
	}
	else if (status == SLEEP)
		printf("%d %d is sleeping\n", get_time(inf), phi->num + 1);
	else if (status == THINK)
		printf("%d %d is thinking\n", get_time(inf), phi->num + 1);
	}
	pthread_mutex_unlock(&inf->mtx_monit);
}

void	*monitoring(void *arg)
{
	t_inf		*inf;
	t_monit		*monit;
	t_phi 		*phi;

	phi = arg;
	inf = phi->inf;
	monit = phi->monit;
	monit->lst_eat = get_time(inf);
	while (1)
	{
		pthread_mutex_lock(&inf->mtx_monit);
		if (inf->end)
		{
			phi->is_dead = 1;
			pthread_mutex_unlock(&inf->mtx_monit);
			pthread_join(phi->thread, NULL);
			return (NULL);
		}
		if (get_time(inf) - monit->lst_eat >= inf->ms_die)
		{
			printf("%d %d is died------------------\n", get_time(inf), phi->num + 1);
			inf->end = 1;
			phi->is_dead = 1;
			pthread_mutex_unlock(&inf->mtx_monit);
			pthread_join(phi->thread, NULL);
			return (NULL);
		}
		pthread_mutex_unlock(&inf->mtx_monit);
	}
	return (NULL);
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
		if (fork_1->is_fork && fork_2->is_fork)
		{
			//printf ("phi=%d EAT\n", phi->num);	
			fork_1->is_fork = 0;
			fork_2->is_fork = 0;
			pthread_mutex_lock(&fork_1->mtx);
			pthread_mutex_lock(&fork_2->mtx);
			pthread_mutex_unlock(&inf->mtx);
			

			print_status(inf, phi, phi->monit, EAT);
			usleep(inf->ms_eat);
			
			fork_1->is_fork = 1;
			pthread_mutex_unlock(&fork_1->mtx);
			usleep(1000);//is that betters
			fork_2->is_fork = 1;
			pthread_mutex_unlock(&fork_2->mtx);
			print_status(inf, phi, phi->monit, SLEEP);
			usleep(inf->ms_slp);
			print_status(inf, phi, phi->monit, THINK);
			//printf("Philo: %d ate=%d\n", phi->num, phi->nb_eat);
		}
		pthread_mutex_unlock(&inf->mtx);//protect ?
		
		pthread_mutex_lock(&inf->mtx_monit);
		if (phi->is_dead /*== inf->nb_phi*/)//ou juste inf->end
		{
			//printf("dead phi:%d\n", phi->num);
			pthread_mutex_unlock(&inf->mtx_monit);
			return (NULL);
		}
		pthread_mutex_unlock(&inf->mtx_monit);
	}
	return (NULL);
}

int		launch_all(t_inf *inf, t_phi *phi, t_monit *monit)
{
	while (phi)
	{
		if (pthread_create(&phi->thread, NULL, routine, phi))
			return (EXIT_FAILURE);
		if (pthread_create(&monit->thread, NULL, monitoring, phi))
			return (EXIT_FAILURE);
		phi = phi->next;
		monit = monit->next;
	}
	monit = inf->monit_1;
	while (monit)
	{
		if (pthread_join(monit->thread, NULL))
			return (EXIT_FAILURE);
		monit = monit->next;
	}
	return (EXIT_SUCCESS);
}
