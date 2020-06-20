/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 17:41:12 by froussel          #+#    #+#             */
/*   Updated: 2020/06/21 00:19:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_monit		*new_monit(void)
{
	t_monit	*monit;

	if (!(monit = malloc(sizeof(*monit))))
		return (NULL);
	monit->next = NULL;
	return (monit);
}

t_fork		*select_fork(t_fork *fork, int num)
{
	int i;

	i = -1;
	while (++i < num)
		fork = fork->next;
	return (fork);
}

void	ft_usleep(uint64_t us)
{
	uint64_t target;

	target = get_the_time() + (us / 1000);
	while (get_the_time() < target)
		usleep(50);
}

uint64_t	get_the_time(void)
{
	struct timeval time;

	if (gettimeofday(&time, NULL))
		return (0);
	return ((1000 * time.tv_sec) + (time.tv_usec / 1000));
}

uint64_t	get_time(uint64_t time_start)
{
	return (get_the_time() - time_start);
}

int			free_all(t_inf *inf, int ret)
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
