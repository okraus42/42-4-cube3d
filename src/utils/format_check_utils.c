/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:13:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	iter_float_triad(char *str, int *i, int *j, int flag)
{
	if (str[*i] == '-')
		(*i)++;
	if (!ft_isdigit(str[*i]))
		flag++;
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if (str[*i] == '.')
		(*i)++;
	if (!ft_isdigit(str[*i]) && flag)
		return (0);
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if ((*j) != 2)
	{
		if (str[(*i)++] != ',')
			return (0);
	}
	else
	{
		if (str[*i])
			return (0);
	}
	(*j)++;
	return (1);
}

static int	iter_int_triad(char *str, int *i, int *j)
{
	if (str[*i] == '-')
		(*i)++;
	if (!ft_isdigit(str[*i]))
		return (0);
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if (*j != 2)
	{
		if (str[(*i)++] != ',')
			return (0);
	}
	else
	{
		if (str[*i])
			return (0);
	}
	(*j)++;
	return (1);
}

int	is_float_triad(char *str)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		flag = 0;
		if (!iter_float_triad(str, &i, &j, flag))
			return (0);
	}
	if (!str[i] && j < 3)
		return (0);
	return (1);
}

int	is_rgb_format(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		if (!iter_int_triad(str, &i, &j))
			return (0);
	}
	if (!str[i] && j < 3)
		return (0);
	return (1);
}

int	has_spec_count(char **split, int target)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num != target)
		return (0);
	return (1);
}
