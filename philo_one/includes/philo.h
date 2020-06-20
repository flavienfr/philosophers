/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 11:51:37 by froussel          #+#    #+#             */
/*   Updated: 2020/06/21 00:19:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK_1 4
# define FORK_2 5
# define DEAD 6

typedef struct		s_inf
{
	int				nb_phi;
	int				ms_die;
	int				ms_eat;
	int				ms_slp;
	int				nb_eat;
	int				time_eat;
	int				end;
	uint64_t		time_start;
	pthread_mutex_t	mtx;
	pthread_mutex_t	mtx_monit;
	struct s_fork	*fork_1;
	struct s_phi	*phi_1;
	struct s_monit	*monit_1;
}					t_inf;

typedef struct		s_monit
{
	pthread_t		thread;
	int				lst_status;
	uint64_t		lst_eat;
	struct s_monit	*next;
}					t_monit;

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

typedef struct		s_fork
{
	int				is_fork;
	pthread_mutex_t	mtx;
	struct s_fork	*next;
}					t_fork;

/*
**	utils.c
*/
int					ft_atoi(const char *str);
char				*ft_itoa(int n);
void				ft_putstr(char *s);

/*
**	other.c
*/
t_monit				*new_monit(void);
void				ft_usleep(uint64_t us);
uint64_t			get_the_time(void);
uint64_t			get_time(uint64_t time_start);
int					free_all(t_inf *inf, int ret);
t_fork				*select_fork(t_fork *fork, int num);

/*
**	philo.c
*/
int					launch_all(t_inf *inf, t_phi *phi);

/*
**	print.c
*/
void				print(int time, int num, int status);

#endif
