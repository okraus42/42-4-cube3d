/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:24:25 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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

void	define_shape(void *object, t_vect3f *pt, t_vect3f *normal, t_object f)
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
	double		dist1;
	double		dist2;
	double		plane1;
	double		plane2;
	t_vect3f	inter;

	inter.x = ray.origin.x + ray.direction.x * t;
	inter.y = ray.origin.y + ray.direction.y * t;
	inter.z = ray.origin.z + ray.direction.z * t;
	dist1 = (-1 * cap1->normal->x * cap1->coords[X]) \
			- (cap1->normal->y * cap1->coords[Y]) \
			- (cap1->normal->z * cap1->coords[Z]);
	dist2 = (-1 * cap2->normal->x * cap2->coords[X]) \
			- (cap2->normal->y * cap2->coords[Y]) \
			- (cap2->normal->z * cap2->coords[Z]);
	plane1 = (cap1->normal->x * inter.x) + (cap1->normal->y * inter.y) \
			+ (cap1->normal->z * inter.z) + dist1;
	plane2 = (cap2->normal->x * inter.x) + (cap2->normal->y * inter.y) \
			+ (cap2->normal->z * inter.z) + dist2;
	if (plane1 > 0 || plane2 > 0)
		return (0);
	else if (plane1 < 0 && plane2 < 0)
		return (1);
	else
		return (0);
}

/*
https://stackoverflow.com/questions/73866852/ray-cylinder-intersection-formula
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
	quad.b = 2 * (dot_product(dir, centered) \
			- (dot_product(dir, axis) * dot_product(centered, axis)));
	quad.c = dot_product(centered, centered) \
			- pow(dot_product(centered, axis), 2) \
			- pow(cylinder->diameter / 2, 2);
	return (solve_quad_cyl(t, quad, ray, cylinder));
}
