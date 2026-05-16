/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:48:53 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:56 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	check_numeric_values(char *s, int a)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (1);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
		{
			value_error_message(a, s);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_scheduler(char *s)
{
	if (!s || (strcmp(s, "fifo") != 0 && strcmp(s, "edf") != 0))
	{
		scheduler_error_msg();
		return (1);
	}
	return (0);
}

int	ft_atoi(char *s, long *res)
{
	int	i;

	i = 0;
	*res = 0;
	while (s[i])
	{
		*res = (*res * 10) + (s[i] - '0');
		i++;
		if (*res > 2147483647)
			return (-1);
	}
	return (0);
}

int	validate_and_build(char **av, t_data *s_data)
{
	long	res;

	if (ft_atoi(av[1], &res) == -1 || res <= 0)
		return (fprintf(stderr, "Error: Invalid number of coders\n"), 1);
	s_data->number_of_coders = res;
	if (ft_atoi(av[2], &res) == -1)
		return (fprintf(stderr, "Error: Invalid burnout-time\n"), 1);
	s_data->time_to_burnout = res;
	if (ft_atoi(av[3], &res) == -1)
		return (fprintf(stderr, "Error: Invalid compile-time\n"), 1);
	s_data->time_to_compile = res;
	if (ft_atoi(av[4], &res) == -1)
		return (fprintf(stderr, "Error: Invalid debug-time\n"), 1);
	s_data->time_to_debug = res;
	if (ft_atoi(av[5], &res) == -1)
		return (fprintf(stderr, "Error: Invalid refactor time\n"), 1);
	s_data->time_to_refactor = res;
	if (ft_atoi(av[6], &res) == -1)
		return (fprintf(stderr, "Error: Invalid number of compiles\n"), 1);
	s_data->number_of_compiles_required = res;
	if (ft_atoi(av[7], &res) == -1)
		return (fprintf(stderr, "Error: Invalid dongle cooldown\n"), 1);
	s_data->dongle_cooldown = res;
	s_data->scheduler = av[8];
	return (0);
}

int	parse_args(int ac, char **av, t_data *s_data)
{
	int	i;

	if (ac != 9)
	{
		fprintf(stderr, "You forgot at least one argument.\n");
		arg_error_message();
		return (1);
	}
	i = 1;
	while (i < 8)
	{
		if (check_numeric_values(av[i], i) == 1)
			return (1);
		i++;
	}
	if (check_scheduler(av[i]) == 1)
		return (1);
	if (validate_and_build(av, s_data) == 1)
		return (fprintf(stderr, "Value has to be strictrly positive, "
				"and can't be larger than INT MAX (2147483647)\n"), 1);
	return (0);
}
