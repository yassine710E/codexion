/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_and_cond_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:41 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:32:30 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_arr_mutex(pthread_mutex_t *ptr_mutex, unsigned int dongles_size)
{
	unsigned int	i;

	i = 0;
	while (i < dongles_size)
		pthread_mutex_init(ptr_mutex + i++, NULL);
}

void	destroy_arr_mutex(pthread_mutex_t *ptr_mutex, unsigned int dongles_size)
{
	unsigned int	i;

	i = 0;
	while (i < dongles_size)
		pthread_mutex_destroy(ptr_mutex + i++);
}

void	init_arr_cond(pthread_cond_t *ptr_cond, unsigned int dongles_size)
{
	unsigned int	i;

	i = 0;
	while (i < dongles_size)
		pthread_cond_init(ptr_cond + i++, NULL);
}

void	destroy_arr_cond(pthread_cond_t *ptr_cond, unsigned int dongles_size)
{
	unsigned int	i;

	i = 0;
	while (i < dongles_size)
		pthread_cond_destroy(ptr_cond + i++);
}
