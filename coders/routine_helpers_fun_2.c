/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers_fun_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 23:54:21 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:55:18 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_coder_exist_in_queue(t_min_heap *m_heap, int coder_id,
		pthread_mutex_t *dongle_mutex)
{
	int		i;
	t_coder	*arr_coders;
	int		condition;

	i = 0;
	arr_coders = (t_coder *)m_heap->queue;
	while (1)
	{
		pthread_mutex_lock(dongle_mutex);
		condition = i < m_heap->size;
		pthread_mutex_unlock(dongle_mutex);
		if (!condition)
			break ;
		if (coder_id == arr_coders[i].coder_id)
			return (1);
		i++;
	}
	return (0);
}

void	broadcast_other_coders(t_shared_data *s_data)
{
	pthread_mutex_lock(s_data->main_mutex);
	pthread_cond_broadcast(s_data->main_cond);
	pthread_mutex_unlock(s_data->main_mutex);
}
