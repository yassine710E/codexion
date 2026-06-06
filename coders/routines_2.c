/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 00:03:30 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 02:19:01 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	save_compile_start(t_shared_data *s_data)
{
	t_coder	*coder;

	coder = s_data->coder;
	pthread_mutex_lock(coder->left->mutex_dongle);
	coder->left->last_compile_start_saver = coder->last_compile_start;
	pthread_mutex_unlock(coder->left->mutex_dongle);
	pthread_mutex_lock(coder->right->mutex_dongle);
	coder->right->last_compile_start_saver = coder->last_compile_start;
	pthread_mutex_unlock(coder->right->mutex_dongle);
}

static int	do_compile(t_shared_data *s_data)
{
	t_coder	*coder;
	int		coder_id;

	coder = s_data->coder;
	coder_id = coder->coder_id;
	logs(s_data->mutex_display, "has taken a dongle",
		get_timestamp_ms(s_data->start), coder_id);
	logs(s_data->mutex_display, "has taken a dongle",
		get_timestamp_ms(s_data->start), coder_id);
	pthread_mutex_lock(s_data->main_mutex);
	coder->last_compile_start = get_timestamp_ms(s_data->start);
	pthread_mutex_unlock(s_data->main_mutex);
	if (!sleep_for_operation(s_data->args->time_to_compile, s_data,
			coder->last_compile_start, "is compiling"))
		return (0);
	leave_dongle(coder->left);
	leave_dongle(coder->right);
	pthread_mutex_lock(s_data->main_mutex);
	coder->count_compiled++;
	pthread_mutex_unlock(s_data->main_mutex);
	save_compile_start(s_data);
	broadcast_other_coders(s_data);
	return (1);
}

static int	do_rest(t_shared_data *s_data)
{
	if (!sleep_for_operation(s_data->args->time_to_debug, s_data,
			get_timestamp_ms(s_data->start), "is debugging"))
		return (0);
	if (!sleep_for_operation(s_data->args->time_to_refactor, s_data,
			get_timestamp_ms(s_data->start), "is refactoring"))
		return (0);
	return (1);
}

static int	coder_loop_condition(t_shared_data *s_data)
{
	int	condition;
	int	c_id;
	int	flag;
	int	n;

	pthread_mutex_lock(s_data->main_mutex);
	c_id = s_data->coder->count_compiled;
	flag = *s_data->flag_burnout;
	n = s_data->args->number_of_compiles_required;
	condition = (c_id < n && !flag);
	pthread_mutex_unlock(s_data->main_mutex);
	return (condition);
}

void	coder_loop(t_shared_data *s_data, t_coder *coder)
{
	while (coder_loop_condition(s_data))
	{
		set_deadline(s_data->args->scheduler, coder, s_data->start,
			s_data->args->time_to_burnout);
		if (s_data->args->number_of_coders > 1)
		{
			request_dongle(coder, coder->left);
			request_dongle(coder, coder->right);
		}
		release_dongle(s_data);
		if (!wait_for_waiter_coders(s_data))
			break ;
		if (!do_compile(s_data))
			break ;
		if (!do_rest(s_data))
			break ;
	}
}
