/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:15:33 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:32 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	init_coders(t_data *s_data)
{
	int	i;

	i = 0;
	while (i < s_data->number_of_coders)
	{
		s_data->coders_tab[i].left_dongle = &s_data->dongles_tab[i];
		s_data->coders_tab[i].right_dongle
			= &s_data->dongles_tab[(i + 1) % s_data->number_of_coders];
		s_data->coders_tab[i].coder_id = i + 1;
		s_data->coders_tab[i].s_data = s_data;
		s_data->coders_tab[i].counter = 0;
		s_data->coders_tab[i].timer = 0;
		s_data->coders_tab[i].is_done = 0;
		pthread_mutex_init(&s_data->coders_tab[i].timer_mut, NULL);
		i++;
	}
	s_data->end_flag = 0;
}

void	init_dongles(t_data *s_data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&s_data->start_mutex, NULL);
	while (i < s_data->number_of_coders)
	{
		pthread_mutex_init(&s_data->dongles_tab[i].mutex, NULL);
		s_data->dongles_tab[i].is_available = 1;
		s_data->dongles_tab[i].available_at = 0;
		s_data->dongles_tab[i].c = 'a' + i;
		s_data->dongles_tab[i].queue.size = 0;
		s_data->dongles_tab[i].queue.capacity = s_data->number_of_coders;
		s_data->dongles_tab[i].queue.array
			= malloc(sizeof(t_coder *) * s_data->number_of_coders);
		if (!s_data->dongles_tab[i].queue.array)
		{
			fprintf(stderr, "Memory allocation failed, "
				"cleaning and exiting program.");
			free_data(s_data, 1);
		}
		i++;
	}
	pthread_mutex_init(&s_data->end_mut, NULL);
	pthread_mutex_init(&s_data->write_m, NULL);
}

void	create_threads(t_data *s_data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&s_data->start_mutex);
	while (i < s_data->number_of_coders)
	{
		pthread_create(&s_data->coders_tab[i].id, NULL,
			coder_routine, &s_data->coders_tab[i]);
		i++;
	}
	s_data->start_time = get_current_time();
	i = 0;
	while (i < s_data->number_of_coders)
	{
		s_data->coders_tab[i].timer = s_data->start_time;
		i++;
	}
	pthread_create(&s_data->monitor_id, NULL, monitor_routine, s_data);
	pthread_mutex_unlock(&s_data->start_mutex);
}

int	init_simulation(t_data *s_data)
{
	s_data->coders_tab = malloc(sizeof(t_coder) * s_data->number_of_coders);
	s_data->dongles_tab = malloc(sizeof(t_dongle) * s_data->number_of_coders);
	if (!s_data->coders_tab || !s_data->dongles_tab)
		return (fprintf(stderr, "Memory allocation failed.\n"), 1);
	s_data->is_edf = strcmp(s_data->scheduler, "edf");
	init_dongles(s_data);
	init_coders(s_data);
	create_threads(s_data);
	return (0);
}
