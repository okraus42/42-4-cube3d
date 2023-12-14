/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 17:14:38 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	intersect_sphere(t_ray ray, t_sphere *sphere, double *t)
{
	t_quadterms	quad;
	t_vect3f	centered;

	centered = subtract_center(ray.origin, sphere->coords);
	quad.a = dot_product(ray.direction, ray.direction);
	quad.b = 2 * dot_product(ray.direction, centered);
	quad.c = dot_product(centered, centered) - pow(sphere->diameter / 2, 2);
	return (solve_quad(t, quad));
}

static void	define_shape(void *object, t_vect3f *pt, t_vect3f *normal, t_object f)
{
	t_disc		*disc;
	t_plane		*plane;

	disc = NULL;
	plane = NULL;
	if (f == DISC)
	{
		disc = (t_disc *)object;
		*pt = array_to_vect(disc->coords);
		*normal = *disc->normal;
	}
	else
	{
		plane = (t_plane *)object;
		*pt = array_to_vect(plane->coords);
		*normal = *plane->normal;
	}
}

int	intersect_plane(t_ray ray, void *object, double *t, t_object flag)
{
	double		denom;
	t_vect3f	point;
	t_vect3f	normal;
	t_vect3f	diff;

	point = (t_vect3f){};
	normal = (t_vect3f){};
	diff = (t_vect3f){};
	define_shape(object, &point, &normal, flag);
	denom = dot_product(normal, ray.direction);
	if (denom < 1e-6 && denom > -1e-6) // nearly or completely parallel
		return (0);
	else
	{
		diff = subtract_vect3f(point, ray.origin);
		*t = dot_product(diff, normal) / denom;
		return (*t >= 0);
	}
}

// l0 - origin, l - direction, p - intersect from ray
int	intersect_disc(t_ray ray, t_disc *disc, double *t)
{
	t_vect3f	center;
	t_vect3f	intersect;
	t_vect3f	v;
	double		distance2;

	intersect = (t_vect3f){};
	v = (t_vect3f){};
	center = array_to_vect(disc->coords);
	if (intersect_plane(ray, disc, t, DISC))
	{
		intersect.x = ray.origin.x + ray.direction.x * *t;
		intersect.y = ray.origin.y + ray.direction.y * *t;
		intersect.z = ray.origin.z + ray.direction.z * *t;
		v = subtract_vect3f(intersect, center);
		distance2 = dot_product(v, v);
		return (distance2 <= pow(disc->radius, 2));
	}
	return (0);
}

// d=-ax_0-by_0-cz_0. 
int	is_between_caps(t_disc	*cap1, t_disc *cap2, t_ray ray, double t)
{
	double	dist1;
	double	dist2;
	double	plane1;
	double	plane2;
	t_vect3f	inter;

	inter.x = ray.origin.x + ray.direction.x * t;
	inter.y = ray.origin.y + ray.direction.y * t;
	inter.z = ray.origin.z + ray.direction.z * t;
	dist1 = (-1 * cap1->normal->x * cap1->coords[X])\
			- (cap1->normal->y * cap1->coords[Y])\
			- (cap1->normal->z * cap1->coords[Z]);
	dist2 = (-1 * cap2->normal->x * cap2->coords[X])\
			- (cap2->normal->y * cap2->coords[Y])\
			- (cap2->normal->z * cap2->coords[Z]);
	plane1 = (cap1->normal->x * inter.x) + (cap1->normal->y * inter.y)\
			+ (cap1->normal->z * inter.z) + dist1;
	plane2 = (cap2->normal->x * inter.x) + (cap2->normal->y * inter.y)\
			+ (cap2->normal->z * inter.z) + dist2;
	if (plane1 > 0 || plane2 > 0)
		return (0);
	else if (plane1 < 0 && plane2 < 0)
		return (1);
	else
		return (0);
}

// https://stackoverflow.com/questions/73866852/ray-cylinder-intersection-formula
/*
D - ray direction
V - axis (normalized)
X - ray.origin - cylinder.center
r - radius

a = D|D - (D|V)^2

c = X|X - (X|V)^2 - r^2

b = 2 * (D-V*(D|V))|(X-V*(X|V)) =
 = 2 * (D|X - D|V*(X|V) - X|V*(D|V) + (D|V)*(X|V)) =
 = 2 * (D|X - (D|V)*(X|V))
*/
int	intersect_cylinder(t_ray ray, t_cylinder *cylinder, double *t)
{
	t_quadterms	quad;
	t_vect3f	centered;
	t_vect3f	axis;
	t_vect3f	dir;

	dir = ray.direction;
	axis = *cylinder->normal;
	centered = subtract_center(ray.origin, cylinder->coords);
	quad.a = dot_product(dir, dir) - pow(dot_product(dir, axis), 2);
	quad.b = 2 * (dot_product(dir, centered)\
			- (dot_product(dir, axis) * dot_product(centered, axis)));
	quad.c = dot_product(centered, centered)\
			- pow(dot_product(centered, axis), 2)\
			- pow(cylinder->diameter / 2, 2);
	return (solve_quad_cyl(t, quad, ray, cylinder));
}

void	get_nearest_object(int flag, void *object_ptr, uint32_t *clr)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_disc		*disc;

	sphere = NULL;
	plane = NULL;
	cylinder = NULL;
	disc = NULL;
	if (flag == SPHERE)
	{
		sphere = (t_sphere *)object_ptr;
		*clr = (sphere->rgb[0] << 24 | sphere->rgb[1] << 16 | sphere->rgb[2] << 8 | 0xFF);
	}
	else if (flag == PLANE)
	{
		plane = (t_plane *)object_ptr;
		*clr = (plane->rgb[0] << 24 | plane->rgb[1] << 16 | plane->rgb[2] << 8 | 0xFF);
	}
	else if (flag == CYLINDER)
	{
		cylinder = (t_cylinder *)object_ptr;
		*clr = (cylinder->rgb[0] << 24 | cylinder->rgb[1] << 16 | cylinder->rgb[2] << 8 | 0xFF);
	}
	else if (flag == DISC)
	{
		disc = (t_disc *)object_ptr;
		*clr = 0xff00ffff;
		(void)disc;
	}
}

void	find_rays(t_master *master)
{
	int		i;
	int		x;
	int		y;
	double	ratio;
	int		fov;
	double	t_near;
	double	t;
	int		object_flag;
	uint32_t	clr;
	void	*object_ptr;
	t_ray	**rays;
	t_vect3f	origin;
	t_vect3f	direction;
	double	scale;

	rays = malloc(sizeof(t_ray *) * (WIDTH));
	t_near = (double)INT_MAX;
	t = (double)INT_MAX;
	i = 0;
	x = 0;
	y = 0;
	fov = master->rt->camera->fov;
	ratio = (double)WIDTH / (double)HEIGHT; // assuming WIDTH > HEIGHT
	update_camera_matrix(master->rt->camera);
	origin = shift_origin(master->rt->camera->matrix); // translate camera position
	// why cos(rad)? see https://www.permadi.com/tutorial/raycast/rayc8.html, alternatively change z to a lower value (e.g. -10)
	scale = tan(cos(rad(fov * 0.5)));
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		y = 0;
		while (y < HEIGHT)
		{
			// will cause conditional jumps for now
			object_flag = EMPTY;
			object_ptr = NULL;
			clr = 0x000000FF;
			t_near = (double)INT_MAX;
			rays[x][y].origin.x = origin.x;
			rays[x][y].origin.y = origin.y;
			rays[x][y].origin.z = origin.z;
			direction.x = (2. * ((x + 0.5) / (double)WIDTH) - 1.) * ratio * scale;
			direction.y = (1. - 2. * ((y + 0.5) / (double)HEIGHT)) * scale;
			direction.z = -1; // change to calibrate focal length, should be -1
			change_ray_direction(master->rt->camera->matrix, &rays[x][y].direction, direction); // change direction based on camera position and its direction
			i = 0;
			while (i < master->rt->n_spheres)
			{
				if (intersect_sphere(rays[x][y], master->rt->spheres[i], &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = SPHERE;
						object_ptr = master->rt->spheres[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_planes)
			{
				if (intersect_plane(rays[x][y], master->rt->planes[i], &t, PLANE))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = PLANE;
						object_ptr = master->rt->planes[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_cylinder(rays[x][y], master->rt->cylinders[i], &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = CYLINDER;
						object_ptr = master->rt->cylinders[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_disc(rays[x][y], master->rt->cylinders[i]->topcap, &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = DISC;
						object_ptr = master->rt->cylinders[i]->topcap;
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_disc(rays[x][y], master->rt->cylinders[i]->botcap, &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = DISC;
						object_ptr = master->rt->cylinders[i]->botcap;
					}
				}
				i++;
			}
			get_nearest_object(object_flag, object_ptr, &clr);
			mlx_put_pixel(master->img, x, y, clr);
			y++;
		}
		free(rays[x]);
		x++;
	}
	free(rays);
}
