/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:13:21 by plouda            #+#    #+#             */
/*   Updated: 2023/11/13 09:14:27 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

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
		if (str[i] == '-')
			i++;
		if (!ft_isdigit(str[i])) // is there at least one digit before the decimal
			flag++;
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (str[i] == '.')
			i++;
		if (!ft_isdigit(str[i]) && flag) // if no digits on either side, invalid
			return (0);
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (j != 2)
		{
			if (str[i++] != ',') // is there a comma at the expected place
				return (0);
		}
		else
		{
			if (str[i]) // is there anything after the last digit
				return (0); 
		}
		j++;
	}
	if (!str[i] && j < 3) // too short
		return (0);
	return (1);
}

int	is_rgb_format(char *str)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		if (str[i] == '-')
			i++;
		if (!ft_isdigit(str[i])) // is there at least one digit
			return (0);
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (j != 2)
		{
			if (str[i++] != ',') // is there a comma at the expected place
				return (0);
		}
		else
		{
			if (str[i]) // is there anything after the last digit
				return (0); 
		}
		j++;
	}
	if (!str[i] && j < 3) // too short
		return (0);
	return (1);
}

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
