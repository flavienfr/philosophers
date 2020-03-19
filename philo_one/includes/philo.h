/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/03/19 19:17:57 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>	//write usleep
# include <stdlib.h>	//malloc EXIT_
# include <sys/time.h>	//time
# include <pthread.h>	//thread

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK_1 4
# define FORK_2 5

# include <stdio.h>		//printf
//# include <errno.h>
//# include <string.h>	//memset
# define DF printf("ici\n")

typedef struct	s_inf
{
	int				nb_phi;
	int				ms_die;
	int				ms_eat;
	int				ms_slp;
	int				nb_eat;
	int				time_eat;
	int				end;
	struct timeval	time;
	long			time_start;
	pthread_mutex_t	mtx;
	pthread_mutex_t	mtx_monit;
	struct s_fork	*fork_1;
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;
}				t_inf;

typedef struct	s_monit
{
	pthread_t			thread;
	int					lst_status;
	int					lst_eat;
	struct s_monit		*next;
}				t_monit;

typedef struct		s_phi
{
	int				num;
	int				nb_eat;
	int				is_dead;
	pthread_t		thread;
	struct s_inf	*inf;
	struct s_monit	*monit;
	struct s_phi	*next;
}					t_phi;

typedef struct	s_fork
{
	int				is_fork;
	pthread_mutex_t	mtx;
	struct s_fork	*next;
}				t_fork;

/*
**	utils.c
*/
int				ft_atoi(const char *str);

/*
**	build.c
*/
int			launch_all(t_inf *inf, t_phi *phi, t_monit *monit);

#endif