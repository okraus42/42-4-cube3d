/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:58:02 by okraus            #+#    #+#             */
/*   Updated: 2024/03/13 17:28:10 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

t_vect3f	get_cylinder_hit_normal(t_vect3f intersection, t_cylinder cylinder)
{
	double		t;
	t_vect3f	normal;

	t = dot_product(*cylinder.normal, subtract_vect3f(intersection,
				array_to_vect(cylinder.botcap->coords)));
	normal = subtract_vect3f(intersection, scale_vect3f(t, *cylinder.normal));
	normal = subtract_vect3f(normal, array_to_vect(cylinder.botcap->coords));
	normalize(&normal);
	return (normal);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double		dist;
	t_vect3f	a;
	double		d;
	t_vect3f	normal;

	dist = point_distance(intersection, array_to_vect(cone.pinnacle->coords));
	d = dist * cone.dist_term;
	a.x = cone.pinnacle->coords[X] + cone.axis->x * d;
	a.y = cone.pinnacle->coords[Y] + cone.axis->y * d;
	a.z = cone.pinnacle->coords[Z] + cone.axis->z * d;
	normal = subtract_vect3f(intersection, a);
	normalize(&normal);
	return (normal);
}

void	set_ambient_intensity(t_shader *shader,
	int *ambient_rgb, double ambient_ratio)
{
	shader->illumination[R] = ambient_rgb[R] / 255.
		* ambient_ratio * shader->rgb_object[R] / 255.;
	shader->illumination[G] = ambient_rgb[G] / 255.
		* ambient_ratio * shader->rgb_object[G] / 255.;
	shader->illumination[B] = ambient_rgb[B] / 255.
		* ambient_ratio * shader->rgb_object[B] / 255.;
}

t_vect3f	get_new_normal(int x, int y, int z)
{
	t_vect3f	newnorm;

	printf("%i %i %i\n", x, y, z);
	x -= 128;
	y -= 128;
	z -= 128;
	if (z < 1)
		z = 1;
	newnorm.x = x;
	newnorm.y = y;
	newnorm.z = z;
	printf("newnorm %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	normalize(&newnorm);
	printf("newnorm %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	return (newnorm);
}
