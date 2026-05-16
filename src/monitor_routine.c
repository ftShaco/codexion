/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:19:05 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:48 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	release_dongle(t_dongle *dongle, int cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = 1;
	dongle->available_at = get_current_time() + cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void	work_done_camarad(t_data *s_data)
{
	pthread_mutex_lock(&s_data->write_m);
	printf("\nAll coders have done their work !\n");
	pthread_mutex_unlock(&s_data->write_m);
	pthread_mutex_lock(&s_data->end_mut);
	s_data->end_flag = 1;
	pthread_mutex_unlock(&s_data->end_mut);
}

int	is_done_helper(t_data *s_data, int i)
{
	int	is_done_copy;

	pthread_mutex_lock(&s_data->end_mut);
	is_done_copy = s_data->coders_tab[i].is_done;
	pthread_mutex_unlock(&s_data->end_mut);
	return (is_done_copy);
}

void	monitoring(int i, int finished_coders, t_data *s_data)
{
	while (1)
	{
		i = 0;
		finished_coders = 0;
		while (i < s_data->number_of_coders)
		{
			if (is_done_helper(s_data, i) == 1)
				finished_coders ++;
			else
			{
				if (monitor_helper(s_data, i) == -1)
					return ;
			}
			i++;
		}
		if (finished_coders == s_data->number_of_coders)
		{
			work_done_camarad(s_data);
			return ;
		}
		usleep(1000);
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*s_data;
	int		i;
	int		finished_coders;

	i = 0;
	finished_coders = 0;
	s_data = (t_data *)arg;
	pthread_mutex_lock(&s_data->start_mutex);
	pthread_mutex_unlock(&s_data->start_mutex);
	monitoring(i, finished_coders, s_data);
	return (NULL);
}
