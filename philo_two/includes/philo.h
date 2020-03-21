/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/03/21 19:34:32 by froussel         ###   ########.fr       */
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

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK_1 4
# define FORK_2 5
# define DEAD 6

# include <stdio.h>		//printf
# include <errno.h>		//strerror(errno)
#include <signal.h>		//ctr-c
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
	sem_t			*sem_pick;//changer
	sem_t			*sem_monit;//changer
	//struct s_fork	*fork_1;
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;

	int				nb_fork;
	sem_t			*sem_fork;//changer
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

/*typedef struct	s_fork
{
	int				nb_fork;
	sem_t			*sem;//changer
	struct s_fork	*next;
}				t_fork;*/

/*
**	utils.c
*/
int				ft_atoi(const char *str);
char			*ft_itoa(int n);
void			ft_putstr(char *s);

/*
**	build.c
*/
int			launch_all(t_inf *inf, t_phi *phi, t_monit *monit);

/*
**	print.c
*/
void    print(int time, int num, int status);

#endif