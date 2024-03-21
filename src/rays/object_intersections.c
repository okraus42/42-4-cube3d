/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_intersections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:24:25 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:32:22 by plouda           ###   ########.fr       */
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
	quad.c = dot_product(centered, centered) - pow(sphere->radius, 2);
	return (solve_quad_sphere(t, quad));
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
	disc_or_plane(object, &point, &normal, flag);
	denom = dot_product(normal, ray.direction);
	if (denom < PRECISION && denom > -PRECISION)
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
	quad.b = 2 * (dot_product(dir, centered)
			- (dot_product(dir, axis) * dot_product(centered, axis)));
	quad.c = dot_product(centered, centered)
		- pow(dot_product(centered, axis), 2)
		- pow(cylinder->radius, 2);
	return (solve_quad_cyl(t, quad, ray, cylinder));
}

int	intersect_cone(t_ray ray, t_cone *cone, double *t)
{
	t_quadterms	quad;
	t_vect3f	centered;
	t_vect3f	dir;
	t_vect3f	centered_cos;

	dir = ray.direction;
	centered = subtract_center(ray.origin, cone->pinnacle->coords);
	cone->half_angle = atan2(cone->radius, cone->height);
	centered_cos.x = centered.x * pow(cos(cone->half_angle), 2);
	centered_cos.y = centered.y * pow(cos(cone->half_angle), 2);
	centered_cos.z = centered.z * pow(cos(cone->half_angle), 2);
	quad.a = pow(dot_product(dir, *cone->axis), 2)
		- pow(cos(cone->half_angle), 2);
	quad.b = 2 * (dot_product(dir, *cone->axis)
			* dot_product(centered, *cone->axis)
			- dot_product(dir, centered_cos));
	quad.c = pow(dot_product(centered, *cone->axis), 2)
		- dot_product(centered, centered_cos);
	return (solve_quad_cone(t, quad, ray, cone));
}
