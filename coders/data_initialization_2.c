/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_initialization_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 22:16:54 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 01:50:56 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_shared_data(t_shared_data *arr_s_data, t_coder *arr_coders,
		t_args *args, int *flag)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		arr_s_data[i].args = args;
		arr_s_data[i].coder = arr_coders + i;
		arr_s_data[i].flag_burnout = flag;
		i++;
	}
}

void	set_mutexes(t_shared_data *arr_s_data, pthread_mutex_t *d_mutex,
		pthread_mutex_t *main_mutex, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		arr_s_data[i].mutex_display = d_mutex;
		arr_s_data[i].main_mutex = main_mutex;
		i++;
	}
}

void	set_conditions(t_shared_data *arr_s_data, pthread_cond_t *d_cond,
		pthread_cond_t *main_cond, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		arr_s_data[i].cond_display = d_cond;
		arr_s_data[i].main_cond = main_cond;
		i++;
	}
}

void	set_start_time(t_shared_data *arr_s_data, struct timeval start,
		t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		arr_s_data[i].start = start;
		i++;
	}
}

void	set_pointer_to_counter_coders(t_shared_data *arr_s_data,
		int *coders_counter, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		arr_s_data[i].how_many_coders_finished = coders_counter;
		i++;
	}
}
