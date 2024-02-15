/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:46:40 by plouda            #+#    #+#             */
/*   Updated: 2024/02/15 11:55:32 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

double	point_distance(t_vect3f p1, t_vect3f p2)
{
	double	dist;

	dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
	return (dist);
}

void	get_clr_components(int *light, int *rgb, double ratio, double bright)
{
	light[R] = (double)rgb[R] * ratio * bright;
	light[G] = (double)rgb[G] * ratio * bright;
	light[B] = (double)rgb[B] * ratio * bright;
}

uint32_t	get_clr_int(int *rgb)
{
	uint32_t	clr;

	clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	return (clr);
}

uint32_t	ft_max_clr(uint32_t a, uint32_t b)
{
	uint32_t	c;

	c = MAX(a & 0xFF000000, b & 0xFF000000) | MAX(a & 0xFF0000, b & 0xFF0000)
		| MAX(a & 0xFF00, b & 0xFF00) | 0xFF;
	return (c);
}

void	clamp(int min, int max, int *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

void	clampf(double min, double max, double *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

t_vect3f	get_cylinder_hit_normal(t_rayfinder *rf, t_ray ray, t_vect3f intersection, t_cylinder cylinder)
{

	double t;
	t_vect3f	normal;

	(void)rf;
	(void)ray;
	t = dot_product(*cylinder.normal, subtract_vect3f(intersection, array_to_vect(cylinder.botcap->coords)));
	normal = subtract_vect3f(intersection, scale_vect3f(t, *cylinder.normal));
	normal = subtract_vect3f(normal, array_to_vect(cylinder.botcap->coords));
	normalize(&normal);
	return (normal);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double	dist;
	double	half_angle;
	t_vect3f	axis;
	t_vect3f	a;
	double		d;
	t_vect3f	normal;

	axis = invert_vect3f(*cone.normal);
	half_angle = atan2(cone.diameter / 2, cone.height);
	dist = point_distance(intersection, array_to_vect(cone.pinnacle->coords));
	d = dist * sqrt(1 + pow(half_angle, 2));
	a.x = cone.pinnacle->coords[X] + axis.x * d;
	a.y = cone.pinnacle->coords[Y] + axis.y * d;
	a.z = cone.pinnacle->coords[Z] + axis.z * d;
	normal = subtract_vect3f(intersection, a);
	normalize(&normal);
	return (normal);
}

void	set_ambient_intensity(t_shader *shader, int *ambient_rgb, double ambient_ratio, int *object_rgb)
{
	shader->illumination[R] = ambient_rgb[R] / 255. * ambient_ratio * object_rgb[R] / 255.;
	shader->illumination[G] = ambient_rgb[G] / 255. * ambient_ratio * object_rgb[G] / 255.;
	shader->illumination[B] = ambient_rgb[B] / 255. * ambient_ratio * object_rgb[B] / 255.;
}
