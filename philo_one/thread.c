/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/19 21:11:56 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//protect all mutex woolim didn't ou juste pour debugg

t_fork	*select_fork(t_fork *fork, int num)
{
	int i;

	i = -1;
	while (++i < num)
		fork = fork->next;
	return (fork);
}

int		get_time(t_inf *inf)
{
	if (gettimeofday(&inf->time, NULL))
		return (0);//gestion erreur
	inf->time.tv_sec = inf->time.tv_sec;
	inf->time.tv_sec -= inf->time_start;
	return ((1000 * inf->time.tv_sec) + (inf->time.tv_usec / 1000));
}

void	print_status(t_inf *inf, t_phi *phi, t_monit *monit, int status)
{
	pthread_mutex_lock(&inf->mtx_monit);
	if (!inf->end && monit->lst_status != status)
	{
		monit->lst_status = status;
		if (status == EAT)
		{
			monit->lst_eat = get_time(inf);
			if (inf->nb_eat)
			{
				if (++phi->nb_eat == inf->nb_eat)
					inf->time_eat++;
				if (inf->time_eat == inf->nb_phi)
					inf->end = 1;
			}
		}
		print(get_time(inf), phi->num + 1, status);
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
		if (inf->end && (phi->is_dead = 1))
		{
			pthread_mutex_unlock(&inf->mtx_monit);
			pthread_join(phi->thread, NULL);
			pthread_mutex_lock(&inf->mtx_monit);
			inf->end++;
			pthread_mutex_unlock(&inf->mtx_monit);
			return (NULL);
		}
		if ((get_time(inf) - monit->lst_eat >= inf->ms_die) && (inf->end = 1))
			print(get_time(inf), phi->num + 1, DEAD);
		pthread_mutex_unlock(&inf->mtx_monit);
	}
	return (NULL);
}

void	eat_sleep(t_inf *inf, t_phi *phi, t_fork *fork_1, t_fork *fork_2)
{
	fork_1->is_fork = 0;
	pthread_mutex_lock(&fork_1->mtx);
	print_status(inf, phi, phi->monit, FORK_1);
	fork_2->is_fork = 0;
	pthread_mutex_lock(&fork_2->mtx);
	print_status(inf, phi, phi->monit, FORK_2);
	pthread_mutex_unlock(&inf->mtx);
	print_status(inf, phi, phi->monit, EAT);
	usleep(inf->ms_eat);
	print_status(inf, phi, phi->monit, SLEEP);
	fork_1->is_fork = 1;
	pthread_mutex_unlock(&fork_1->mtx);
	if (inf->nb_phi % 2)
		usleep(10000);//less or more
	fork_2->is_fork = 1;
	pthread_mutex_unlock(&fork_2->mtx);
	usleep(inf->ms_slp);
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
	while (1)
	{
		print_status(inf, phi, phi->monit, THINK);
		pthread_mutex_lock(&inf->mtx);
		if (fork_1->is_fork && fork_2->is_fork)
			eat_sleep(inf, phi, fork_1, fork_2);
		pthread_mutex_unlock(&inf->mtx);
		pthread_mutex_lock(&inf->mtx_monit);
		if (phi->is_dead)
		{
			pthread_mutex_unlock(&inf->mtx_monit);
			return (NULL);
		}
		pthread_mutex_unlock(&inf->mtx_monit);
	}
	return (NULL);
}

int		launch_all(t_inf *inf, t_phi *phi, t_monit *monit)
{
	int end;

	while (phi)
	{
		if (pthread_create(&phi->thread, NULL, routine, phi))
			return (EXIT_FAILURE);
		if (pthread_create(&phi->monit->thread, NULL, monitoring, phi))
			return (EXIT_FAILURE);
		if (pthread_detach(phi->monit->thread))
			return (EXIT_FAILURE);
		phi = phi->next;
	}
	end = 0;
	while (end != inf->nb_phi + 1)
	{
		pthread_mutex_lock(&inf->mtx_monit);
		end = inf->end;
		pthread_mutex_unlock(&inf->mtx_monit);
	}
	return (EXIT_SUCCESS);
}
