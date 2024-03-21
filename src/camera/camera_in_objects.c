/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_in_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:40:33 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:39:57 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	detect_camera_inside_objects(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
		detect_camera_inside_sphere(rt->spheres[i++], rt->camera);
	i = 0;
	while (i < rt->n_planes)
		detect_camera_inside_plane(rt->planes[i++], rt->camera);
	i = 0;
	while (i < rt->n_cylinders)
	{
		detect_camera_inside_cylinder(rt->cylinders[i], rt->camera);
		detect_camera_inside_disc(rt->cylinders[i]->botcap, rt->camera);
		detect_camera_inside_disc(rt->cylinders[i]->topcap, rt->camera);
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		detect_camera_inside_cone(rt->cones[i], rt->camera);
		detect_camera_inside_disc(rt->cones[i]->base, rt->camera);
		detect_camera_inside_disc(rt->cones[i]->pinnacle, rt->camera);
		i++;
	}
}
