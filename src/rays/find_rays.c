/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/26 16:32:48 by plouda           ###   ########.fr       */
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

int	find_intersections(t_master *master, t_ray ray, t_rayfinder *rf)
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
	return (flag);
}

t_vect3f	get_hit_normal(t_rayfinder *rf, t_ray ray, t_vect3f intersection, t_cylinder cylinder)
{
	t_vect3f	hit_normal;
	t_vect3f	centered;
	double			m; // a scalar that determines the closest point on the axis to the hit point

	centered = subtract_center(ray.origin, cylinder.coords);
	m = (dot_product(ray.direction, *cylinder.normal) * rf->t_near) + dot_product(centered, *cylinder.normal);
	hit_normal.x = intersection.x - cylinder.coords[X] - cylinder.normal->x * m;
	hit_normal.y = intersection.y - cylinder.coords[Y] - cylinder.normal->y * m;
	hit_normal.z = intersection.z - cylinder.coords[Z] - cylinder.normal->z * m;
	normalize(&hit_normal);
	return (hit_normal);
}

double	point_distance(t_vect3f p1, t_vect3f p2)
{
	double	dist;

	dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
	return (dist);
}

void	get_nearest_object(int flag, void *object_ptr, uint32_t *clr, t_rayfinder *rf, t_ray ray, t_master *master)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_disc		*disc;
	t_vect3f	hit_normal;
	t_vect3f	intersection;
	t_vect3f	light_dir;
	t_vect3f	shadow_inter;
	t_ray		shadowray;
	double		light_ratio;
	double		r;
	double		g;
	double		b;
	int			rgb[3];
	double		shadow_bias;
	double		light_dist;
	double		inter_dist;

	sphere = NULL;
	plane = NULL;
	cylinder = NULL;
	disc = NULL;
	shadow_bias = 1e-4;
	intersection.x = ray.origin.x + rf->t_near * ray.direction.x;
	intersection.y = ray.origin.y + rf->t_near * ray.direction.y;
	intersection.z = ray.origin.z + rf->t_near * ray.direction.z;
	// reset t_near, but only if it's not cylinder (used to calculate hit norm)
	if (flag == SPHERE)
	{
		sphere = (t_sphere *)object_ptr;
		hit_normal.x = intersection.x - sphere->coords[X];
		hit_normal.y = intersection.y - sphere->coords[Y];
		hit_normal.z = intersection.z - sphere->coords[Z];
		normalize(&hit_normal);
		light_dir.x = master->rt->light->coords[X] - intersection.x;
		light_dir.y = master->rt->light->coords[Y] - intersection.y;
		light_dir.z = master->rt->light->coords[Z] - intersection.z;
		normalize(&light_dir);
		light_ratio = dot_product(hit_normal, light_dir);
		if (light_ratio < 0)
			light_ratio = 0;
		r = sphere->rgb[R] * light_ratio * master->rt->light->brightness;
		g = sphere->rgb[G] * light_ratio * master->rt->light->brightness;
		b = sphere->rgb[B] * light_ratio * master->rt->light->brightness;
		shadowray.direction = light_dir;
		shadowray.origin = add_vect3f(intersection, scale_vect3f(shadow_bias, hit_normal));
		light_dist = point_distance(shadowray.origin, array_to_vect(master->rt->light->coords));
		if (find_intersections(master, shadowray, rf))
		{
			shadow_inter.x = shadowray.origin.x + rf->t_near * shadowray.direction.x;
			shadow_inter.y = shadowray.origin.y + rf->t_near * shadowray.direction.y;
			shadow_inter.z = shadowray.origin.z + rf->t_near * shadowray.direction.z;
			inter_dist = point_distance(shadow_inter, shadowray.origin);
			if (inter_dist < light_dist)
			{
				rgb[R] = sphere->rgb_ambient[R];
				rgb[G] = sphere->rgb_ambient[G];
				rgb[B] = sphere->rgb_ambient[B];
			}
			else
			{
				rgb[R] = ft_max((int)r, sphere->rgb_ambient[R]);
				rgb[G] = ft_max((int)g, sphere->rgb_ambient[G]);
				rgb[B] = ft_max((int)b, sphere->rgb_ambient[B]);
			}
		}
		else
		{
			rgb[R] = ft_max((int)r, sphere->rgb_ambient[R]);
			rgb[G] = ft_max((int)g, sphere->rgb_ambient[G]);
			rgb[B] = ft_max((int)b, sphere->rgb_ambient[B]);
		}
		*clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	}
	else if (flag == PLANE)
	{
		plane = (t_plane *)object_ptr;
		hit_normal = *plane->normal;
		light_dir.x = master->rt->light->coords[X] - intersection.x;
		light_dir.y = master->rt->light->coords[Y] - intersection.y;
		light_dir.z = master->rt->light->coords[Z] - intersection.z;
		normalize(&light_dir);
		light_ratio = dot_product(hit_normal, light_dir);
		if (light_ratio < 0)
			light_ratio = 0;
		r = plane->rgb[R] * light_ratio * master->rt->light->brightness;
		g = plane->rgb[G] * light_ratio * master->rt->light->brightness;
		b = plane->rgb[B] * light_ratio * master->rt->light->brightness;
		shadowray.direction = light_dir;
		shadowray.origin = add_vect3f(intersection, scale_vect3f(shadow_bias, hit_normal));
		if (find_intersections(master, shadowray, rf))
		{
			rgb[R] = plane->rgb_ambient[R];
			rgb[G] = plane->rgb_ambient[G];
			rgb[B] = plane->rgb_ambient[B];
		}
		else
		{
			rgb[R] = ft_max((int)r, plane->rgb_ambient[R]);
			rgb[G] = ft_max((int)g, plane->rgb_ambient[G]);
			rgb[B] = ft_max((int)b, plane->rgb_ambient[B]);
		}
		*clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	}
	else if (flag == CYLINDER)
	{
		cylinder = (t_cylinder *)object_ptr;
		hit_normal = get_hit_normal(rf, ray, intersection, *cylinder);
		light_dir.x = master->rt->light->coords[X] - intersection.x;
		light_dir.y = master->rt->light->coords[Y] - intersection.y;
		light_dir.z = master->rt->light->coords[Z] - intersection.z;
		normalize(&light_dir);
		light_ratio = dot_product(hit_normal, light_dir);
		if (light_ratio < 0)
			light_ratio = 0;
		r = cylinder->rgb[R] * light_ratio * master->rt->light->brightness;
		g = cylinder->rgb[G] * light_ratio * master->rt->light->brightness;
		b = cylinder->rgb[B] * light_ratio * master->rt->light->brightness;
		shadowray.direction = light_dir;
		shadowray.origin = add_vect3f(intersection, scale_vect3f(shadow_bias, hit_normal));
		if (find_intersections(master, shadowray, rf))
		{
			rgb[R] = cylinder->rgb_ambient[R];
			rgb[G] = cylinder->rgb_ambient[G];
			rgb[B] = cylinder->rgb_ambient[B];
		}
		else
		{
			rgb[R] = ft_max((int)r, cylinder->rgb_ambient[R]);
			rgb[G] = ft_max((int)g, cylinder->rgb_ambient[G]);
			rgb[B] = ft_max((int)b, cylinder->rgb_ambient[B]);
		}
		*clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	}
	else if (flag == DISC)
	{
		disc = (t_disc *)object_ptr;
		hit_normal = *disc->normal;
		light_dir.x = master->rt->light->coords[X] - intersection.x;
		light_dir.y = master->rt->light->coords[Y] - intersection.y;
		light_dir.z = master->rt->light->coords[Z] - intersection.z;
		normalize(&light_dir);
		light_ratio = dot_product(hit_normal, light_dir);
		if (light_ratio < 0)
			light_ratio = 0;
		r = disc->rgb[R] * light_ratio * master->rt->light->brightness;
		g = disc->rgb[G] * light_ratio * master->rt->light->brightness;
		b = disc->rgb[B] * light_ratio * master->rt->light->brightness;
		shadowray.direction = light_dir;
		shadowray.origin = add_vect3f(intersection, scale_vect3f(shadow_bias, hit_normal));
		if (find_intersections(master, shadowray, rf))
		{
			rgb[R] = disc->rgb_ambient[R];
			rgb[G] = disc->rgb_ambient[G];
			rgb[B] = disc->rgb_ambient[B];
		}
		else
		{
			rgb[R] = ft_max((int)r, disc->rgb_ambient[R]);
			rgb[G] = ft_max((int)g, disc->rgb_ambient[G]);
			rgb[B] = ft_max((int)b, disc->rgb_ambient[B]);
		}
		*clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
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
			get_nearest_object(rf.object_flag, rf.object_ptr, &rf.clr, &rf, rays[x][y], master);
			mlx_put_pixel(master->img, x, y, rf.clr);
			y++;
		}
		free(rays[x]);
		x++;
	}
	free(rays);
}
