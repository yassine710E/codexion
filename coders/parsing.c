/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:48 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 01:32:26 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_is_num(char *s)
{
	int	i;

	i = 0;
	if (!*s)
		return (0);
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	error(char *err_msg)
{
	fprintf(stderr, "%s\n", err_msg);
	return (0);
}

static int	validate_args(int c, char **v)
{
	int	i;
	int	is_overflow;
	int	tmp_num;

	i = 0;
	is_overflow = 0;
	if (c != 9)
		return (error("number of arguments must be 8"));
	while (++i < c - 1)
	{
		tmp_num = ft_atoi(v[i], &is_overflow);
		if (!ft_is_num(v[i]))
			return (error("Non-integer input or empty string"));
		if (is_overflow)
			return (error("overflow hhhhhhhh"));
		if (tmp_num < 0)
			return (error("Negative numbers (e.g. -5)"));
		if (tmp_num == 0 && i != 7)
			return (error("invalid argument 0"));
	}
	if (strcmp(v[c - 1], "fifo") && strcmp(v[c - 1], "edf"))
		return (error("The value must be exactly one of: fifo or edf"));
	return (1);
}

static void	fill_args(char **v, t_args *args)
{
	int	is_overflow;

	is_overflow = 0;
	args->number_of_coders = ft_atoi(v[1], &is_overflow);
	args->time_to_burnout = ft_atoi(v[2], &is_overflow);
	args->time_to_compile = ft_atoi(v[3], &is_overflow);
	args->time_to_debug = ft_atoi(v[4], &is_overflow);
	args->time_to_refactor = ft_atoi(v[5], &is_overflow);
	args->number_of_compiles_required = ft_atoi(v[6], &is_overflow);
	args->dongle_cooldown = ft_atoi(v[7], &is_overflow);
	args->scheduler = v[8];
}

int	parsing(int c, char **v, t_args *args)
{
	if (!validate_args(c, v))
		return (0);
	fill_args(v, args);
	return (1);
}
