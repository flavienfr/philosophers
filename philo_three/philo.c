/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/06/21 15:12:04 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_inf *inf, t_phi *phi, t_monit *monit, int status)
{
	sem_wait(inf->sem_monit);
	if (!phi->is_dead)
	{
		if (status == EAT)
			monit->lst_eat = get_time(inf->time_start);
		print(get_time(inf->time_start), phi->num + 1, status);
	}
	sem_post(inf->sem_monit);
}

void	*monitoring(void *arg)
{
	t_inf	*inf;
	t_monit	*monit;
	t_phi	*phi;

	phi = arg;
	inf = phi->inf;
	monit = phi->monit;
	monit->lst_eat = get_time(inf->time_start);
	while (1)
	{
		sem_wait(inf->sem_monit);
		if ((get_time(inf->time_start) - monit->lst_eat
			>= (uint64_t)inf->ms_die))
		{
			phi->is_dead = 1;
			print(get_time(inf->time_start), phi->num + 1, DEAD);
			exit(TIME_DEATH);
		}
		sem_post(inf->sem_monit);
	}
}

void	routine(t_phi *phi)
{
	t_inf	*inf;

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
		sem_wait(inf->sem_monit);
		if (++phi->nb_eat == inf->nb_eat)
			exit(NB_EAT_DEATH);
		sem_post(inf->sem_monit);
		print_status(inf, phi, phi->monit, SLEEP);
		usleep(inf->ms_slp);
	}
	exit(0);
}

int		launch_all(t_inf *inf, t_phi *phi)
{
	int		i;

	i = -1;
	inf->time_start = get_the_time();
	while (phi)
	{
		if ((inf->pid_tab[++i] = fork()) == 0)
		{
			if (pthread_create(&phi->monit->thread, NULL, monitoring, phi))
				return (EXIT_FAILURE);
			if (pthread_detach(phi->monit->thread))
				return (EXIT_FAILURE);
			routine(phi);
		}
		else if (inf->pid_tab[i] == -1)
			return (EXIT_FAILURE);
		phi = phi->next;
	}
	return (EXIT_SUCCESS);
}
