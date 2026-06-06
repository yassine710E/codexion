/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_initialization_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:15 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 22:19:41 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_min_heap	*init_min_heap(void)
{
	t_min_heap	*heap;

	heap = malloc(sizeof(t_min_heap));
	if (!heap)
		return (NULL);
	heap->queue = malloc(sizeof(t_coder) * 2);
	if (!heap->queue)
		return (NULL);
	heap->size = 0;
	return (heap);
}

void	set_dongle(t_dongle *dongle, unsigned int dongle_id, t_min_heap *m_heap,
		pthread_mutex_t *mutex)
{
	if (!dongle || !m_heap || !mutex)
		return ;
	dongle->dongle_id = dongle_id;
	dongle->m_heap = m_heap;
	dongle->mutex_dongle = mutex;
	dongle->last_compile_start_saver = 0;
}

static int	init_first_coder(t_coder *arr, pthread_mutex_t *mutexes,
		t_dongle **tmp_left, t_dongle **tmp_right)
{
	t_dongle	*left;
	t_dongle	*right;

	left = malloc(sizeof(t_dongle));
	right = malloc(sizeof(t_dongle));
	if (!left || !right)
	{
		free(left);
		free(right);
		return (0);
	}
	set_dongle(left, 1, init_min_heap(), mutexes);
	set_dongle(right, 2, init_min_heap(), mutexes + 1);
	arr[0].coder_id = 1;
	arr[0].left = left;
	arr[0].right = right;
	*tmp_left = right;
	*tmp_right = left;
	return (1);
}

static int	init_middle_coder(t_coder *arr, unsigned int i,
		pthread_mutex_t *mutexes, t_dongle **tmp_left)
{
	t_dongle	*right;

	right = malloc(sizeof(t_dongle));
	if (!right)
		return (0);
	set_dongle(right, i + 2, init_min_heap(), mutexes + i + 1);
	arr[i].coder_id = i + 1;
	arr[i].left = *tmp_left;
	arr[i].right = right;
	*tmp_left = right;
	return (1);
}

void	set_coders(t_coder *arr_coders, unsigned int size_coders,
		pthread_mutex_t *ptr_mutex)
{
	unsigned int	i;
	t_dongle		*tmp_left;
	t_dongle		*tmp_right;

	if (size_coders == 0)
		return ;
	if (!init_first_coder(arr_coders, ptr_mutex, &tmp_left, &tmp_right))
		return ;
	i = 0;
	while (++i < size_coders)
	{
		if (i == size_coders - 1)
		{
			arr_coders[i].coder_id = i + 1;
			arr_coders[i].left = tmp_left;
			arr_coders[i].right = tmp_right;
		}
		else
		{
			if (!init_middle_coder(arr_coders, i, ptr_mutex, &tmp_left))
				return ;
		}
	}
}
