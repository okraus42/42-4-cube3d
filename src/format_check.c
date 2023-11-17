/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:15:44 by plouda            #+#    #+#             */
/*   Updated: 2023/11/17 12:20:26 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	check_format_ambient(char **split)
{
	if (!has_spec_count(split, 3))
		return (id_err("A", E_SPEC, "2"));
	if (!is_floatable(split[1]))
		return (id_err("A", E_RATIO, E_INTFLOAT));
	if (!float_in_range(split[1]))
		return (id_err("A", E_RATIO, E_MAX_DIGITS));
	if (!is_rgb_format(split[2]))
		return (id_err("A", E_RGB, E_TRIAD_INT));
	return (0);
}

int	check_format_camera(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("C", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("C", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("C", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("C", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("C", E_VECT, E_MAX_DIGITS));
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
		return (id_err("L", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("L", E_COORD, E_MAX_DIGITS));
	if (!is_floatable(split[2]))
		return (id_err("L", E_BRIGHT, E_INTFLOAT));
	if (!float_in_range(split[2]))
		return (id_err("L", E_BRIGHT, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("L", E_RGB, E_TRIAD_INT));
	return (0);
}

int	check_format_sphere(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("sp", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("sp", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("sp", E_COORD, E_MAX_DIGITS));
	if (!is_floatable(split[2]))
		return (id_err("sp", E_DIA, E_INTFLOAT));
	if (!float_in_range(split[2]))
		return (id_err("sp", E_DIA, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("sp", E_RGB, E_TRIAD_INT));
	return (0);
}

int	check_format_plane(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("pl", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("pl", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("pl", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("pl", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("pl", E_VECT, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("pl", E_RGB, E_TRIAD_INT));
	return (0);
}

int	check_format_cylinder(char **split)
{
	if (!has_spec_count(split, 6))
		return (id_err("cy", E_SPEC, "5"));
	if (!is_float_triad(split[1]))
		return (id_err("cy", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("cy", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("cy", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("cy", E_VECT, E_MAX_DIGITS));
	if (!is_floatable(split[3]))
		return (id_err("cy", E_DIA, E_INTFLOAT));
	if (!float_in_range(split[3]))
		return (id_err("cy", E_DIA, E_MAX_DIGITS));
	if (!is_floatable(split[4]))
		return (id_err("cy", E_HEIGHT, E_INTFLOAT));
	if (!float_in_range(split[4]))
		return (id_err("cy", E_HEIGHT, E_MAX_DIGITS));
	if (!is_rgb_format(split[5]))
		return (id_err("cy", E_RGB, E_TRIAD_INT));
	return (0);
}
