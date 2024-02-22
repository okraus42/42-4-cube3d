/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2024/02/22 16:30:10 by plouda           ###   ########.fr       */
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

int	find_intersections(t_master *master, t_ray ray, t_rayfinder *rf, t_raytype type)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (i < master->rt->n_spheres)
	{
		if (intersect_sphere(ray, master->rt->spheres[i], &rf->t))
			flag = update_object_ref(rf, master->rt->spheres[i], SPHERE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		if (intersect_plane(ray, master->rt->planes[i], &rf->t, PLANE))
			flag = update_object_ref(rf, master->rt->planes[i], PLANE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cylinders)
	{
		if (intersect_cylinder(ray, master->rt->cylinders[i], &rf->t))
			flag = update_object_ref(rf, master->rt->cylinders[i], CYLINDER);
		if (intersect_disc(ray, master->rt->cylinders[i]->topcap, &rf->t))
			flag = update_object_ref(rf, master->rt->cylinders[i]->topcap, DISC);
		if (intersect_disc(ray, master->rt->cylinders[i]->botcap, &rf->t))
			flag = update_object_ref(rf, master->rt->cylinders[i]->botcap, DISC);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cones)
	{
		if (intersect_cone(ray, master->rt->cones[i], &rf->t))
			flag = update_object_ref(rf, master->rt->cones[i], CONE);
		/* if (intersect_disc(ray, master->rt->cones[i]->pinnacle, &rf->t))
			flag = update_object_ref(rf, master->rt->cones[i]->pinnacle, DISC); */
		if (intersect_disc(ray, master->rt->cones[i]->base, &rf->t))
			flag = update_object_ref(rf, master->rt->cones[i]->base, DISC);
		i++;
	}
	i = 0;
	while (i < master->rt->n_lights)
	{
		if (type != SHADOW && intersect_sphere(ray, master->rt->light_spheres[i], &rf->t))
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

void	shade_nearest_object(int flag, void *object_ptr, t_rayfinder *rf, t_ray ray, t_master *master)
{
	t_vect3f	intersection;

	intersection = get_intersection(ray.origin, ray.direction, rf->t_near);
	if (flag == SPHERE)
		sphere_shader(rf, intersection, object_ptr, master);
	else if (flag == PLANE)
		plane_shader(rf, intersection, object_ptr, master);
	else if (flag == CYLINDER)
		cylinder_shader(rf, intersection, object_ptr, master, ray);
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

	rf.ratio = (double)WIDTH / (double)HEIGHT; // assuming WIDTH > HEIGHT
	rf.fov = master->rt->camera->fov;
	rf.scale = tan(cos(rad(rf.fov * 0.5))); // why cos(rad)? see https://www.permadi.com/tutorial/raycast/rayc8.html, alternatively change z to a lower value (e.g. -10)
	rf.t_near = (double)INT_MAX;
	rf.t = (double)INT_MAX;
	rf.object_flag = EMPTY;
	rf.object_ptr = NULL;
	rf.clr = 0x000000FF;
	rf.origin = shift_origin(master->rt->camera->matrix);
	rf.cam_mat = master->rt->camera->matrix;
	rf.shadowray.direction = (t_vect3f){};
	rf.shadowray.origin = (t_vect3f){};
	rf.hit_normal = (t_vect3f){};
	rf.light_dir = (t_vect3f){};
	rf.light_ratio = 0;
	rf.light_dist = 0;
	rf.shadow_inter = (t_vect3f){};
	rf.inter_dist = 0;
	rf.light_intensity = master->options->light_intensity;
	return (rf);
}

void	update_ray_direction(t_rayfinder *rf, t_ray *ray, int x, int y)
{
	ray->origin.x = rf->origin.x;
	ray->origin.y = rf->origin.y;
	ray->origin.z = rf->origin.z;
	ray->direction.x = (2. * ((x + 0.5) / (double)WIDTH) - 1.) \
				* rf->ratio * rf->scale;
	ray->direction.y = (1. - 2. * ((y + 0.5) / (double)HEIGHT)) \
				* rf->scale;
	ray->direction.z = -1; // change to calibrate focal length, should be -1
	change_ray_direction(rf->cam_mat, &ray->direction, ray->direction);
}

void	find_rays(t_master *master)
{
	int		x;
	int		y;
	t_ray	**rays;
	t_rayfinder	rf;

	update_camera_matrix(master->rt->camera);
	rays = malloc(sizeof(t_ray *) * (WIDTH));
	rf = init_rayfinder(master);
	x = 0;
	y = 0;
	detect_camera_inside_objects(master->rt);
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		y = 0;
		while (y < HEIGHT)
		{
			reset_rayfinder(&rf);
			update_ray_direction(&rf, &rays[x][y], x, y);
			find_intersections(master, rays[x][y], &rf, PRIMARY);
			shade_nearest_object(rf.object_flag, rf.object_ptr, &rf, rays[x][y], master);
			mlx_put_pixel(master->img, x, y, rf.clr);
			y++;
		}
		free(rays[x]);
		x++;
	}
	free(rays);
}
