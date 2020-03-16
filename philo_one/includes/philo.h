/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/03/16 17:24:05 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>	//write usleep
# include <stdlib.h>	//malloc EXIT_
# include <sys/time.h>	//time
# include <string.h>	//memset
# include <pthread.h>	//thread

# include <stdio.h>		//printf
# define DF printf("ici\n")

typedef struct	s_inf
{
	int				nb_phi;//5
	int				ms_die;//800
	int				ms_eat;//200
	int				ms_slp;//200
	int				nb_eat;
	long			ms;
	int				end;
	//int				eat_reach;
	pthread_mutex_t	mtx;
	pthread_t		clock;
	struct s_fork	*fork_1;
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;
}				t_inf;

typedef struct	s_monit
{
	pthread_t			thread;
	long int			lst_eat;
	//pthread_mutex_t	mtx_print;
	struct s_monit		*next;
}				t_monit;

typedef struct		s_phi
{
	int				num;
	int				status; // eat: 1 sleep: 2 think: 3
	int				nb_eat;
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

#endif