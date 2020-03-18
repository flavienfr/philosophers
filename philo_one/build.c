/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 10:47:13 by froussel          #+#    #+#             */
/*   Updated: 2020/03/18 16:12:40 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

t_fork	*new_fork(void)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(*fork))))
		return (NULL);
	fork->is_fork = 1;
	if (pthread_mutex_init(&fork->mtx, NULL))
		return (NULL);
	fork->next = NULL;
	return (fork);
}

int		create_fork(t_inf *inf)
{
	t_fork	*cur;
	int		i;

	i = 0;
	if (!(inf->fork_1 = new_fork()))
		return (1);
	cur = inf->fork_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur->next = new_fork()))
			return (1);
		cur = cur->next;
	}
	return (0);
}

t_phi	*new_phi(t_inf *inf, t_monit *monit, int num)
{
	t_phi	*phi;

	if (!(phi = malloc(sizeof(*phi))))
		return (NULL);
	phi->num = num;
	phi->status = 0;
	phi->inf = inf;
	phi->monit = monit;
	phi->next = NULL;
	return (phi);
}

t_monit	*new_monit()
{
	t_monit	*monit;

	if (!(monit = malloc(sizeof(*monit))))
		return (NULL);
	monit->next = NULL;
	return (monit);
}

int		create_phi_monit(t_inf *inf)
{
	t_phi	*cur_p;
	t_monit	*cur_m;
	int		i;

	i = 0;
	if (!(inf->monit_1 = new_monit(inf, i)))
		return (1);
	if (!(inf->phi_1 = new_phi(inf, inf->monit_1, i)))
		return (1);
	cur_p = inf->phi_1;
	cur_m = inf->monit_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur_m->next = new_monit(inf, i)))
			return (1);
		if (!(cur_p->next = new_phi(inf, cur_m, i)))
			return (1);
		cur_p->is_dead = 0;
		cur_p = cur_p->next;
		cur_m = cur_m->next;
	}
	return (0);
}

int		init_inf(t_inf *inf)
{
	inf->end = 0;
	inf->time_eat = 0;
	if (pthread_mutex_init(&inf->mtx_monit, NULL))
		return (1);
	if (pthread_mutex_init(&inf->mtx, NULL))
		return (1);
	if (gettimeofday(&inf->time, NULL))
		return (EXIT_FAILURE);
	inf->time_start = inf->time.tv_sec;
	return (0);
}

int		main(int ac, char **av)
{
	t_inf	inf;

	if (ac != 5 && ac != 6)
		return (EXIT_FAILURE);
	inf.nb_phi = ft_atoi(av[1]);
	inf.ms_die = ft_atoi(av[2]);
	inf.ms_eat = ft_atoi(av[3]) * 1000;
	inf.ms_slp = ft_atoi(av[4]) * 1000;
	inf.nb_eat = ac == 6 ? ft_atoi(av[5]) : 0;
	if (inf.nb_phi <= 0 || inf.ms_die <= 0 || inf.ms_eat <= 0
		|| inf.ms_slp <= 0 || (ac == 6 && inf.nb_eat <= 0))
		return (EXIT_FAILURE);
	if (init_inf(&inf))
		return (free_all(&inf, free_all(&inf, EXIT_FAILURE)));
	if (create_fork(&inf))
		return (free_all(&inf, free_all(&inf, EXIT_FAILURE)));
	if (create_phi_monit(&inf))
		return (free_all(&inf, free_all(&inf, EXIT_FAILURE)));
	if (launch_all(&inf, inf.phi_1, inf.monit_1))
		return (free_all(&inf, EXIT_FAILURE));
	return (free_all(&inf, EXIT_SUCCESS));
}