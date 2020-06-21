/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 14:54:22 by user42            #+#    #+#             */
/*   Updated: 2020/06/21 15:01:38 by user42           ###   ########.fr       */
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
