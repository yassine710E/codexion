/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:37 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/21 23:29:38 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	push(t_min_heap *m_heap, t_coder *coder)
{
	int		i;
	t_coder	*arr_coder;

	i = 0;
	arr_coder = (t_coder *)(m_heap->queue);
	arr_coder[m_heap->size] = *coder;
	m_heap->size += 1;
	while (i < m_heap->size && m_heap->size >= 2)
	{
		if (2 * i + 1 < m_heap->size && arr_coder[i].deadline > arr_coder[2 * i
				+ 1].deadline)
			swap(arr_coder + i, arr_coder + (2 * i + 1));
		if (2 * i + 2 < m_heap->size && arr_coder[i].deadline > arr_coder[2 * i
				+ 2].deadline)
			swap(arr_coder + i, arr_coder + (2 * i + 2));
		i++;
	}
}

void	pop(t_min_heap *m_heap)
{
	int		i;
	t_coder	*queue;

	if (m_heap->size >= 1)
	{
		i = 0;
		queue = (t_coder *)m_heap->queue;
		while (++i < m_heap->size)
			queue[i - 1] = queue[i];
		m_heap->size--;
	}
}

void	swap(t_coder *item1, t_coder *item2)
{
	t_coder	tmp;

	if (!item1 || !item2)
		return ;
	tmp = *item1;
	*item1 = *item2;
	*item2 = tmp;
}
