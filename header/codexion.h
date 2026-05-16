/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:49:31 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 18:45:15 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <threads.h>
# include <pthread.h>
# include <sys/time.h>
# include <stddef.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_heap	t_heap;
typedef struct s_dongle	t_dongle;

struct s_data
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	int				is_edf;

	t_coder			*coders_tab;
	t_dongle		*dongles_tab;

	size_t			start_time;
	int				end_flag;

	pthread_mutex_t	start_mutex;
	pthread_mutex_t	write_m;
	pthread_mutex_t	end_mut;

	pthread_t		monitor_id;

};

struct s_coder
{
	int				coder_id;
	size_t			timer;
	size_t			heap_arrival;
	int				counter;
	pthread_t		id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*s_data;
	int				is_done;
	pthread_mutex_t	timer_mut;
};

struct s_heap
{
	t_coder	**array;
	int		size;
	int		capacity;
};

struct s_dongle
{
	pthread_mutex_t	mutex;
	int				is_available;
	size_t			available_at;
	t_heap			queue;
	char			c;
};

// PARSER //
int		parse_args(int ac, char **av, t_data *s_data);

// SIMU //
int		init_simulation(t_data *s_data);

// ERROR.C //
void	arg_error_message(void);
void	value_error_message(int i, char *s);
void	scheduler_error_msg(void);

// UTILS.C //
int		ft_isdigit(char c);
size_t	get_current_time(void);
void	print_action(t_coder *coder, char *action);
void	smart_sleep(size_t time_in_ms, t_coder *coder);
int		is_higher_priority(t_coder *a, t_coder *b);

// MEMORY.C //
void	free_data(t_data *s_data, int mode);

// ROUTINES.C //
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
int		check_if_dead(t_coder *coder);
void	release_dongle(t_dongle *dongle, int cooldown);

// ROUTINES HELPERS //
void	is_debugging(t_coder *coder);
void	is_refactoring(t_coder *coder);
void	burnout_routine(t_data *s_data, int i);
int		monitor_helper(t_data *s_data, int i);

// HEAP.C //
void	heap_process(t_coder *coder, t_dongle *dongle);

#endif
