/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:44:58 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:45:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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

t_vect3f	invert_vect3f(t_vect3f vect)
{
	t_vect3f	inv;

	inv.x = vect.x * -1;
	inv.y = vect.y * -1;
	inv.z = vect.z * -1;
	return (inv);
}
