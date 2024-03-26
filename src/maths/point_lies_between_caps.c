/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_lies_between_caps.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:28:27 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:29:16 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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
	dist1 = (-1 * cap1->normal->x * cap1->coords[X])
		- (cap1->normal->y * cap1->coords[Y])
		- (cap1->normal->z * cap1->coords[Z]);
	dist2 = (-1 * cap2->normal->x * cap2->coords[X])
		- (cap2->normal->y * cap2->coords[Y])
		- (cap2->normal->z * cap2->coords[Z]);
	plane1 = (cap1->normal->x * inter.x) + (cap1->normal->y * inter.y)
		+ (cap1->normal->z * inter.z) + dist1;
	plane2 = (cap2->normal->x * inter.x) + (cap2->normal->y * inter.y)
		+ (cap2->normal->z * inter.z) + dist2;
	if (plane1 > PRECISION || plane2 > PRECISION)
		return (0);
	else if (plane1 < -PRECISION && plane2 < -PRECISION)
		return (1);
	else
		return (0);
}
