/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_light.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:29:51 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 15:39:50 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_light(t_rt *rt)
{
	rt->light = malloc(sizeof(t_light));
	rt->light->coords = malloc(sizeof(double) * 3);
	rt->light->rgb = malloc(sizeof(int) * 3);
}

int	check_format_light(char **split)
{
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

int	fill_light(t_rt *rt, char **split)
{
	if (check_format_light(split))
		return (1);
	get_coords(rt->light->coords, split[1]);
	rt->light->brightness = ft_atof(split[2]);
	if (rt->light->brightness < 0.0 || rt->light->brightness > 1.0)
		return (id_err("L", E_BRIGHT_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->light->rgb, split[3]))
		return (id_err("L", E_RGB_RANGE, E_RANGE_INT));
	return (0);
}

void	free_light(t_rt *rt)
{
	free(rt->light->coords);
	free(rt->light->rgb);
	free(rt->light);
}
