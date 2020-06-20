/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 15:03:06 by froussel          #+#    #+#             */
/*   Updated: 2020/06/20 23:04:38 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		free_all(t_inf *inf, int ret)
{
	t_phi	*nxt_phi;
	t_monit	*nxt_monit;

	sem_close(inf->sem_pick);
	sem_unlink("/pickup");
	sem_close(inf->sem_monit);
	sem_unlink("/sem_monit");
	sem_close(inf->sem_fork);
	sem_unlink("/fork");
	while (inf->phi_1)
	{
		nxt_phi = inf->phi_1->next;
		free(inf->phi_1);
		inf->phi_1 = nxt_phi;
	}
	while (inf->monit_1)
	{
		nxt_monit = inf->monit_1->next;
		free(inf->monit_1);
		inf->monit_1 = nxt_monit;
	}
	free(inf->pid_tab);
	return (ret);
}

void	close_all(t_inf *inf)
{
	int		i;
	int		philo;
	int		status;

	philo = inf->nb_phi;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WEXITSTATUS(status) == TIME_DEATH)
		{
			i = -1;
			while (++i < inf->nb_phi)
				kill(inf->pid_tab[i], SIGINT);
			return ;
		}
		else if (WEXITSTATUS(status) == NB_EAT_DEATH)
		{
			sem_post(inf->sem_monit);
			if (--philo == 0)
				return ;
		}
	}
}
