/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:57:41 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:24 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	arg_error_message(void)
{
	fprintf(stderr, "Do only write the values for the mandatory variables.\n"
		"They have to be in the following order: number_of_coders, "
		"time_to_burnout, time_to_compile, time_to_debug, time_to_refactor, "
		"number_of_compiles_required, dongle_cooldown, scheduler\n"
		"Usage example: ./codexion 5 800 200 200 200 7 10 fifo\n");
}

void	value_error_message(int i, char *s)
{
	fprintf(stderr, "The value #%d: '%s' isn't accepted by the program. "
		"\nPlease do only use positive values, and only numbers.\n"
		"Any other character will force the program to quit.\n", i, s);
}

void	scheduler_error_msg(void)
{
	fprintf(stderr, "Error while reading the scheduler type.\n"
		"Please do only use 'fifo' or 'edf' in lowercase.\n"
		"fifo = First In, First Out: the dongle is granted"
		" to the coder whose request arrived first.\n"
		"edf = Earliest Deadline First, with deadline = "
		"last_compile_start + time_to_burnout\n");
}
