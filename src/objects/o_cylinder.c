/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:43:36 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 14:54:46 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_cylinders(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->cylinders = ft_calloc(ids[5] + 1, sizeof(t_cylinder *));
	rt->n_cylinders = 0;
	while (i < (ids[5]))
	{
		rt->cylinders[i] = ft_calloc(1, sizeof(t_cylinder));
		rt->cylinders[i]->coords = ft_calloc(3, sizeof(double));
		rt->cylinders[i]->nvect = ft_calloc(3, sizeof(double));
		rt->cylinders[i]->rgb = ft_calloc(3, sizeof(double));
		rt->cylinders[i]->normal = ft_calloc(1, sizeof(t_vect3f));
		rt->cylinders[i]->right = ft_calloc(1, sizeof(t_vect3f));
		rt->cylinders[i]->up = ft_calloc(1, sizeof(t_vect3f));
		rt->cylinders[i]->glossiness = 0.5;
		rt->cylinders[i]->checkerboard = NULL;
		rt->cylinders[i]->texture = NULL;
		rt->cylinders[i]->vector_map = NULL;
		rt->cylinders[i]->mode = DEFAULT;
		init_discs(rt->cylinders[i]);
		i++;
	}
	rt->cylinders[i] = NULL;
}

int	check_cylinder_spec_count(char **split)
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

int	precompute_cylinder_elements(t_cylinder *cylinder, char **split)
{
	double	diameter;
	
	diameter = ft_atof(split[3]);
	if (diameter <= 0)
		return (id_err("cy", E_DIA_RANGE, E_RANGE_STRICT));
	cylinder->radius = diameter / 2.;
	cylinder->height = ft_atof(split[4]);
	if (cylinder->height <= 0)
		return (id_err("cy", E_HEIGHT_RANGE, E_RANGE_STRICT));
	return (0);
}

int	fill_cylinder(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_cylinders;
	if (check_format_cylinder(split))
		return (1);
	get_coords(rt->cylinders[i]->coords, split[1]);
	if (!get_nvect(rt->cylinders[i]->nvect, split[2]))
		return (id_err("cy", E_VECT_RANGE, E_RANGE_NORM));
	if (rt->cylinders[i]->nvect[X] == 0 && rt->cylinders[i]->nvect[Y] == 0
		&& rt->cylinders[i]->nvect[Z] == 0)
		return (id_err("cy", E_NORM_ZERO, NULL));
	*rt->cylinders[i]->normal = get_normal(rt->cylinders[i]->nvect[X],
			rt->cylinders[i]->nvect[Y], rt->cylinders[i]->nvect[Z]);
	if (precompute_cylinder_elements(rt->cylinders[i], split))
		return (1);
	if (!get_rgb(rt->cylinders[i]->rgb, split[5]))
		return (id_err("cy", E_RGB_RANGE, E_RANGE_INT));
	set_cylinder_vects(rt->cylinders[i]);
	set_checkerboard_pointer("cy", rt, split, &rt->cylinders[i]->checkerboard);
	set_texture_pointer("cy", rt, split, &rt->cylinders[i]->texture);
	set_vector_map_pointer("cy", rt, split, &rt->cylinders[i]->vector_map);
	get_discs(rt->cylinders[i]);
	rt->n_cylinders++;
	return (0);
}

void	free_cylinders(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->cylinders[i] != NULL)
	{
		free(rt->cylinders[i]->coords);
		free(rt->cylinders[i]->nvect);
		free(rt->cylinders[i]->rgb);
		free_discs(rt->cylinders[i]);
		free(rt->cylinders[i]->normal);
		free(rt->cylinders[i]->right);
		free(rt->cylinders[i]->up);
		free(rt->cylinders[i++]);
	}
	free(rt->cylinders);
}
