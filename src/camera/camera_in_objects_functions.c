/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_in_objects_functions.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:39:25 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:40:01 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	detect_camera_inside_sphere(t_sphere *sphere, t_camera *camera)
{
	double	cam_dist;

	cam_dist = point_distance(array_to_vect(sphere->coords),
			array_to_vect(camera->coords));
	if (cam_dist < sphere->radius
		&& absf(cam_dist - sphere->radius) > PRECISION)
		sphere->camera_inside = 1;
	else
		sphere->camera_inside = 0;
}

void	detect_camera_inside_plane(t_plane *plane, t_camera *camera)
{
	double	dist;
	double	subspace;

	dist = (-1 * plane->normal->x * plane->coords[X])
		- (plane->normal->y * plane->coords[Y])
		- (plane->normal->z * plane->coords[Z]);
	subspace = (plane->normal->x * camera->coords[X])
		+ (plane->normal->y * camera->coords[Y])
		+ (plane->normal->z * camera->coords[Z]) + dist;
	if (subspace < -PRECISION)
		plane->camera_inside = 1;
	else
		plane->camera_inside = 0;
}

void	detect_camera_inside_cylinder(t_cylinder *cylinder, t_camera *camera)
{
	if (point_lies_in_cylinder(array_to_vect(camera->coords),
			array_to_vect(cylinder->botcap->coords),
			array_to_vect(cylinder->topcap->coords), cylinder->radius))
		cylinder->camera_inside = 1;
	else
		cylinder->camera_inside = 0;
}

void	detect_camera_inside_disc(t_disc *disc, t_camera *camera)
{
	double	dist;
	double	subspace;

	dist = (-1 * disc->normal->x * disc->coords[X])
		- (disc->normal->y * disc->coords[Y])
		- (disc->normal->z * disc->coords[Z]);
	subspace = (disc->normal->x * camera->coords[X])
		+ (disc->normal->y * camera->coords[Y])
		+ (disc->normal->z * camera->coords[Z]) + dist;
	if (subspace < -PRECISION)
		disc->camera_inside = 1;
	else
		disc->camera_inside = 0;
}

void	detect_camera_inside_cone(t_cone *cone, t_camera *camera)
{
	if (point_lies_in_cone(array_to_vect(camera->coords),
			*cone, cone->radius, cone->height))
		cone->camera_inside = 1;
	else
		cone->camera_inside = 0;
}
