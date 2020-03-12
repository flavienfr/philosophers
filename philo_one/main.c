/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/12 21:56:20 by froussel         ###   ########.fr       */
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
	t_phi	*phi;
	int		*forks;
	
	forks = phi->forks;	
	pthread_mutex_lock(&phi->pickup);
	/*if (forks[phi->num] && forks[(phi->num + 1) % phi->num])
	{
		forks[phi->num] = 0;
		forks[(phi->num + 1) % phi->num] = 0;
		pthread_mutex_unlock(&phi->pickup);
		printf("time toeat\n");
	}
	else*/
		pthread_mutex_unlock(&phi->pickup);
	//lock
	// if n && n+1 % nb_phil
	//	take forks
	//	unlock
	//	eat -> delay et liberer fork plus petite et delay puis l'autre
	//else
	//	unlock
	printf ("je suis pas mutex\n");
	return (NULL);
}

int		free_all(t_inf *inf, int ret)
{
	//if (inf->forks)
	//	free(inf->forks);
	return (ret);
}

int		create_fork(t_inf *inf)//remettre struct 
{
	int	i;

	if (!(inf->forks = malloc(sizeof(int) * inf->nb_phi)))
		return (1);
	i = -1;
	while (++i < inf->nb_phi)
		inf->forks[i] = 1;
	return (0);
}

t_phi	*new_phi(t_inf *inf, int num)
{
	t_phi	*phi;

	if (!(phi = malloc(sizeof(*phi))))
		return (NULL);
	if (!(phi->thread = malloc(sizeof(pthread_t))))
		return (NULL);
	phi->pickup = &inf->thread;
	phi->num = &phi->mtx_gbl;
	phi->forks = inf->forks;
	phi->next = NULL;
	return (phi);
}

int		create_phi(t_inf *inf)
{
	t_phi	*cur;
	int		i;

	i = 0;
	if (inf->nb_phi)
		inf->phi_1 = new_phi(inf, i);
	cur = inf->phi_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur->next = new_phi(inf, i)))
			return (1);
		cur = cur->next;
	}
	return (0);
}

int		launch_phi(t_inf *inf)
{
	t_phi	*cur;
	int		i;

	i = -1;
	cur = inf->phi_1;
	while (cur)
	{
		if (pthread_create(cur->thread, NULL, routine, cur))
			return (EXIT_FAILURE);
		cur = cur->next;
	}
	/*i = -1;
	while (++i < inf->nb_phi)
		if (pthread_join(phi_gp[i], NULL))
			return (EXIT_FAILURE);*/
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
	// init mutex gbl
	if (inf.nb_phi < 0 || inf.ms_die < 0 || inf.ms_eat < 0
		|| inf.ms_slp < 0 || (ac == 6 && inf.nb_eat < 0))
		return (EXIT_FAILURE);
	if (create_fork(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	if (create_phi(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	return (launch_phi(&inf));
}

/*time
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	//printf("timestamp\nseconde=%d microseconde=%d\n", (int)tv.tv_sec, tv.tv_usec);
*/
