/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 21:56:31 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 02:12:01 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_coders_and_dongles_and_m_heap(t_args args, t_coder *arr_coders)
{
	int	i;

	i = 0;
	while (i < args.number_of_coders)
	{
		free(arr_coders[i].left->m_heap->queue);
		arr_coders[i].left->m_heap->queue = NULL;
		free(arr_coders[i].left->m_heap);
		arr_coders[i].left->m_heap = NULL;
		free(arr_coders[i].left);
		arr_coders[i].left = NULL;
		if (args.number_of_coders == 1)
		{
			free(arr_coders[i].right->m_heap->queue);
			arr_coders[i].right->m_heap->queue = NULL;
			free(arr_coders[i].right->m_heap);
			arr_coders[i].right->m_heap = NULL;
			free(arr_coders[i].right);
			arr_coders[i].right = NULL;
		}
		i++;
	}
	free(arr_coders);
}

void	free_shared_data__mutexes_threads(t_shared_data *s_data,
		pthread_mutex_t *mutex_arr, pthread_t *arr_threads)
{
	free(s_data);
	free(mutex_arr);
	free(arr_threads);
}

static void	setup_data(t_resources *r, t_sync *s, t_args *args, int *flags)
{
	struct timeval	start;

	memset(r->arr_coders, 0, sizeof(t_coder) * args->number_of_coders);
	memset(r->s_data, 0, sizeof(t_shared_data) * args->number_of_coders);
	init_arr_mutex(r->mutex_arr, args->number_of_coders);
	pthread_mutex_init(&s->main_mutex, NULL);
	pthread_mutex_init(&s->display_mutex, NULL);
	pthread_cond_init(&s->main_cond, NULL);
	pthread_cond_init(&s->display_cond, NULL);
	set_coders(r->arr_coders, args->number_of_coders, r->mutex_arr);
	gettimeofday(&start, NULL);
	set_shared_data(r->s_data, r->arr_coders, args, &flags[0]);
	set_mutexes(r->s_data, &s->display_mutex, &s->main_mutex, args);
	set_conditions(r->s_data, &s->display_cond, &s->main_cond, args);
	set_start_time(r->s_data, start, args);
	set_pointer_to_counter_coders(r->s_data, &flags[1], args);
}

static void	launch_and_join(t_resources *r, t_args *args)
{
	int	i;

	i = 0;
	if (pthread_create(&r->thread_monitor, NULL, routine_monitor, r->s_data))
		return ;
	while (i < args->number_of_coders)
	{
		if (pthread_create(r->arr_threads + i, NULL, coder_routine, r->s_data
				+ i))
			return ;
		i++;
	}
	i = 0;
	while (i < args->number_of_coders)
	{
		pthread_join(r->arr_threads[i], NULL);
		i++;
	}
	pthread_join(r->thread_monitor, NULL);
}

int	main(int c, char **v)
{
	t_args		args;
	t_sync		sync;
	t_resources	res;
	int			flags[2];

	flags[0] = 0;
	flags[1] = 0;
	if (!parsing(c, v, &args))
		return (1);
	if (!alloc_resources(&res, &args))
		return (1);
	setup_data(&res, &sync, &args, flags);
	launch_and_join(&res, &args);
	cleanup(&res, &sync, &args);
	return (0);
}
