/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:49:54 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:08:04 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		fprintf(stderr, "Error: gettimeofday failed\n");
		return (0);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_action(t_coder *coder, char *action)
{
	pthread_mutex_lock(&coder->s_data->write_m);
	if (check_if_dead(coder) == 0)
	{
		printf("%ld %d %s\n", get_current_time() - coder->s_data->start_time,
			coder->coder_id, action);
	}
	pthread_mutex_unlock(&coder->s_data->write_m);
}

void	smart_sleep(size_t time_in_ms, t_coder *coder)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < time_in_ms)
	{
		if (check_if_dead(coder) == 1)
			break ;
		usleep(500);
	}
}

int	is_higher_priority(t_coder *a, t_coder *b)
{
	if (a->s_data->is_edf == 0)
		return (a->heap_arrival <= b->heap_arrival);
	else
		return (a->timer <= b->timer);
}
