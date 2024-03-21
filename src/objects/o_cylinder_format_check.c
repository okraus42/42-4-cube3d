/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder_format_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:15:23 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:16:12 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	check_cylinder_spec_count(char **split)
{
	int	spec_count_flag;

	spec_count_flag = has_spec_count(split, 6);
	if (!spec_count_flag)
		return (id_err("cy", E_SPEC, "5"));
	else if (spec_count_flag < 0)
		return (id_err("cy", "Invalid specifier format",
				"unique texture identifiers after the 5th specifier"));
	return (0);
}

int	check_format_cylinder(char **split)
{
	if (check_cylinder_spec_count(split))
		return (1);
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
