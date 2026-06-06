/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:44 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:36:08 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sleep_for_operation(TIME t_operation, t_shared_data *s_data,
		long last_operation_start, char *msg)
{
	long	end;
	int		f;

	end = get_timestamp_ms(s_data->start) + t_operation;
	while (get_timestamp_ms(s_data->start) < end)
	{
		pthread_mutex_lock(s_data->main_mutex);
		f = *s_data->flag_burnout;
		pthread_mutex_unlock(s_data->main_mutex);
		if (f)
		{
			broadcast_other_coders(s_data);
			return (0);
		}
		usleep(500);
	}
	logs(s_data->mutex_display, msg, last_operation_start,
		s_data->coder->coder_id);
	return (1);
}

long	get_timestamp_ms(struct timeval start)
{
	struct timeval	now;
	long			seconds;
	long			microseconds;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - start.tv_sec;
	microseconds = now.tv_usec - start.tv_usec;
	return ((seconds * 1000) + (microseconds / 1000));
}
