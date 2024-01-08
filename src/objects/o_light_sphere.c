/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_light_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:27:13 by plouda            #+#    #+#             */
/*   Updated: 2024/01/08 15:22:47 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_light_sphere(t_rt *rt)
{
	rt->light_sphere = ft_calloc(1, sizeof(t_sphere));
	rt->light_sphere->coords = ft_calloc(3, sizeof(double));
	rt->light_sphere->rgb = ft_calloc(3, sizeof(int));
	rt->light_sphere->rgb_ambient = ft_calloc(3, sizeof(int));
	rt->light_sphere->rgb_light = ft_calloc(3, sizeof(int));
}

int	fill_light_sphere(t_rt *rt)
{
	rt->light_sphere->coords[X] = rt->light->coords[X];
	rt->light_sphere->coords[Y] = rt->light->coords[Y];
	rt->light_sphere->coords[Z] = rt->light->coords[Z];
	rt->light_sphere->diameter = 0.1;
	rt->light_sphere->rgb[R] = rt->light->rgb[R];
	rt->light_sphere->rgb[G] = rt->light->rgb[G];
	rt->light_sphere->rgb[B] = rt->light->rgb[B];
	return (0);
}

void	free_light_sphere(t_rt *rt)
{
	free(rt->light_sphere->coords);
	free(rt->light_sphere->rgb);
	free(rt->light_sphere->rgb_ambient);
	free(rt->light_sphere->rgb_light);
	free(rt->light_sphere);
}
