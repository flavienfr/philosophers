/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/03/24 14:54:35 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>	//write usleep
# include <stdlib.h>	//malloc EXIT_
# include <sys/time.h>	//time
# include <pthread.h>	//thread
# include <semaphore.h>	//semaphore
# include <fcntl.h> 	//O_CREAT
#include <sys/types.h>	//waitpid kill
#include <sys/wait.h>	//waitpid
#include <signal.h>		//kill

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK_1 4
# define FORK_2 5
# define DEAD 6
# define TIME_DEATH 10
# define NB_EAT_DEATH 11

# include <stdio.h>		//printf
# include <errno.h>		//strerror(errno)
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
	pid_t			*pid_tab;
	sem_t			*sem_pick;//changer
	sem_t			*sem_monit;//changer
	sem_t			*sem_fork;//changer
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;
}				t_inf;

typedef struct	s_monit
{
	pthread_t			thread;
	int					lst_eat;
	struct s_monit		*next;
}				t_monit;

typedef struct		s_phi
{
	int				num;
	int				nb_eat;
	int				is_dead;
	//pthread_t		thread;
	struct s_inf	*inf;
	struct s_monit	*monit;
	struct s_phi	*next;
}					t_phi;

/*
**	utils.c
*/
int				ft_atoi(const char *str);
char			*ft_itoa(int n);
void			ft_putstr(char *s);

/*
**	thread.c
*/
int			launch_all(t_inf *inf, t_phi *phi);
void		close_all(t_inf *inf);

/*
**	print.c
*/
void    print(int time, int num, int status);

#endif