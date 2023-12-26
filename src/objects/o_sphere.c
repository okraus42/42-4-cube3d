/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_sphere.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:40:34 by plouda            #+#    #+#             */
/*   Updated: 2023/12/26 16:32:05 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_spheres(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->spheres = ft_calloc(ids[3] + 1, sizeof(t_sphere *));
	rt->n_spheres = 0;
	while (i < (ids[3]))
	{
		rt->spheres[i] = ft_calloc(1, sizeof(t_sphere));
		rt->spheres[i]->coords = ft_calloc(3, sizeof(double));
		rt->spheres[i]->rgb = ft_calloc(3, sizeof(int));
		rt->spheres[i++]->rgb_ambient = ft_calloc(3, sizeof(int));
	}
	rt->spheres[i] = NULL;
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

int	fill_sphere(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_spheres;
	if (check_format_sphere(split))
		return (1);
	get_coords(rt->spheres[i]->coords, split[1]);
	rt->spheres[i]->diameter = ft_atof(split[2]);
	if (rt->spheres[i]->diameter <= 0)
		return (id_err("sp", E_DIA_RANGE, E_RANGE_STRICT));
	if (!get_rgb(rt->spheres[i]->rgb, split[3]))
		return (id_err("sp", E_RGB_RANGE, E_RANGE_INT));
	rt->n_spheres++;
	return (0);
}

void	free_spheres(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->spheres[i] != NULL)
	{
		free(rt->spheres[i]->coords);
		free(rt->spheres[i]->rgb);
		free(rt->spheres[i]->rgb_ambient);
		free(rt->spheres[i++]);
	}
	free(rt->spheres);
}
