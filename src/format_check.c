/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:15:44 by plouda            #+#    #+#             */
/*   Updated: 2023/11/13 09:17:16 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	check_format_ambient(char **split)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num != 3)
	{
		throw_error("A: Invalid number of specifiers, expected 2");
		return (1);
	}
	if (!is_floatable(split[1]))
	{
		throw_error("A: Wrong ratio format, expected float");
		return (1);
	}
	if (!float_in_range(split[1]))
	{
		throw_error("A: Wrong ratio format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[2]))
	{
		throw_error("A: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	check_format_camera(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("C: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("C: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("C: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_float_triad(split[2]))
	{
		throw_error("C: Wrong vector format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[2], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("C: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_integer(split[3]))
	{
		throw_error("C: Wrong FoV format, expected an integer");
		return (1);
	}
	return (0);
}

int	check_format_light(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("L: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("L: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("L: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_floatable(split[2]))
	{
		throw_error("L: Wrong brightness format, expected float");
		return (1);
	}
	if (!float_in_range(split[2]))
	{
		throw_error("L: Wrong brightness format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[3]))
	{
		throw_error("L: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	check_format_sphere(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("sp: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("sp: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("sp: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_floatable(split[2]))
	{
		throw_error("sp: Wrong diameter format, expected float");
		return (1);
	}
	if (!float_in_range(split[2]))
	{
		throw_error("sp: Wrong diameter format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[3]))
	{
		throw_error("sp: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}
