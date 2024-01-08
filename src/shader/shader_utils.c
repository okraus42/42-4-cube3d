/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:46:40 by plouda            #+#    #+#             */
/*   Updated: 2024/01/08 15:03:20 by plouda           ###   ########.fr       */
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

#define MAX(a,b) (((a) > (b)) * (a) + ((b) >= (a)) * (b))

uint32_t	ft_max_clr(uint32_t a, uint32_t b)
{
	uint32_t	c;

	c = MAX(a & 0xFF000000, b & 0xFF000000) | MAX(a & 0xFF0000, b & 0xFF0000)
		| MAX(a & 0xFF00, b & 0xFF00) | 0xFF;
	// if (a >= b)
	// 	return (a);
	// else
	// 	return (b);
	return (c);
}

void	clamp(double min, double max, double *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
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

void	trace_shadow(t_master *m, t_rayfinder *rf, uint32_t amb, uint32_t light)
{
	if (find_intersections(m, rf->shadowray, rf, SHADOW))
	{
		rf->shadow_inter = get_intersection(rf->shadowray.origin, \
		rf->shadowray.direction, rf->t_near);
		rf->inter_dist = point_distance(rf->shadow_inter, rf->shadowray.origin);
		if (rf->inter_dist < rf->light_dist)
			rf->clr = amb;
		else
			rf->clr = ft_max_clr(amb, light);
	}
	else
		rf->clr = ft_max_clr(amb, light);
}
