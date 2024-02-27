/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_in_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:40:33 by plouda            #+#    #+#             */
/*   Updated: 2024/02/27 10:49:10 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	detect_camera_inside_sphere(t_sphere *sphere, t_camera *camera)
{
	double	cam_dist;

	cam_dist = point_distance(array_to_vect(sphere->coords), array_to_vect(camera->coords)); //testing
	if (cam_dist < sphere->radius && absf(cam_dist - sphere->radius) > PRECISION)
		sphere->camera_inside = 1;
	else
		sphere->camera_inside = 0;
}

void	detect_camera_inside_plane(t_plane *plane, t_camera *camera)
{
	double	dist;
	double	subspace;
	
	dist = (-1 * plane->normal->x * plane->coords[X]) \
			- (plane->normal->y * plane->coords[Y]) \
			- (plane->normal->z * plane->coords[Z]);
	subspace = (plane->normal->x * camera->coords[X]) \
			+ (plane->normal->y * camera->coords[Y]) \
			+ (plane->normal->z * camera->coords[Z]) + dist;
	if (subspace < -PRECISION)
		plane->camera_inside = 1;
	else
		plane->camera_inside = 0;
}

void	detect_camera_inside_cylinder(t_cylinder *cylinder, t_camera *camera)
{
	if (point_lies_in_cylinder(array_to_vect(camera->coords), array_to_vect(cylinder->botcap->coords), array_to_vect(cylinder->topcap->coords), cylinder->radius))
		cylinder->camera_inside = 1;
	else
		cylinder->camera_inside = 0;
}

void	detect_camera_inside_disc(t_disc *disc, t_camera *camera) // is this needed?
{
	double	dist;
	double	subspace;

	dist = (-1 * disc->normal->x * disc->coords[X]) \
			- (disc->normal->y * disc->coords[Y]) \
			- (disc->normal->z * disc->coords[Z]);
	subspace = (disc->normal->x * camera->coords[X]) \
			+ (disc->normal->y * camera->coords[Y]) \
			+ (disc->normal->z * camera->coords[Z]) + dist;
	if (subspace < -PRECISION)
		disc->camera_inside = 1;
	else
		disc->camera_inside = 0;
}

void	detect_camera_inside_cone(t_cone *cone, t_camera *camera)
{
	if (point_lies_in_cone(array_to_vect(camera->coords), *cone, cone->radius, cone->height))
		cone->camera_inside = 1;
	else
		cone->camera_inside = 0;
}

void	detect_camera_inside_objects(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		detect_camera_inside_sphere(rt->spheres[i], rt->camera);
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		detect_camera_inside_plane(rt->planes[i], rt->camera);
		i++;
	}
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
