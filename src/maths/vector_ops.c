/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:04:07 by plouda            #+#    #+#             */
/*   Updated: 2023/12/21 12:55:34 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

// will fail for 0,0,0!
t_vect3f	get_normal(double px, double py, double pz)
{
	t_vect3f	normal;
	double		magnitude;

	magnitude = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));
	normal.x = px / magnitude;
	normal.y = py / magnitude;
	normal.z = pz / magnitude;
	return (normal);
}

// will fail for 0,0,0!
void	normalize(t_vect3f *vect)
{
	double	magnitude;

	magnitude = sqrt(pow(vect->x, 2) + pow(vect->y, 2) + pow(vect->z, 2));
	vect->x = vect->x / magnitude;
	vect->y = vect->y / magnitude;
	vect->z = vect->z / magnitude;
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

t_vect3f	add_vect3f(t_vect3f vect1, t_vect3f vect2)
{
	t_vect3f	res;

	res.x = vect1.x + vect2.x;
	res.y = vect1.y + vect2.y;
	res.z = vect1.z + vect2.z;
	return (res);
}

t_vect3f	scale_vect3f(double scale, t_vect3f vect)
{
	t_vect3f	res;

	res.x = scale * vect.x;
	res.y = scale * vect.y;
	res.z = scale * vect.z;
	return (res);
}
