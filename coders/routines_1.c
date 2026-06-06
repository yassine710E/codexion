/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:53 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 01:30:19 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_shared_data *s_data)
{
	t_coder	*coder;
	t_coder	*left;
	t_coder	*right;

	coder = s_data->coder;
	left = (t_coder *)coder->left->m_heap->queue;
	right = (t_coder *)coder->right->m_heap->queue;
	if (!(coder->left->m_heap->size && coder->coder_id == left[0].coder_id
			&& coder->right->m_heap->size
			&& coder->coder_id == right[0].coder_id))
	{
		if (coder->left->m_heap->size && coder->coder_id == left[0].coder_id)
			leave_dongle(coder->left);
		else if (coder->right->m_heap->size
			&& coder->coder_id == right[0].coder_id)
			leave_dongle(coder->right);
		set_deadline(s_data->args->scheduler, coder, s_data->start,
			s_data->args->time_to_burnout);
	}
}

static void	wait_for_dongle_cooldown(t_shared_data *s_data, t_dongle *left,
		t_dongle *right)
{
	long	lcs;

	if (left->last_compile_start_saver > right->last_compile_start_saver)
		lcs = left->last_compile_start_saver;
	else
		lcs = right->last_compile_start_saver;
	while (1)
	{
		if (get_timestamp_ms(s_data->start) >= lcs
			+ s_data->args->time_to_compile + s_data->args->dongle_cooldown)
			break ;
	}
}

static void	check_and_request_dongles(t_shared_data *s_data)
{
	t_coder	*coder;

	coder = s_data->coder;
	if (!is_coder_exist_in_queue(coder->left->m_heap, coder->coder_id,
			coder->left->mutex_dongle))
		request_dongle(coder, coder->left);
	if (!is_coder_exist_in_queue(coder->right->m_heap, coder->coder_id,
			coder->right->mutex_dongle))
		request_dongle(coder, coder->right);
	wait_for_dongle_cooldown(s_data, coder->left, coder->right);
}

static int	get_ids(t_shared_data *s_data, int *left_id, int *right_id)
{
	t_coder	*coder;
	t_coder	*left;
	t_coder	*right;

	coder = s_data->coder;
	left = (t_coder *)coder->left->m_heap->queue;
	right = (t_coder *)coder->right->m_heap->queue;
	if (s_data->args->number_of_coders > 1)
	{
		pthread_mutex_lock(coder->left->mutex_dongle);
		*left_id = left[0].coder_id;
		pthread_mutex_unlock(coder->left->mutex_dongle);
		pthread_mutex_lock(coder->right->mutex_dongle);
		*right_id = right[0].coder_id;
		pthread_mutex_unlock(coder->right->mutex_dongle);
	}
	else
	{
		*left_id = coder->coder_id + 1;
		*right_id = *left_id;
	}
	return (coder->coder_id == *left_id && coder->coder_id == *right_id);
}

int	wait_for_waiter_coders(t_shared_data *s_data)
{
	t_coder	*coder;
	int		left_id;
	int		right_id;

	coder = s_data->coder;
	pthread_mutex_lock(s_data->main_mutex);
	while (1)
	{
		if (get_ids(s_data, &left_id, &right_id))
			break ;
		coder->is_waiting = 1;
		pthread_cond_wait(s_data->main_cond, s_data->main_mutex);
		if (*s_data->flag_burnout)
		{
			pthread_mutex_unlock(s_data->main_mutex);
			return (0);
		}
		check_and_request_dongles(s_data);
	}
	coder->is_waiting = 0;
	pthread_mutex_unlock(s_data->main_mutex);
	return (1);
}
