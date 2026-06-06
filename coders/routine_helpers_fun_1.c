/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers_fun_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:51 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:55:07 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	debugging_hh(t_dongle *dongle)
{
	int		i;
	t_coder	*coders_queue;

	i = 0;
	coders_queue = (t_coder *)dongle->m_heap->queue;
	while (i < dongle->m_heap->size)
		printf("%d ", coders_queue[i++].coder_id);
}

void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(dongle->mutex_dongle);
	push(dongle->m_heap, coder);
	pthread_mutex_unlock(dongle->mutex_dongle);
}

void	leave_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(dongle->mutex_dongle);
	pop(dongle->m_heap);
	pthread_mutex_unlock(dongle->mutex_dongle);
}

void	logs(pthread_mutex_t *display_mutex, char *message, long current_time,
		int coder_id)
{
	pthread_mutex_lock(display_mutex);
	printf("%ld %d %s\n", current_time, coder_id, message);
	pthread_mutex_unlock(display_mutex);
}

void	set_deadline(char *scheduler, t_coder *coder, struct timeval start,
		TIME t_to_burnout)
{
	if (!strcmp("fifo", scheduler))
		coder->deadline = get_timestamp_ms(start);
	else
		coder->deadline = coder->last_compile_start + t_to_burnout;
}
