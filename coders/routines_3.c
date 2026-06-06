/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 00:12:04 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 01:33:35 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_shared_data *s_data, t_coder *coders, int i)
{
	long	now;
	int		burned;
	int		c_1;
	int		c_2;

	pthread_mutex_lock(s_data->main_mutex);
	now = get_timestamp_ms(s_data->start);
	c_1 = now - coders[i].last_compile_start >= s_data->args->time_to_burnout;
	c_2 = coders[i].count_compiled < s_data->args->number_of_compiles_required;
	burned = (c_1 && c_2);
	if (burned)
		*s_data->flag_burnout = 1;
	pthread_mutex_unlock(s_data->main_mutex);
	if (burned)
	{
		usleep(2000);
		broadcast_other_coders(s_data);
		pthread_mutex_lock(s_data->mutex_display);
		fprintf(stderr, "%ld %d burnout\n", now, coders[i].coder_id);
		pthread_mutex_unlock(s_data->mutex_display);
		return (1);
	}
	return (0);
}

static int	monitor_loop_condition(t_shared_data *s_data)
{
	int	condition;
	int	c_1;
	int	c_2;

	pthread_mutex_lock(s_data->main_mutex);
	c_1 = *s_data->how_many_coders_finished < s_data->args->number_of_coders;
	c_2 = !*s_data->flag_burnout;
	condition = (c_1 && c_2);
	pthread_mutex_unlock(s_data->main_mutex);
	return (condition);
}

void	*routine_monitor(void *data)
{
	t_shared_data	*s_data;
	t_coder			*arr_coders;
	int				i;

	if (!data)
		return (NULL);
	s_data = (t_shared_data *)data;
	arr_coders = s_data->coder;
	while (monitor_loop_condition(s_data))
	{
		i = -1;
		while (++i < s_data->args->number_of_coders)
		{
			if (check_burnout(s_data, arr_coders, i))
				return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

void	*coder_routine(void *data)
{
	t_shared_data	*s_data;
	t_coder			*coder;

	if (!data)
		return (NULL);
	s_data = (t_shared_data *)data;
	coder = s_data->coder;
	coder_loop(s_data, coder);
	pthread_mutex_lock(s_data->main_mutex);
	*s_data->how_many_coders_finished += 1;
	pthread_mutex_unlock(s_data->main_mutex);
	return (NULL);
}
