/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 10:47:13 by froussel          #+#    #+#             */
/*   Updated: 2020/06/21 15:00:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_phi	*new_phi(t_inf *inf, t_monit *monit, int num)
{
	t_phi	*phi;

	if (!(phi = malloc(sizeof(*phi))))
		return (NULL);
	phi->num = num;
	phi->nb_eat = 0;
	phi->is_dead = 0;
	phi->inf = inf;
	phi->monit = monit;
	phi->next = NULL;
	return (phi);
}

t_monit	*new_monit(void)
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
	if (!(inf->monit_1 = new_monit()))
		return (1);
	if (!(inf->phi_1 = new_phi(inf, inf->monit_1, i)))
		return (1);
	cur_p = inf->phi_1;
	cur_m = inf->monit_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur_m->next = new_monit()))
			return (1);
		if (!(cur_p->next = new_phi(inf, cur_m->next, i)))
			return (1);
		cur_p = cur_p->next;
		cur_m = cur_m->next;
	}
	return (0);
}

int		init_inf_fork(t_inf *inf)
{
	inf->end = 0;
	inf->time_eat = 0;
	if (!(inf->sem_pick = sem_open("/pickup", O_CREAT, 0777, 1)))
		return (1);
	if (!(inf->sem_monit = sem_open("/sem_monit", O_CREAT, 0777, 1)))
		return (1);
	if (!(inf->sem_fork = sem_open("/fork", O_CREAT, 0777, inf->nb_phi)))
		return (1);
	return (0);
}

int		main(int ac, char **av)
{
	t_inf	inf;

	sem_unlink("/pickup");
	sem_unlink("/sem_monit");
	sem_unlink("/fork");
	if (ac != 5 && ac != 6)
		return (EXIT_FAILURE);
	inf.nb_phi = ft_atoi(av[1]);
	inf.ms_die = ft_atoi(av[2]);
	inf.ms_eat = ft_atoi(av[3]) * 1000;
	inf.ms_slp = ft_atoi(av[4]) * 1000;
	inf.nb_eat = ac == 6 ? ft_atoi(av[5]) : 0;
	if (inf.nb_phi <= 1 || inf.ms_die <= 0 || inf.ms_eat <= 0
		|| inf.ms_slp <= 0 || (ac == 6 && inf.nb_eat <= 0))
		return (EXIT_FAILURE);
	if (init_inf_fork(&inf))
		return (free_all(&inf, free_all(&inf, EXIT_FAILURE)));
	if (create_phi_monit(&inf))
		return (free_all(&inf, free_all(&inf, EXIT_FAILURE)));
	if (launch_all(&inf, inf.phi_1))
		return (free_all(&inf, EXIT_FAILURE));
	return (free_all(&inf, EXIT_SUCCESS));
}
