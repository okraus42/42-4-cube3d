/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:51:48 by okraus            #+#    #+#             */
/*   Updated: 2024/03/05 18:54:48 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	get_sphere_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	normalize(&p);
	uu = 1 - (0.5 + (atan2(p.z, p.x) / (6.28318530718)));
	vv = 1 - (0.5 + (asin(p.y) / 3.14159265358979));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	get_plane_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	uu = p.x / 16;
	vv = p.y / 16;
	*u = ((long long)(uu * scale[0] + 2147483648LL) % (int)scale[0]);
	*v = scale[1] - 1
		- ((long long)(vv * scale[1] + 2147483648LL) % (int)scale[1]);
}

void	get_cone_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = 1 - ((p.z + scale[2] / 2) / scale[2]);
	normalize(&p);
	uu = 0.5 + (atan2(p.y, p.x) / (6.28318530718));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	get_cylinder_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = 1 - ((p.z + scale[2] / 2) / scale[2]);
	normalize(&p);
	uu = (0.5 + (atan2(p.y, p.x) / (6.28318530718)));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	get_disc_uv(t_vect3f p, long long *u, long long *v, double scale[3])
{
	double		uu;
	double		vv;

	uu = (p.x + scale[2]) / (2 * scale[2]);
	vv = (p.y + scale[2]) / (2 * scale[2]);
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (long long)(uu * scale[0]);
	*v = scale[1] - (long long)(vv * scale[1]);
}
