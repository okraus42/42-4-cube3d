/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:54:05 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 15:29:27 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_ambient(t_rt *rt)
{
	rt->ambient = malloc(sizeof(t_ambient));
	rt->ambient->rgb = malloc(sizeof(int) * 3);
}

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

int	fill_ambient(t_rt *rt, char **split)
{
	if (check_format_ambient(split))
		return (1);
	rt->ambient->ratio = ft_atof(split[1]);
	if (rt->ambient->ratio < 0.0 || rt->ambient->ratio > 1.0)
		return (id_err("A", E_RATIO_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->ambient->rgb, split[2]))
		return (id_err("A", E_RGB_RANGE, E_RANGE_INT));
	return (0);
}

void	free_ambient(t_rt *rt)
{
	free(rt->ambient->rgb);
	free(rt->ambient);
}
