/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 23:22:57 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:25:29 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	alloc_resources(t_resources *r, t_args *args)
{
	r->arr_coders = malloc(sizeof(t_coder) * args->number_of_coders);
	r->s_data = malloc(sizeof(t_shared_data) * args->number_of_coders);
	r->mutex_arr = malloc(sizeof(pthread_mutex_t) * args->number_of_coders);
	r->arr_threads = malloc(sizeof(pthread_t) * args->number_of_coders);
	if (!r->arr_coders || !r->s_data || !r->mutex_arr || !r->arr_threads)
		return (0);
	return (1);
}

void	cleanup(t_resources *r, t_sync *s, t_args *args)
{
	pthread_cond_destroy(&s->display_cond);
	pthread_cond_destroy(&s->main_cond);
	pthread_mutex_destroy(&s->main_mutex);
	pthread_mutex_destroy(&s->display_mutex);
	destroy_arr_mutex(r->mutex_arr, args->number_of_coders);
	free_coders_and_dongles_and_m_heap(*args, r->arr_coders);
	free_shared_data__mutexes_threads(r->s_data, r->mutex_arr, r->arr_threads);
}
