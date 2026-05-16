/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:58:46 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:19 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	main(int ac, char **av)
{
	t_data	s_data;
	int		i;

	if (parse_args(ac, av, &s_data) == 1)
		return (fprintf(stderr, "Exiting program..."), 1);
	if (init_simulation(&s_data) == 1)
	{
		free_data(&s_data, 0);
		return (fprintf(stderr, "Exiting program..."), 1);
	}
	i = 0;
	while (i < s_data.number_of_coders)
	{
		pthread_join(s_data.coders_tab[i].id, NULL);
		i++;
	}
	pthread_join(s_data.monitor_id, NULL);
	free_data(&s_data, 1);
	printf("\nExiting program...\n");
	return (0);
}
