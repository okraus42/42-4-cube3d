/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:02:32 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 16:03:22 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	is_floatable(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == '.')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	is_integer(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	float_in_range(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		i++;
		len++;
	}
	if (len > 4)
		return (0);
	len = 0;
	if (str[i] == '.')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		i++;
		len++;
	}
	if (len > 5)
		return (0);
	return (1);
}

int	triad_in_range(char *triad)
{
	char	**subsplit;

	subsplit = ft_split(triad, ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		ft_free_split(&subsplit);
		return (0);
	}
	ft_free_split(&subsplit);
	return (1);
}
