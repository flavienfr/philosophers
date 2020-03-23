/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/23 20:12:59 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//protect all mutex woolim didn't ou juste pour debugg

int		get_time(t_inf *inf)
{
	if (gettimeofday(&inf->time, NULL))
		return (0);//gestion erreur
	//inf->time.tv_sec = inf->time.tv_sec;// dell ca partout 
	inf->time.tv_sec -= inf->time_start;
	return ((1000 * inf->time.tv_sec) + (inf->time.tv_usec / 1000));
}

void	print_status(t_inf *inf, t_phi *phi, t_monit *monit, int status)
{
	sem_wait(inf->sem_monit);
	if (!phi->is_dead)
	{
		if (status == EAT)
			monit->lst_eat = get_time(inf);
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
		if ((get_time(inf) - monit->lst_eat >= inf->ms_die))
		{
			phi->is_dead = 1;
			print(get_time(inf), phi->num + 1, DEAD);
			exit(TIME_DEATH);
		}
		sem_post(inf->sem_monit);

	}
	return (NULL);
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
		print_status(inf, phi, phi->monit, SLEEP);
		usleep(inf->ms_slp);
		if (phi->is_dead)
			exit(0);
	}
	exit(0);
}

int		launch_all(t_inf *inf, t_phi *phi)
{
	int		i;
	int		status;

	i = -1;
	while (phi)
	{
		if ((inf->pid_tab[++i] = fork()) == 0)//-1 erreur
		{
			if (pthread_create(&phi->monit->thread, NULL, monitoring, phi))
				return (EXIT_FAILURE);
			if (pthread_detach(phi->monit->thread))
				return (EXIT_FAILURE);
			routine(phi);
		}
		phi = phi->next;
	}
	waitpid(-1, &status, 0);//check return
	i = -1;
	while (++i < inf->nb_phi)
		kill(inf->pid_tab[i], SIGINT);
	return (EXIT_SUCCESS);
}
