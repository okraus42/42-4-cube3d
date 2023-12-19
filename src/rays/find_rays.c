/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/19 17:04:17 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	update_object_ref(t_rayfinder *rf, void *object, t_object flag)
{
	if (rf->t < rf->t_near)
	{
		rf->t_near = rf->t;
		rf->object_flag = flag;
		rf->object_ptr = object;
	}
}

void	find_intersections(t_master *master, t_ray ray, t_rayfinder *rf)
{
	int	i;

	i = 0;
	while (i < master->rt->n_spheres)
	{
		if (intersect_sphere(ray, master->rt->spheres[i], &rf->t))
			update_object_ref(rf, master->rt->spheres[i], SPHERE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		if (intersect_plane(ray, master->rt->planes[i], &rf->t, PLANE))
			update_object_ref(rf, master->rt->planes[i], PLANE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cylinders)
	{
		if (intersect_cylinder(ray, master->rt->cylinders[i], &rf->t))
			update_object_ref(rf, master->rt->cylinders[i], CYLINDER);
		if (intersect_disc(ray, master->rt->cylinders[i]->topcap, &rf->t))
			update_object_ref(rf, master->rt->cylinders[i]->topcap, DISC);
		if (intersect_disc(ray, master->rt->cylinders[i]->botcap, &rf->t))
			update_object_ref(rf, master->rt->cylinders[i]->botcap, DISC);
		i++;
	}
}

void	get_nearest_object(int flag, void *object_ptr, uint32_t *clr, t_rayfinder *rf, t_ray ray)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_disc		*disc;
	t_vect3f		hit_normal;
	t_vect3f		intersection;
	t_vect3f		view_dir;
	double			facing_ratio;
	double				r;
	double				g;
	double				b;

	sphere = NULL;
	plane = NULL;
	cylinder = NULL;
	disc = NULL;
	intersection.x = ray.origin.x + rf->t_near * ray.direction.x;
	intersection.y = ray.origin.y + rf->t_near * ray.direction.y;
	intersection.z = ray.origin.z + rf->t_near * ray.direction.z;
	if (flag == SPHERE)
	{
		sphere = (t_sphere *)object_ptr;
		hit_normal.x = intersection.x - sphere->coords[X];
		hit_normal.y = intersection.y - sphere->coords[Y];
		hit_normal.z = intersection.z - sphere->coords[Z];
		normalize(&hit_normal);
		view_dir = (t_vect3f){ray.direction.x * -1, ray.direction.y * -1, ray.direction.z * -1};
		facing_ratio = dot_product(hit_normal, view_dir);
		if (facing_ratio < 0)
			facing_ratio = 0;
		r = sphere->rgb[R] * facing_ratio;
		g = sphere->rgb[G] * facing_ratio;
		b = sphere->rgb[B] * facing_ratio;
		*clr = ((int)r << 24
				| (int)g << 16
				| (int)b << 8
				| 0xFF);
	}
	else if (flag == PLANE)
	{
		plane = (t_plane *)object_ptr;
/* 		hit_normal.x = intersection.x - plane->coords[X];
		hit_normal.y = intersection.y - plane->coords[Y];
		hit_normal.z = intersection.z - plane->coords[Z]; */
		hit_normal = *plane->normal;
		normalize(&hit_normal);
		view_dir = (t_vect3f){ray.direction.x * -1, ray.direction.y * -1, ray.direction.z * -1};
		facing_ratio = dot_product(hit_normal, view_dir);
		if (facing_ratio < 0)
			facing_ratio = 0;
		r = plane->rgb[R] * facing_ratio;
		g = plane->rgb[G] * facing_ratio;
		b = plane->rgb[B] * facing_ratio;
		*clr = ((int)r << 24
				| (int)g << 16
				| (int)b << 8
				| 0xFF);
		//*clr = (plane->rgb[0] << 24 | plane->rgb[1] << 16 | plane->rgb[2] << 8 | 0xFF);
	}
	else if (flag == CYLINDER)
	{
		cylinder = (t_cylinder *)object_ptr;
		*clr = (cylinder->rgb[0] << 24 | cylinder->rgb[1] << 16 | cylinder->rgb[2] << 8 | 0xFF);
	}
	else if (flag == DISC)
	{
		disc = (t_disc *)object_ptr;
		hit_normal = *disc->normal;
		normalize(&hit_normal);
		view_dir = (t_vect3f){ray.direction.x * -1, ray.direction.y * -1, ray.direction.z * -1};
		facing_ratio = dot_product(hit_normal, view_dir);
		if (facing_ratio < 0)
			facing_ratio = 0;
		r = disc->rgb[R] * facing_ratio;
		g = disc->rgb[G] * facing_ratio;
		b = disc->rgb[B] * facing_ratio;
		*clr = ((int)r << 24
				| (int)g << 16
				| (int)b << 8
				| 0xFF);
		//*clr = 0xff00ffff;
		(void)disc;
	}
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
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		y = 0;
		while (y < HEIGHT)
		{
			reset_rayfinder(&rf);
			update_ray_direction(&rf, &rays[x][y], x, y);
			find_intersections(master, rays[x][y], &rf);
			get_nearest_object(rf.object_flag, rf.object_ptr, &rf.clr, &rf, rays[x][y]);
			mlx_put_pixel(master->img, x, y, rf.clr);
			y++;
		}
		free(rays[x]);
		x++;
	}
	free(rays);
}
