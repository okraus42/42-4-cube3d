/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:12:40 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	update_object_ref(t_rayfinder *rf, void *object, t_object flag)
{
	if (rf->t < rf->t_near)
	{
		rf->t_near = rf->t;
		rf->object_flag = flag;
		rf->object_ptr = object;
	}
	return (1);
}

static void	find_intersections_1(t_master *master,
	t_ray ray, t_rayfinder *rf, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_spheres)
	{
		if (intersect_sphere(ray, master->rt->spheres[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->spheres[i], SPHERE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		if (intersect_plane(ray, master->rt->planes[i], &rf->t, PLANE))
			*flag = update_object_ref(rf, master->rt->planes[i], PLANE);
		i++;
	}
}

static void	find_intersections_2(t_master *master,
	t_ray ray, t_rayfinder *rf, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_cylinders)
	{
		if (intersect_cylinder(ray, master->rt->cylinders[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->cylinders[i], CYLINDER);
		if (intersect_disc(ray, master->rt->cylinders[i]->topcap, &rf->t))
			*flag = update_object_ref(rf,
					master->rt->cylinders[i]->topcap, DISC);
		if (intersect_disc(ray, master->rt->cylinders[i]->botcap, &rf->t))
			*flag = update_object_ref(rf,
					master->rt->cylinders[i]->botcap, DISC);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cones)
	{
		if (intersect_cone(ray, master->rt->cones[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->cones[i], CONE);
		if (intersect_disc(ray, master->rt->cones[i]->base, &rf->t))
			*flag = update_object_ref(rf, master->rt->cones[i]->base, DISC);
		i++;
	}
}

int	find_intersections(t_master *master,
	t_ray ray, t_rayfinder *rf, t_raytype type)
{
	int	i;
	int	flag;

	flag = 0;
	find_intersections_1(master, ray, rf, &flag);
	find_intersections_2(master, ray, rf, &flag);
	i = 0;
	while (i < master->rt->n_lights)
	{
		if (type != SHADOW && intersect_sphere(ray,
				master->rt->light_spheres[i], &rf->t))
			flag = update_object_ref(rf, master->rt->light_spheres[i], LIGHT);
		i++;
	}
	return (flag);
}

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
		light_shader(rf, object_ptr, master);
}

void	reset_rayfinder(t_rayfinder *rf)
{
	rf->t = (double)INT_MAX;
	rf->t_near = (double)INT_MAX;
	rf->object_flag = EMPTY;
	rf->object_ptr = NULL;
	rf->clr = 0x000000FF;
}

t_rayfinder	init_rayfinder(t_master	*master)
{
	t_rayfinder	rf;

	rf.ratio = (double)master->w_width / (double)master->w_height;
	rf.fov = master->rt->camera->fov;
	rf.scale = tan(rad(rf.fov * 0.5));
	rf.t_near = (double)INT_MAX;
	rf.t = (double)INT_MAX;
	rf.object_flag = EMPTY;
	rf.object_ptr = NULL;
	rf.clr = 0x000000FF;
	rf.origin = shift_origin(master->rt->camera->matrix);
	rf.cam_mat = master->rt->camera->matrix;
	rf.shadowray.direction = (t_vect3f){};
	rf.shadowray.origin = (t_vect3f){};
	rf.shadow_inter = (t_vect3f){};
	rf.inter_dist = 0;
	rf.w_height = master->w_height;
	rf.w_width = master->w_width;
	return (rf);
}

/* since origin is 0,0,0, it's just shifting it to "from" point from origin.
Proper matrix multiplication becomes relevant when:
1/ origin is not 0,0,0
2/ homogenous coordinate is not 1 (e.g. after projection matrix)
This could also be encoded in the matrix itself instead of 0,0,0,1
*/
t_vect3f	shift_origin(double **cam)
{
	t_vect3f	origin;

	origin.x = cam[3][0];
	origin.y = cam[3][1];
	origin.z = cam[3][2];
	return (origin);
}

void	change_ray_direction(double **cam, t_vect3f *direction, t_vect3f temp)
{
	direction->x = temp.x * cam[0][0] + temp.y * cam[1][0] + temp.z * cam[2][0];
	direction->y = temp.x * cam[0][1] + temp.y * cam[1][1] + temp.z * cam[2][1];
	direction->z = temp.x * cam[0][2] + temp.y * cam[1][2] + temp.z * cam[2][2];
	normalize(direction);
}

void	update_ray_direction(t_rayfinder *rf, t_ray *ray, int x, int y)
{
	ray->origin.x = rf->origin.x;
	ray->origin.y = rf->origin.y;
	ray->origin.z = rf->origin.z;
	ray->direction.x = (2. * ((x + 0.5) / (double)rf->w_width) - 1.)
		* rf->ratio * rf->scale;
	ray->direction.y = (1. - 2. * ((y + 0.5) / (double)rf->w_height))
		* rf->scale;
	ray->direction.z = -1;
	change_ray_direction(rf->cam_mat, &ray->direction, ray->direction);
}

void	camera_things(t_master *master)
{
	update_camera_matrix(master->rt->camera);
	detect_camera_inside_objects(master->rt);
}

void	find_rays(t_master *master)
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
