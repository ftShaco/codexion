/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:18:40 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:37 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	free_data(t_data *s_data, int mode)
{
	int	i;

	if (mode == 1)
	{
		i = 0;
		pthread_mutex_destroy(&s_data->start_mutex);
		pthread_mutex_destroy(&s_data->end_mut);
		pthread_mutex_destroy(&s_data->write_m);
		while (i < s_data->number_of_coders)
		{
			pthread_mutex_destroy(&s_data->dongles_tab[i].mutex);
			if (s_data->dongles_tab[i].queue.array)
				free(s_data->dongles_tab[i].queue.array);
			i++;
		}
	}
	if (s_data->dongles_tab)
		free(s_data->dongles_tab);
	if (s_data->coders_tab)
		free(s_data->coders_tab);
}
