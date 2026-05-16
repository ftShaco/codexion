/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:36:11 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:09 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	check_if_dead(t_coder *coder)
{
	int	status;

	pthread_mutex_lock(&coder->s_data->end_mut);
	status = coder->s_data->end_flag || coder->is_done;
	pthread_mutex_unlock(&coder->s_data->end_mut);
	return (status);
}

void	first_phase(t_coder *coder)
{
	pthread_mutex_lock(&coder->timer_mut);
	coder->timer = get_current_time();
	pthread_mutex_unlock(&coder->timer_mut);
	print_action(coder, "is compiling");
	smart_sleep(coder->s_data->time_to_compile, coder);
	release_dongle(coder->left_dongle, coder->s_data->dongle_cooldown);
	release_dongle(coder->right_dongle, coder->s_data->dongle_cooldown);
}

void	display_compilation(t_coder *coder)
{
	if (coder->counter == 1)
	{
		printf("%ld Coder #%d compiled for the %dst time\n",
			get_current_time() - coder->s_data->start_time,
			coder->coder_id, coder->counter);
	}
	else
	{
		printf("%ld Coder #%d compiled for the %dth time\n",
			get_current_time() - coder->s_data->start_time,
			coder->coder_id, coder->counter);
	}
}

void	is_compiling(t_coder *coder)
{
	first_phase(coder);
	coder->counter += 1;
	if (coder->counter == coder->s_data->number_of_compiles_required)
	{
		print_action(coder, "Coder has done the job!");
		pthread_mutex_lock(&coder->s_data->end_mut);
		coder->is_done = 1;
		pthread_mutex_unlock(&coder->s_data->end_mut);
		return ;
	}
	pthread_mutex_lock(&coder->s_data->write_m);
	if (check_if_dead(coder) == 0)
		display_compilation(coder);
	pthread_mutex_unlock(&coder->s_data->write_m);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->s_data->start_mutex);
	pthread_mutex_unlock(&coder->s_data->start_mutex);
	while (check_if_dead(coder) == 0)
	{
		if (coder->coder_id % 2 == 0)
			heap_process(coder, coder->right_dongle);
		heap_process(coder, coder->left_dongle);
		if (coder->coder_id % 2 != 0)
			heap_process(coder, coder->right_dongle);
		is_compiling(coder);
		is_debugging(coder);
		is_refactoring(coder);
	}
	return (NULL);
}
