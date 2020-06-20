/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/06/20 22:15:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK_1 4
# define FORK_2 5
# define DEAD 6

typedef struct	s_inf
{
	int				nb_phi;
	int				ms_die;
	int				ms_eat;
	int				ms_slp;
	int				nb_eat;
	int				time_eat;
	int				end;
	int				nb_fork;
	struct timeval	time;
	long			time_start;
	sem_t			*sem_pick;
	sem_t			*sem_monit;
	sem_t			*sem_fork;
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;
}				t_inf;

typedef struct	s_monit
{
	pthread_t		thread;
	int				lst_eat;
	struct s_monit	*next;
}				t_monit;

typedef struct	s_phi
{
	int				num;
	int				nb_eat;
	int				is_dead;
	pthread_t		thread;
	struct s_inf	*inf;
	struct s_monit	*monit;
	struct s_phi	*next;
}				t_phi;

/*
**	utils.c
*/
int				ft_atoi(const char *str);
char			*ft_itoa(int n);
void			ft_putstr(char *s);

/*
**	thread.c
*/
int				launch_all(t_inf *inf, t_phi *phi);

/*
**	print_free.c
*/
void			print(int time, int num, int status);
int				free_all(t_inf *inf, int ret);

#endif
