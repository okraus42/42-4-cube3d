/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_light_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:27:13 by plouda            #+#    #+#             */
/*   Updated: 2024/02/09 15:51:57 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_light_sphere(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->light_spheres = ft_calloc(ids[2] + 1, sizeof(t_sphere *));
	rt->n_lights = 0;
	while (i < (ids[2]))
	{
		rt->light_spheres[i] = ft_calloc(1, sizeof(t_sphere));
		rt->light_spheres[i]->coords = ft_calloc(3, sizeof(double));
		rt->light_spheres[i]->rgb = ft_calloc(3, sizeof(int));
		rt->light_spheres[i]->rgb_ambient = ft_calloc(3, sizeof(int));
		rt->light_spheres[i]->rgb_light = ft_calloc(3, sizeof(int));
		//rt->light_spheres[i]->brightness = 1;
		rt->light_spheres[i++]->mode = DEFAULT;
	}
	rt->light_spheres[i] = NULL;
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
	int			i;

	i = rt->n_lights;
	if (check_format_light(split))
		return (1);
	get_coords(rt->light_spheres[i]->coords, split[1]);
	rt->light_spheres[i]->brightness = ft_atof(split[2]);
	if (rt->light_spheres[i]->brightness < 0.0 || rt->light_spheres[i]->brightness > 1.0)
		return (id_err("L", E_BRIGHT_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->light_spheres[i]->rgb, split[3]))
		return (id_err("L", E_RGB_RANGE, E_RANGE_INT));
	rt->n_lights++;
	return (0);
}

int	fill_light_sphere(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_lights;
	if (check_format_light(split))
		return (1);
	get_coords(rt->light_spheres[i]->coords, split[1]);
	rt->light_spheres[i]->brightness = ft_atof(split[2]);
	if (rt->light_spheres[i]->brightness < 0.0 || rt->light_spheres[i]->brightness > 1.0)
		return (id_err("L", E_BRIGHT_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->light_spheres[i]->rgb, split[3]))
		return (id_err("L", E_RGB_RANGE, E_RANGE_INT));
	rt->light_spheres[i]->diameter = 0.1;
	rt->n_lights++;
	return (0);
}

void	free_light_sphere(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_lights)
	{
		free(rt->light_spheres[i]->coords);
		free(rt->light_spheres[i]->rgb);
		free(rt->light_spheres[i]->rgb_ambient);
		free(rt->light_spheres[i]->rgb_light);
		free(rt->light_spheres[i++]);
	}
	free(rt->light_spheres);
}
