/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:47:51 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:59 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	is_debugging(t_coder *coder)
{
	print_action(coder, "is debugging");
	smart_sleep(coder->s_data->time_to_debug, coder);
}

void	is_refactoring(t_coder *coder)
{
	print_action(coder, "is refactoring");
	smart_sleep(coder->s_data->time_to_refactor, coder);
}

void	burnout_routine(t_data *s_data, int i)
{
	pthread_mutex_lock(&s_data->end_mut);
	s_data->end_flag = 1;
	pthread_mutex_unlock(&s_data->end_mut);
	pthread_mutex_lock(&s_data->write_m);
	printf("%ld %d burned out\n", get_current_time() - s_data->start_time,
		s_data->coders_tab[i].coder_id);
	pthread_mutex_unlock(&s_data->write_m);
}

int	monitor_helper(t_data *s_data, int i)
{
	size_t	t;
	size_t	timer_copy;

	t = get_current_time();
	pthread_mutex_lock(&s_data->coders_tab[i].timer_mut);
	timer_copy = s_data->coders_tab[i].timer;
	pthread_mutex_unlock(&s_data->coders_tab[i].timer_mut);
	if (t >= timer_copy && (t - timer_copy)
		>= (size_t)s_data->time_to_burnout)
	{
		burnout_routine(s_data, i);
		return (-1);
	}
	return (0);
}
