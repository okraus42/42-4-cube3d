/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:33:22 by plouda            #+#    #+#             */
/*   Updated: 2024/03/01 15:32:11 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

// axis = axis along which we're rotating
t_quat	get_rot_quat(t_vect3f axis, double angle)
{
	t_quat	rot;

	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = axis.x * sin(rad(angle) / 2);
	rot.q2 = axis.y * sin(rad(angle) / 2);
	rot.q3 = axis.z * sin(rad(angle) / 2);
	return (rot);
}

// axis = axis/point that we're rotating
t_quat	get_point_quat(t_vect3f axis)
{
	t_quat	point;

	point.q0 = 0;
	point.q1 = axis.x;
	point.q2 = axis.y;
	point.q3 = axis.z;
	return (point);
}

// works for rotation quaternion only, quat conjugate = inverse
t_quat	get_inverse_quat(t_quat quat)
{
	t_quat	inverse;

	inverse.q0 = quat.q0;
	inverse.q1 = quat.q1 * -1;
	inverse.q2 = quat.q2 * -1;
	inverse.q3 = quat.q3 * -1;
	return (inverse);
}

// quaternion multiplication is associative, but not commutative
t_quat	mult_quat(t_quat i, t_quat j)
{
	t_quat	res;

	res.q0 = i.q0 * j.q0 - i.q1 * j.q1 - i.q2 * j.q2 - i.q3 * j.q3;
	res.q1 = i.q0 * j.q1 + i.q1 * j.q0 - i.q2 * j.q3 + i.q3 * j.q2;
	res.q2 = i.q0 * j.q2 + i.q1 * j.q3 + i.q2 * j.q0 - i.q3 * j.q1;
	res.q3 = i.q0 * j.q3 - i.q1 * j.q2 + i.q2 * j.q1 + i.q3 * j.q0;
	return (res);
}

/*
res1.q0 = i.q0 * point.q0 - i.q1 * point.q1 - i.q2 * point.q2 - i.q3 * point.q3;
res1.q1 = i.q0 * point.q1 + i.q1 * point.q0 - i.q2 * point.q3 + i.q3 * point.q2;
res1.q2 = i.q0 * point.q2 + i.q1 * point.q3 + i.q2 * point.q0 - i.q3 * point.q1;
res1.q3 = i.q0 * point.q3 - i.q1 * point.q2 + i.q2 * point.q1 + i.q3 * point.q0;

res2.q0 = res1.q0 * r.q0 - res1.q1 * r.q1 - res1.q2 * r.q2 - res1.q3 * r.q3;
res2.q1 = res1.q0 * r.q1 + res1.q1 * r.q0 - res1.q2 * r.q3 + res1.q3 * r.q2;
res2.q2 = res1.q0 * r.q2 + res1.q1 * r.q3 + res1.q2 * r.q0 - res1.q3 * r.q1;
res2.q3 = res1.q0 * r.q3 - res1.q1 * r.q2 + res1.q2 * r.q1 + res1.q3 * r.q0;
*/

//	vector a = crossproduct(v1, v2);
//	q.xyz = a;
//	q.w = sqrt((v1.Length ^ 2) * (v2.Length ^ 2)) + dotproduct(v1, v2);

static double	get_quat_length(t_quat q)
{
	double	l;

	l = sqrt(pow(q.q0, 2) + pow(q.q1, 2) + pow(q.q2, 2) + pow(q.q3, 2));
	return (l);
}

void	normalize_quat(t_quat *q)
{
	double	l;

	l = get_quat_length(*q);
	q->q0 /= l;
	q->q1 /= l;
	q->q2 /= l;
	q->q3 /= l;
}

// static double	get_vector_length(t_vect3f v)
// {
// 	double	l;

// 	l = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
// 	return (l);
// }

t_quat	get_rotvect_quat(t_vect3f v1, t_vect3f v2)
{
	t_quat		q;
	t_vect3f	v;
	double		dot;

	v = cross_product(v1, v2);
	dot = dot_product(v1, v2);
	q.q1 = v.x;
	q.q2 = v.y;
	q.q3 = v.z;
	q.q0 = 1 + dot;
	normalize_quat(&q);
	if (q.q0 < 0.001 && q.q0 > -0.001)
	{
		q.q0 = 0;
		q.q1 = 0;
		q.q2 = 0;
		q.q3 = 1;
	}
	normalize_quat(&q);
	return (q);
}

void	rotate_vect(t_vect3f *vect, t_quat q)
{
	t_quat	i;
	t_quat	res;
	t_quat	point;
	t_quat	result;

	point = get_point_quat(*vect);
	i = get_inverse_quat(q);
	res = mult_quat(i, point);
	result = mult_quat(res, q);
	vect->x = result.q1;
	vect->y = result.q2;
	vect->z = result.q3;
}

t_quat	get_obj_quat(t_vect3f norm, t_vect3f up)
{
	t_quat		q;
	t_quat		first;
	t_quat		second;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	newup;

	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	if (norm.z < -0.999)
	{
		q.q0 = 0;
		q.q1 = 0;
		q.q2 = 1;
		q.q3 = 0;
		return (q);
	}
	first = get_rotvect_quat(norm, z);
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	normalize(&newup);
	rotate_vect(&newup, first);
	normalize(&newup);
	second = get_rotvect_quat(newup, y);
	q = mult_quat(first, second);
	normalize_quat(&q);
	return (q);
}

//sphere
t_quat	get_sphere_tan_quat(t_vect3f norm)
{
	t_quat		q;
	t_quat		first;
	t_quat		second;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	up;
	t_vect3f	newup;
	t_vect3f	tmp;
	t_vect3f	right;

	tmp = (t_vect3f){0, 1, 0};
	if (norm.x == 0. && norm.y == 1. && norm.z == 0.)
	{
		right = (t_vect3f){1, 0, 0};
		up = (t_vect3f){0, 0, -1};
	}
	else if (norm.x == 0. && norm.y == -1 && norm.z == 0.)
	{
		right = (t_vect3f){1, 0, 0};
		up = (t_vect3f){0, 0, 1};
	}
	else
	{
		right = cross_product(tmp, norm);
		normalize(&right);
		up = cross_product(norm, right);
		normalize(&up);
	}
	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	first = get_rotvect_quat(norm, z);
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	rotate_vect(&norm, first);
	rotate_vect(&up, first);
	normalize(&up);
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	second = get_rotvect_quat(newup, y);
	rotate_vect(&newup, second);
	rotate_vect(&norm, second);
	q = mult_quat(first, second);
	normalize_quat(&q);
	return (q);
}

//cylinder
t_quat	get_cylinder_tan_quat(t_vect3f norm)
{
	t_quat		q;
	t_quat		first;
	t_quat		second;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	up;
	t_vect3f	newup;

	up.x = 0.;
	up.y = 0.;
	up.z = 1.;
	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	first = get_rotvect_quat(norm, z);
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	rotate_vect(&norm, first);
	rotate_vect(&up, first);
	normalize(&up);
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	second = get_rotvect_quat(newup, y);
	rotate_vect(&newup, second);
	rotate_vect(&norm, second);
	q = mult_quat(first, second);
	normalize_quat(&q);
	return (q);
}

//cone to be done

t_quat	get_cone_tan_quat(t_vect3f norm, t_cone *cone, t_vect3f	i)
{
	t_quat		q;
	t_quat		first;
	t_quat		second;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	up;
	t_vect3f	newup;

	up.x = cone->pinnacle->coords[X] - i.x;
	up.y = cone->pinnacle->coords[Y] - i.y;
	up.z = cone->pinnacle->coords[Z] - i.z;
	normalize(&up);
	rotate_vect(&up, cone->q);
	normalize(&up);
	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	first = get_rotvect_quat(norm, z);
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	rotate_vect(&norm, first);
	rotate_vect(&up, first);
	up.z = 0;
	normalize(&up);
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	second = get_rotvect_quat(newup, y);
	rotate_vect(&newup, second);
	rotate_vect(&norm, second);
	q = mult_quat(first, second);
	normalize_quat(&q);
	return (q);
}
