/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_light_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:27:13 by plouda            #+#    #+#             */
/*   Updated: 2024/02/08 12:26:08 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_light_sphere(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->light_spheres = ft_calloc(ids[2] + 1, sizeof(t_sphere *));
	while (i < (ids[2]))
	{
		rt->light_spheres[i] = ft_calloc(1, sizeof(t_sphere));
		rt->light_spheres[i]->coords = ft_calloc(3, sizeof(double));
		rt->light_spheres[i]->rgb = ft_calloc(3, sizeof(int));
		rt->light_spheres[i]->rgb_ambient = ft_calloc(3, sizeof(int));
		rt->light_spheres[i]->rgb_light = ft_calloc(3, sizeof(int));
		rt->light_spheres[i++]->mode = DEFAULT;
	}
	rt->light_spheres[i] = NULL;
}

int	fill_light_sphere(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_lights)
	{
		rt->light_spheres[i]->coords[X] = rt->lights[i]->coords[X];
		rt->light_spheres[i]->coords[Y] = rt->lights[i]->coords[Y];
		rt->light_spheres[i]->coords[Z] = rt->lights[i]->coords[Z];
		rt->light_spheres[i]->diameter = 0.1;
		rt->light_spheres[i]->rgb[R] = rt->lights[i]->rgb[R];
		rt->light_spheres[i]->rgb[G] = rt->lights[i]->rgb[G];
		rt->light_spheres[i]->rgb[B] = rt->lights[i]->rgb[B];
		i++;
	}
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
