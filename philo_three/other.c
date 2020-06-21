/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 15:08:08 by user42            #+#    #+#             */
/*   Updated: 2020/06/21 15:10:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_the_time(void)
{
	struct timeval time;

	if (gettimeofday(&time, NULL))
		return (0);
	return ((1000 * time.tv_sec) + (time.tv_usec / 1000));
}

uint64_t	get_time(uint64_t time_start)
{
	return (get_the_time() - time_start);
}
