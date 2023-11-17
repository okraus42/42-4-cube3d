/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:15:44 by plouda            #+#    #+#             */
/*   Updated: 2023/11/17 12:10:25 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	check_format_ambient(char **split)
{
	if (!has_spec_count(split, 3))
		return (id_err("A", E_SPEC, "2"));
	if (!is_floatable(split[1]))
		return (id_err("A", E_RATIO, "int or float"));
	if (!float_in_range(split[1]))
		return (id_err("A", E_RATIO, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[2]))
		return (id_err("A", E_RGB, "3 integers separated by commas"));
	return (0);
}

int	check_format_camera(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("C", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("C", E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err("C", E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_float_triad(split[2]))
		return (id_err("C", E_VECT, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[2]))
		return (id_err("C", E_VECT, "max. 4 integer digits and 5 decimals"));
	if (!is_integer(split[3]))
		return (id_err("C", E_FOV, "an integer"));
	return (0);
}

int	check_format_light(char **split)
{
	int		num;

	if (!has_spec_count(split, 4))
		return (id_err("L", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("L", E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err("L", E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_floatable(split[2]))
		return (id_err("L", E_BRIGHT, "int or float"));
	if (!float_in_range(split[2]))
		return (id_err("L", E_BRIGHT, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[3]))
		return (id_err("L", E_RGB, "3 integers separated by commas"));
	return (0);
}

int	check_format_sphere(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("sp", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("sp", E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err("sp", E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_floatable(split[2]))
		return (id_err("sp", E_DIA, "int or float"));
	if (!float_in_range(split[2]))
		return (id_err("sp", E_DIA, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[3]))
		return (id_err("sp", E_RGB, "3 integers separated by commas"));
	return (0);
}

int	check_format_plane(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("pl", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("pl", E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err("pl", E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_float_triad(split[2]))
		return (id_err("pl", E_VECT, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[2]))
		return (id_err("pl", E_VECT, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[3]))
		return (id_err("pl", E_RGB, "3 integers separated by commas"));
	return (0);
}

int	check_format_cylinder(char **split)
{
	if (!has_spec_count(split, 6))
		return (id_err("cy", E_SPEC, "5"));
	if (!is_float_triad(split[1]))
		return (id_err("cy", E_COORD, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[1]))
		return (id_err("cy", E_COORD, "max. 4 integer digits and 5 decimals"));
	if (!is_float_triad(split[2]))
		return (id_err("cy", E_VECT, "3 integers/floats separated by commas"));
	if (!triad_in_range(split[2]))
		return (id_err("cy", E_VECT, "max. 4 integer digits and 5 decimals"));
	if (!is_floatable(split[3]))
		return (id_err("cy", E_DIA, "int or float"));
	if (!float_in_range(split[3]))
		return (id_err("cy", E_DIA, "max. 4 integer digits and 5 decimals"));
	if (!is_floatable(split[4]))
		return (id_err("cy", E_HEIGHT, "int or float"));
	if (!float_in_range(split[4]))
		return (id_err("cy", E_HEIGHT, "max. 4 integer digits and 5 decimals"));
	if (!is_rgb_format(split[5]))
		return (id_err("cy", E_RGB, "3 integers separated by commas"));
	return (0);
}
