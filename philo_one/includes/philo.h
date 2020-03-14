/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/03/14 17:13:08 by froussel         ###   ########.fr       */
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
# define DF printf("line\n")

typedef struct	s_inf
{
	int				nb_phi;//5
	int				ms_die;//800
	int				ms_eat;//200
	int				ms_slp;//200
	int				nb_eat;
	int				*forks;
	pthread_mutex_t	mtx;
	struct s_phi	*phi_1;
}				t_inf;

typedef struct		s_phi
{
	int				num;
	int				lst_eat;
	pthread_t		*thread;
	struct s_inf	*inf;
	struct s_phi	*next;
}					t_phi;

/*typedef struct	s_fork
{
	int				is_fork;
	pthread_mutex_t	mutex;
	struct s_fork	*next;
}				t_fork;*/

/*
**	utils.c
*/
int				ft_atoi(const char *str);

#endif