/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:49:15 by froussel          #+#    #+#             */
/*   Updated: 2020/03/16 00:21:11 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_fork	*select_fork(t_fork *fork, int num)
{
	int i;

	i = -1;
	while (++i < num)
		fork = fork->next;
	return (fork);
}

void	*get_time(void *arg) //thread time
{
	t_inf 			*inf;
	struct timeval	time;
	long int		time_start;
	unsigned long	sc;

	inf = arg;
	if (gettimeofday(&time, NULL))
		return (NULL);
	time_start = time.tv_sec;
	while (1)
	{
		if (gettimeofday(&time, NULL))//why NULL
			return (NULL);//erreur
		sc = time.tv_sec;
		sc -= time_start;
		inf->ms = (1000 * sc) + (time.tv_usec / 1000);//can we reade and write at th same time
		//printf("time=%d\n", ms);
	}
	return (NULL);
}

void	*monitoring(void *arg)
{
	t_inf		*inf;
	t_monit		*monit;
	t_phi 		*phi;
	int 		old_stat;

	phi = arg;
	inf = phi->inf;
	monit = phi->monit;
	old_stat = 0;
	monit->lst_eat = inf->ms;
	while (1)
	{
		//printf("delay=%ld inf->ms_die=%d\n", inf->ms - monit->lst_eat, inf->ms_die);
		if (inf->ms - monit->lst_eat >= inf->ms_die)
		{
			printf("%ld %d is died------------------\n", inf->ms, phi->num);
			return (NULL);
		}
		if (phi->status && old_stat != phi->status)// mutext pour lire et ecrire status
		{
			old_stat = phi->status;
			if (phi->status == 1)
			{
				monit->lst_eat = inf->ms;
				printf("%ld %d is eating\n", inf->ms, phi->num);
			}
			else if (phi->status == 2)
				printf("%ld %d is sleeping\n", inf->ms, phi->num);
			else if (phi->status == 3)
				printf("%ld %d is thinking\n", inf->ms, phi->num);
		}
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
			
			phi->nb_eat++;
			phi->status = 1;
			usleep(inf->ms_eat);
			
			fork_1->is_fork = 1;
			pthread_mutex_unlock(&fork_1->mtx);
			usleep(1);
			fork_2->is_fork = 1;
			pthread_mutex_unlock(&fork_2->mtx);

			phi->status = 2;
			usleep(inf->ms_slp);
			phi->status = 3;
			//printf("Philo: %d ate=%d\n", phi->num, phi->nb_eat);
		}
		else
		{
			pthread_mutex_unlock(&inf->mtx);
			phi->status = 3;
		}
	}
	return (NULL);
}

int		free_all(t_inf *inf, int ret)
{
	//if (inf->forks)
	//	free(inf->forks);
	return (ret);
}

t_fork	*new_fork(void)
{
	t_fork *fork;

	if (!(fork = malloc(sizeof(*fork))))
		return (NULL);
	fork->is_fork = 1;
	fork->mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	fork->next = NULL;
	return (fork);
}

int		create_fork(t_inf *inf)
{
	t_fork	*cur;
	int		i;

	i = 0;
	inf->fork_1 = new_fork();
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
	if (!(phi->thread = malloc(sizeof(pthread_t))))//don't need to malloc thread
		return (NULL);
	phi->num = num;
	phi->status = 0;
	phi->inf = inf;
	phi->monit = monit;
	phi->next = NULL;
	return (phi);
}

t_monit	*new_monit(t_inf *inf, int num)
{
	t_monit	*monit;

	if (!(monit = malloc(sizeof(*monit))))
		return (NULL);
	if (!(monit->thread = malloc(sizeof(pthread_t))))
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
	inf->monit_1 = new_monit(inf, i); //protection pour tous
	inf->phi_1 = new_phi(inf, inf->monit_1, i);
	cur_p = inf->phi_1;
	cur_m = inf->monit_1;
	while (++i < inf->nb_phi)
	{
		if (!(cur_m->next = new_monit(inf, i)))
			return (1);
		if (!(cur_p->next = new_phi(inf, cur_m, i)))
			return (1);
		cur_p = cur_p->next;
		cur_m = cur_m->next;
	}
	return (0);
}

int		launch_all(t_inf *inf, t_phi *phi, t_monit *monit)
{
	if (pthread_create(&inf->clock, NULL, get_time, inf))
			return (EXIT_FAILURE);
	while (phi)
	{
		if (pthread_create(phi->thread, NULL, routine, phi))
			return (EXIT_FAILURE);
		if (pthread_create(phi->monit->thread, NULL, monitoring, phi))
			return (EXIT_FAILURE);
		phi = phi->next;
	}

	/*phi = inf->phi_1;
	while (phi)
	{
		if (pthread_create(phi->monit->thread, NULL, routine, phi))
			return (EXIT_FAILURE);
		phi = phi->next;
	}*/

	phi = inf->phi_1;
	while (phi)//attendre phi pour erreur ou
	{
		if (pthread_join(*phi->thread, NULL))
			return (EXIT_FAILURE);
		phi = phi->next;
	}
	return (EXIT_SUCCESS);
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
	inf.nb_eat = ac == 6 ? ft_atoi(av[5]) : -1;
	// init mutex gbl
	if (inf.nb_phi <= 0 || inf.ms_die < 0 || inf.ms_eat < 0
		|| inf.ms_slp < 0 || (ac == 6 && inf.nb_eat < 0))
		return (EXIT_FAILURE);
	inf.mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	if (create_fork(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	if (create_phi_monit(&inf))
		return (free_all(&inf, EXIT_FAILURE));
	return (launch_all(&inf, inf.phi_1, inf.monit_1));
}
