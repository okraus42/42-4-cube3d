/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 09:43:58 by plouda            #+#    #+#             */
/*   Updated: 2024/01/23 12:27:58 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"


void	get_light_clr(t_light *light, int *rgb_light, int *rgb)
{

	rgb_light[R] = ((int)((rgb[R] + 1) * (light->rgb[R] + 1)) - 1) >> 8;
	rgb_light[G] = ((int)((rgb[G] + 1) * (light->rgb[G] + 1)) - 1) >> 8;
	rgb_light[B] = ((int)((rgb[B] + 1) * (light->rgb[B] + 1)) - 1) >> 8;
}


void	precompute_light(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		get_light_clr(rt->light, rt->spheres[i]->rgb_light, rt->spheres[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		get_light_clr(rt->light, rt->planes[i]->rgb_light, rt->planes[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_cylinders)
	{
		get_light_clr(rt->light, rt->cylinders[i]->rgb_light, rt->cylinders[i]->rgb);
		get_light_clr(rt->light, rt->cylinders[i]->botcap->rgb_light, rt->cylinders[i]->rgb);
		get_light_clr(rt->light, rt->cylinders[i]->topcap->rgb_light, rt->cylinders[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		get_light_clr(rt->light, rt->cones[i]->rgb_light, rt->cones[i]->rgb);
		get_light_clr(rt->light, rt->cones[i]->base->rgb_light, rt->cones[i]->rgb);
		get_light_clr(rt->light, rt->cones[i]->pinnacle->rgb_light, rt->cones[i]->rgb);
		i++;
	}
}

void	get_ambient_clr(t_ambient *ambient, int *rgb_ambient, int *rgb)
{

	rgb_ambient[R] = ((int)((rgb[R] + 1) * (ambient->rgb[R] * ambient->ratio + 1)) - 1) >> 8;
	rgb_ambient[G] = ((int)((rgb[G] + 1) * (ambient->rgb[G] * ambient->ratio + 1)) - 1) >> 8;
	rgb_ambient[B] = ((int)((rgb[B] + 1) * (ambient->rgb[B] * ambient->ratio + 1)) - 1) >> 8;
}

void	precompute_ambient(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		get_ambient_clr(rt->ambient, rt->spheres[i]->rgb_ambient, rt->spheres[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		get_ambient_clr(rt->ambient, rt->planes[i]->rgb_ambient, rt->planes[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_cylinders)
	{
		get_ambient_clr(rt->ambient, rt->cylinders[i]->rgb_ambient, rt->cylinders[i]->rgb);
		get_ambient_clr(rt->ambient, rt->cylinders[i]->botcap->rgb_ambient, rt->cylinders[i]->rgb);
		get_ambient_clr(rt->ambient, rt->cylinders[i]->topcap->rgb_ambient, rt->cylinders[i]->rgb);
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		get_ambient_clr(rt->ambient, rt->cones[i]->rgb_ambient, rt->cones[i]->rgb);
		get_ambient_clr(rt->ambient, rt->cones[i]->base->rgb_ambient, rt->cones[i]->rgb);
		get_ambient_clr(rt->ambient, rt->cones[i]->pinnacle->rgb_ambient, rt->cones[i]->rgb);
		i++;
	}
}
