/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/19 19:39:27 by froussel          #+#    #+#             */
/*   Updated: 2020/06/20 23:06:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
**  EAT     1
**  SLEEP   2
**  THINK   3
**  FORK_1  4
**  FORK_2  5
**  DEAD    6
*/

void	print(int time, int num, int status)
{
	char	*str_time;
	char	*str_num;

	if (status < 1 && status > 6)
		return ;
	if (!(str_time = ft_itoa(time)))
		return ;
	if (!(str_num = ft_itoa(num)))
		return ;
	ft_putstr(str_time);
	ft_putstr(" ");
	ft_putstr(str_num);
	if (status == EAT)
		ft_putstr(" is eating\n");
	else if (status == SLEEP)
		ft_putstr(" is sleeping\n");
	else if (status == THINK)
		ft_putstr(" is thinking\n");
	else if (status == FORK_1 || status == FORK_2)
		ft_putstr(" has taken a fork\n");
	else if (status == DEAD)
		ft_putstr(" died\n");
	free(str_time);
	free(str_num);
}
