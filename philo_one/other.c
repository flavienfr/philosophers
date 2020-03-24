/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ohter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 17:41:12 by froussel          #+#    #+#             */
/*   Updated: 2020/03/24 17:47:30 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_monit	*new_monit()
{
	t_monit	*monit;

	if (!(monit = malloc(sizeof(*monit))))
		return (NULL);
	monit->next = NULL;
	return (monit);
}

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

int		free_all(t_inf *inf, int ret)
{
	t_fork	*nxt_fork;
	t_phi	*nxt_phi;

	while (inf->fork_1)
	{
		nxt_fork = inf->fork_1->next;
		if (pthread_mutex_destroy(&inf->fork_1->mtx))
			return (EXIT_FAILURE);
		free(inf->fork_1);
		inf->fork_1 = nxt_fork;
	}
	while (inf->phi_1)
	{
		free(inf->phi_1->monit);
		nxt_phi = inf->phi_1->next;
		free(inf->phi_1);
		inf->phi_1 = nxt_phi;
	}
	if (pthread_mutex_destroy(&inf->mtx))
		return (EXIT_FAILURE);
	if (pthread_mutex_destroy(&inf->mtx_monit))
		return (EXIT_FAILURE);
	return (ret);
}
