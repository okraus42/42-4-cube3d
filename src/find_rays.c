/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/13 09:28:18 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

double	deg(double rad)
{
	double	deg;

	deg = rad * 180 / M_PI;
	return (deg);
}

double	rad(double deg)
{
	double	rad;

	rad = deg * M_PI / 180;
	return (rad);
}


void	ft_swapf(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

double	absf(double n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

// will fail for 0,0,0!
void	normalize(t_vect3f *vect, double px, double py, double pz)
{
	double	magnitude;

	magnitude = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));
	vect->x = px / magnitude;
	vect->y = py / magnitude;
	vect->z = pz / magnitude;
}

double	dot_product(t_vect3f vect1, t_vect3f vect2)
{
	double	res;

	res = vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
	return (res);
}

t_vect3f	cross_product(t_vect3f vect1, t_vect3f vect2)
{
	t_vect3f	res;

	res.x = vect1.y * vect2.z - vect1.z * vect2.y;
	res.y = vect1.z * vect2.x - vect1.x * vect2.z;
	res.z = vect1.x * vect2.y - vect1.y * vect2.x;
	return (res);
}

t_vect3f	subtract_center(t_vect3f vect1, double *coords)
{
	t_vect3f	res;

	res.x = vect1.x - coords[X];
	res.y = vect1.y - coords[Y];
	res.z = vect1.z - coords[Z];
	return (res);
}

t_vect3f	subtract_vect3f(t_vect3f vect1, t_vect3f vect2)
{
	t_vect3f	res;

	res.x = vect1.x - vect2.x;
	res.y = vect1.y - vect2.y;
	res.z = vect1.z - vect2.z;
	return (res);
}


int	test_sphere(t_ray ray, t_sphere *sphere, double *t)
{
	double	res[2];
	double	a;
	double	b;
	double	c;
	double	discr;
	double	q;
	t_vect3f	centered;

	centered = subtract_center(ray.origin, sphere->coords);
	a = dot_product(ray.direction, ray.direction);
	b = 2 * dot_product(ray.direction, centered);
	c = dot_product(centered, centered) - pow(sphere->diameter / 2, 2);
	discr = pow(b, 2) - 4 * a * c;
	if (discr < 0)
		return (0);
	else if (discr == 0)
	{
		res[0] = - 0.5 * b / a;
		res[1] = res[0];
		*t = res[0];
		return (1);
	}
	else
	{
		if (b > 0)
			q = -0.5 * (b + sqrt(discr));
		else
			q = -0.5 * (b - sqrt(discr));
		res[0] = q / a;
		res[1] = c / q;
		if (res[0] > res[1])
			ft_swapf(&res[0], &res[1]);
		if (res[0] < 0)
		{
			res[0] = res[1];
			if (res[0] < 0)
				return (0);
		}
		*t = res[0];
		return (1);
	}
}

int	test_plane(t_ray ray, t_plane *plane, double *t)
{
	double	denom;
	t_vect3f	point;
	t_vect3f	normal;
	t_vect3f	diff;

	point.x = plane->coords[X];
	point.y = plane->coords[Y];
	point.z = plane->coords[Z];
	normal.x = plane->normal->x;
	normal.y = plane->normal->y;
	normal.z = plane->normal->z;
	denom = dot_product(normal, ray.direction);
	if (denom > 1e-6) // means they're nearly or completely parallel
	{
		diff = subtract_vect3f(point, ray.origin);
		*t = dot_product(diff, normal) / denom;
		//printf("aaaa");

		return (*t >= 0);
	}
	if (denom < -1e-6) // means they're nearly or completely parallel
	{
		diff = subtract_vect3f(point, ray.origin);
		*t = dot_product(diff, normal) / denom;
		//*t = absf(*t);
		return (*t >= 0);
	}
	return (0);
}

// literally the same as test_plane, needs merging
int	test_disc_plane(t_ray ray, t_disc *plane, double *t)
{
	double	denom;
	t_vect3f	point;
	t_vect3f	normal;
	t_vect3f	diff;

	point.x = plane->coords[X];
	point.y = plane->coords[Y];
	point.z = plane->coords[Z];
	normal.x = plane->normal->x;
	normal.y = plane->normal->y;
	normal.z = plane->normal->z;
	denom = dot_product(normal, ray.direction);
	if (denom > 1e-6) // means they're nearly or completely parallel
	{
		diff = subtract_vect3f(point, ray.origin);
		*t = dot_product(diff, normal) / denom;
		//printf("aaaa");

		return (*t >= 0);
	}
	if (denom < -1e-6) // means they're nearly or completely parallel
	{
		diff = subtract_vect3f(point, ray.origin);
		*t = dot_product(diff, normal) / denom;
		//*t = absf(*t);
		return (*t >= 0);
	}
	return (0);
}

// l0 - origin, l - direction, p - intersect from ray
int	test_disc(t_ray ray, t_disc *disc, double *t)
{
	t_vect3f	center; //p0
	t_vect3f	intersect;
	t_vect3f	v;
	double	distance2;

	//*t = 0;
	center.x = disc->coords[X];
	center.y = disc->coords[Y];
	center.z = disc->coords[Z];
	if (test_disc_plane(ray, disc, t))
	{
		intersect.x = ray.origin.x + ray.direction.x * *t;
		intersect.y = ray.origin.y + ray.direction.y * *t;
		intersect.z = ray.origin.z + ray.direction.z * *t;
		v.x = intersect.x - center.x;
		v.y = intersect.y - center.y;
		v.z = intersect.z - center.z;
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
	dist1 = (-1 * cap1->normal->x * cap1->coords[X]) - (cap1->normal->y * cap1->coords[Y]) - (cap1->normal->z * cap1->coords[Z]);
	dist2 = (-1 * cap2->normal->x * cap2->coords[X]) - (cap2->normal->y * cap2->coords[Y]) - (cap2->normal->z * cap2->coords[Z]);
	plane1 = (cap1->normal->x * inter.x) + (cap1->normal->y * inter.y) + (cap1->normal->z * inter.z) + dist1;
	plane2 =(cap2->normal->x * inter.x) + (cap2->normal->y * inter.y) + (cap2->normal->z * inter.z) + dist2;
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
int	test_cylinder(t_ray ray, t_cylinder *cylinder, double *t)
{
	double	res[2];
	double	a;
	double	b;
	double	c;
	double	discr;
	double	q;
	t_vect3f	centered;
	t_vect3f	axis;

	axis.x = cylinder->normal->x;
	axis.y = cylinder->normal->y;
	axis.z = cylinder->normal->z;
	centered = subtract_center(ray.origin, cylinder->coords);
	a = dot_product(ray.direction, ray.direction) - pow(dot_product(ray.direction, axis), 2);
	b = 2 * (dot_product(ray.direction, centered) - (dot_product(ray.direction, axis) * dot_product(centered, axis)));
	c = dot_product(centered, centered) - pow(dot_product(centered, axis), 2) - pow(cylinder->diameter / 2, 2);
	discr = pow(b, 2) - 4 * a * c;
	if (discr < 0)
		return (0);
	else if (discr == 0)
	{
		res[0] = - 0.5 * b / a;
		res[1] = res[0];
		if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[0]))
		{
			*t = res[0];
			return (1);
		}
		else
			return (0);
	}
	else
	{
		if (b > 0)
			q = -0.5 * (b + sqrt(discr));
		else
			q = -0.5 * (b - sqrt(discr));
		res[0] = q / a;
		res[1] = c / q;
		if (res[0] > res[1])
			ft_swapf(&res[0], &res[1]);
		if (res[0] < 0)
		{
			res[0] = res[1];
			if (res[0] < 0)
				return (0);
		}
		if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[0]))
		{
			*t = res[0];
			return (1);
		}
		if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[1]))
		{
			*t = res[1];
			return (1);
		}
		else
			return (0);
	}
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
	double	px;
	double	py;
	double	pz;
	int		fov;
	double	t_near;
	double	t;
	int		object_flag;
	uint32_t	clr;
	void	*object_ptr;
	t_ray	**rays;
	t_vect3f	origin;
	//double	**camera_matrix;
	t_vect3f	direction;

	rays = malloc(sizeof(t_ray *) * (WIDTH));
	x = 0;
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		x++;
	}
	t_near = (double)INT_MAX;
	t = (double)INT_MAX;
	i = 0;
	x = 0;
	y = 0;
	fov = master->rt->camera->fov;
	ratio = (double)WIDTH / (double)HEIGHT; // assuming WIDTH > HEIGHT
	//camera_matrix = set_camera(master->rt->camera); // create camera matrix
	update_camera_matrix(master->rt->camera);
	origin = shift_origin(master->rt->camera->matrix); // translate camera position
	while (x < WIDTH)
	{
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
			// why cos(rad)? see https://www.permadi.com/tutorial/raycast/rayc8.html, alternatively change z to a lower value (e.g. -10)
			px = (2. * ((x + 0.5) / (double)WIDTH) - 1.) * ratio * tan(cos(rad(fov * 0.5)));
			py = (1. - 2. * ((y + 0.5) / (double)HEIGHT)) * tan(cos(rad(fov * 0.5)));
			pz = -1; // change to calibrate focal length, should be -1
			direction.x = px;
			direction.y = py;
			direction.z = pz;
			change_ray_direction(master->rt->camera->matrix, &rays[x][y].direction, direction); // change direction based on camera position and its direction
			i = 0;
			while (i < master->rt->n_spheres)
			{
				if (test_sphere(rays[x][y], master->rt->spheres[i], &t))
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
				if (test_plane(rays[x][y], master->rt->planes[i], &t))
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
				if (test_cylinder(rays[x][y], master->rt->cylinders[i], &t))
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
				if (test_disc(rays[x][y], master->rt->cylinders[i]->topcap, &t))
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
				if (test_disc(rays[x][y], master->rt->cylinders[i]->botcap, &t))
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
