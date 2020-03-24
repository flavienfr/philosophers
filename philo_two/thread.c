/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/24 17:56:23 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//protect all mutex woolim didn't ou juste pour debugg

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
	sem_wait(inf->sem_monit);
	if (!inf->end)
	{
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
	sem_post(inf->sem_monit);
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
		sem_wait(inf->sem_monit);
		if (inf->end && (phi->is_dead = 1))
		{
			sem_post(inf->sem_monit);
			pthread_join(phi->thread, NULL);
			sem_wait(inf->sem_monit);
			inf->end++;
			sem_post(inf->sem_monit);
			return (NULL);
		}
		if ((get_time(inf) - monit->lst_eat >= inf->ms_die) && (inf->end = 1))
			print(get_time(inf), phi->num + 1, DEAD);
		sem_post(inf->sem_monit);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_phi	*phi;
	t_inf	*inf;

	phi = arg;
	inf = phi->inf;
	while (1)
	{
		print_status(inf, phi, phi->monit, THINK);
		sem_wait(inf->sem_pick);
		sem_wait(inf->sem_fork);
		print_status(inf, phi, phi->monit, FORK_1);
		sem_wait(inf->sem_fork);
		print_status(inf, phi, phi->monit, FORK_2);
		sem_post(inf->sem_pick);
		print_status(inf, phi, phi->monit, EAT);
		usleep(inf->ms_eat);
		sem_post(inf->sem_fork);
		sem_post(inf->sem_fork);
		print_status(inf, phi, phi->monit, SLEEP);
		usleep(inf->ms_slp);
		if (phi->is_dead)
			return (NULL);
	}
	return (NULL);
}

int		launch_all(t_inf *inf, t_phi *phi)
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
		sem_wait(inf->sem_monit);
		end = inf->end;
		sem_post(inf->sem_monit);
	}
	return (EXIT_SUCCESS);
}
