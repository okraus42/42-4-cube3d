/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:39:23 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

t_vect3f	get_intersection(t_vect3f origin, t_vect3f direction, double t)
{
	t_vect3f	intersection;

	intersection.x = origin.x + t * direction.x;
	intersection.y = origin.y + t * direction.y;
	intersection.z = origin.z + t * direction.z;
	return (intersection);
}

void	shade_nearest_object(int flag,
	void *object_ptr, t_rayfinder *rf, t_master *master)
{
	t_vect3f	intersection;

	intersection = get_intersection(rf->ray.origin, rf->ray.direction,
			rf->t_near);
	if (flag == SPHERE)
		sphere_shader(rf, intersection, object_ptr, master);
	else if (flag == PLANE)
		plane_shader(rf, intersection, object_ptr, master);
	else if (flag == CYLINDER)
		cylinder_shader(rf, intersection, object_ptr, master);
	else if (flag == DISC)
		disc_shader(rf, intersection, object_ptr, master);
	else if (flag == CONE)
		cone_shader(rf, intersection, object_ptr, master);
	else if (flag == LIGHT)
		light_sphere_shader(rf, object_ptr);
}

void	camera_things(t_master *master)
{
	update_camera_matrix(master->rt->camera);
	detect_camera_inside_objects(master->rt);
}

void	cast_rays(t_master *master)
{
	int			x;
	int			y;
	t_ray		**rays;
	t_rayfinder	rf;

	camera_things(master);
	rays = malloc(sizeof(t_ray *) * (master->w_width));
	rf = init_rayfinder(master);
	x = -1;
	while (++x < master->w_width)
	{
		rays[x] = malloc(sizeof(t_ray) * (master->w_height));
		y = -1;
		while (++y < master->w_height)
		{
			reset_rayfinder(&rf);
			update_ray_direction(&rf, &rays[x][y], x, y);
			find_intersections(master, rays[x][y], &rf, PRIMARY);
			rf.ray = rays[x][y];
			shade_nearest_object(rf.object_flag, rf.object_ptr, &rf, master);
			mlx_put_pixel(master->img, x, y, rf.clr);
		}
		free(rays[x]);
	}
	free(rays);
}
