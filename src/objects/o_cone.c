/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cone.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:09:54 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:09:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_cones(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->cones = ft_calloc(ids[6] + 1, sizeof(t_cone *));
	rt->n_cones = 0;
	while (i < (ids[6]))
	{
		rt->cones[i] = ft_calloc(1, sizeof(t_cone));
		rt->cones[i]->coords = ft_calloc(3, sizeof(double));
		rt->cones[i]->nvect = ft_calloc(3, sizeof(double));
		rt->cones[i]->rgb = ft_calloc(3, sizeof(double));
		rt->cones[i]->normal = ft_calloc(1, sizeof(t_vect3f));
		rt->cones[i]->right = ft_calloc(1, sizeof(t_vect3f));
		rt->cones[i]->up = ft_calloc(1, sizeof(t_vect3f));
		rt->cones[i]->axis = ft_calloc(1, sizeof(t_vect3f));
		rt->cones[i]->glossiness = 0.5;
		rt->cones[i]->checkerboard = NULL;
		rt->cones[i]->texture = NULL;
		rt->cones[i]->vector_map = NULL;
		rt->cones[i]->mode = DEFAULT;
		init_cone_discs(rt->cones[i]);
		i++;
	}
	rt->cones[i] = NULL;
}

int	precompute_cone_elements(t_cone *cone, char **split)
{
	double	diameter;

	diameter = ft_atof(split[3]);
	if (diameter <= 0)
		return (id_err("co", E_DIA_RANGE, E_RANGE_STRICT));
	cone->radius = diameter / 2.;
	cone->height = ft_atof(split[4]);
	if (cone->height <= 0)
		return (id_err("co", E_HEIGHT_RANGE, E_RANGE_STRICT));
	cone->half_angle = atan2(cone->radius, cone->height);
	cone->dist_term = sqrt(1 + pow(cone->half_angle, 2));
	return (0);
}

int	fill_cone(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_cones;
	if (check_format_cone(split))
		return (1);
	get_coords(rt->cones[i]->coords, split[1]);
	if (!get_nvect(rt->cones[i]->nvect, split[2]))
		return (id_err("co", E_VECT_RANGE, E_RANGE_NORM));
	if (rt->cones[i]->nvect[X] == 0 && rt->cones[i]->nvect[Y] == 0
		&& rt->cones[i]->nvect[Z] == 0)
		return (id_err("co", E_NORM_ZERO, NULL));
	*rt->cones[i]->normal = get_normal(rt->cones[i]->nvect[X],
			rt->cones[i]->nvect[Y], rt->cones[i]->nvect[Z]);
	*rt->cones[i]->axis = invert_vect3f(*rt->cones[i]->normal);
	if (precompute_cone_elements(rt->cones[i], split))
		return (1);
	if (!get_rgb(rt->cones[i]->rgb, split[5]))
		return (id_err("co", E_RGB_RANGE, E_RANGE_INT));
	set_cone_vects(rt->cones[i]);
	set_checkerboard_pointer("co", rt, split, &rt->cones[i]->checkerboard);
	set_texture_pointer("co", rt, split, &rt->cones[i]->texture);
	set_vector_map_pointer("co", rt, split, &rt->cones[i]->vector_map);
	get_cone_discs(rt->cones[i]);
	rt->n_cones++;
	return (0);
}

void	free_cones(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->cones[i] != NULL)
	{
		free(rt->cones[i]->coords);
		free(rt->cones[i]->nvect);
		free(rt->cones[i]->rgb);
		free_cone_discs(rt->cones[i]);
		free(rt->cones[i]->normal);
		free(rt->cones[i]->right);
		free(rt->cones[i]->up);
		free(rt->cones[i]->axis);
		free(rt->cones[i++]);
	}
	free(rt->cones);
}
