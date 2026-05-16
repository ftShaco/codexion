/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jreibel <jreibel@student.42.mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:00:42 by jreibel           #+#    #+#             */
/*   Updated: 2026/05/15 19:07:30 by jreibel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	push_heap(t_heap *heap, t_coder *new_coder)
{
	int		i;
	int		parent;
	t_coder	*tmp;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->array[i] = new_coder;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (is_higher_priority(heap->array[i], heap->array[parent]))
		{
			tmp = heap->array[parent];
			heap->array[parent] = heap->array[i];
			heap->array[i] = tmp;
			i = parent;
		}
		else
			break ;
	}
	heap->size++;
}

int	sort_and_prior(t_heap *heap, int i, int prior_child)
{
	t_coder	*tmp;

	tmp = heap->array[i];
	heap->array[i] = heap->array[prior_child];
	heap->array[prior_child] = tmp;
	i = prior_child;
	return (i);
}

int	pop_heap_helper(int l_child, int r_child, int prior_child, t_heap *heap)
{
	if (l_child < heap->size && is_higher_priority(heap->array[l_child],
			heap->array[prior_child]))
		prior_child = l_child;
	if (r_child < heap->size && is_higher_priority(heap->array[r_child],
			heap->array[prior_child]))
		prior_child = r_child;
	return (prior_child);
}

void	pop_heap(t_heap *heap)
{
	int		i;
	int		l_child;
	int		r_child;
	int		prior_child;

	if (heap->size <= 0)
		return ;
	heap->array[0] = heap->array[heap->size - 1];
	heap->size--;
	i = 0;
	while (1)
	{
		l_child = (2 * i) + 1;
		r_child = (2 * i) + 2;
		prior_child = i;
		prior_child = pop_heap_helper(l_child, r_child, prior_child, heap);
		if (prior_child == i)
			break ;
		i = sort_and_prior(heap, i, prior_child);
	}
}

void	heap_process(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	coder->heap_arrival = get_current_time();
	push_heap(&dongle->queue, coder);
	pthread_mutex_unlock(&dongle->mutex);
	while (check_if_dead(coder) == 0)
	{
		pthread_mutex_lock(&dongle->mutex);
		if (dongle->queue.array[0] == coder
			&& dongle->is_available == 1
			&& get_current_time() >= dongle->available_at)
		{
			dongle->is_available = 0;
			pop_heap(&dongle->queue);
			pthread_mutex_unlock(&dongle->mutex);
			print_action(coder, "has taken a dongle");
			break ;
		}
		pthread_mutex_unlock(&dongle->mutex);
		usleep(100);
	}
}
