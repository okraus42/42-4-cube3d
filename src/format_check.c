/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:15:44 by plouda            #+#    #+#             */
/*   Updated: 2023/11/16 11:45:47 by plouda           ###   ########.fr       */
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
		return (id_err('A', E_SPEC, "2"));
	if (!is_floatable(split[1]))
		return (id_err('A', E_RATIO, "int or float"));
	if (!float_in_range(split[1]))
		return (id_err('A', E_RATIO, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[2]))
		return (id_err('A', E_RGB, "3 integers separated by commas"));
	return (0);
}

int	check_format_camera(char **split)
{
	int		num;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
		return (id_err('C', E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err('C', E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err('C', E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_float_triad(split[2]))
		return (id_err('C', E_VECT, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[2]))
		return (id_err('C', E_VECT, "max. 4 integer digits and 5 decimals"));
	if (!is_integer(split[3]))
		return (id_err('C', E_FOV, "an integer"));
	return (0);
}

int	check_format_light(char **split)
{
	int		num;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
		return (id_err('L', E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err('L', E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err('L', E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_floatable(split[2]))
		return (id_err('L', E_BRIGHT, "int or float"));
	if (!float_in_range(split[2]))
		return (id_err('L', E_BRIGHT, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[3]))
		return (id_err('L', E_RGB, "3 integers separated by commas"));
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

int	check_format_plane(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("pl: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("pl: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("pl: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_float_triad(split[2]))
	{
		throw_error("pl: Wrong vector format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[2], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("pl: Wrong vector format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_rgb_format(split[3]))
	{
		throw_error("pl: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	check_format_cylinder(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 6)
	{
		throw_error("cy: Invalid number of specifiers, expected 5");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("cy: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("cy: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_float_triad(split[2]))
	{
		throw_error("cy: Wrong vector format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[2], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("cy: Wrong vector format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_floatable(split[3]))
	{
		throw_error("cy: Wrong diameter format, expected float");
		return (1);
	}
	if (!float_in_range(split[3]))
	{
		throw_error("cy: Wrong diameter format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_floatable(split[4]))
	{
		throw_error("cy: Wrong height format, expected float");
		return (1);
	}
	if (!float_in_range(split[4]))
	{
		throw_error("cy: Wrong height format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[5]))
	{
		throw_error("cy: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}
