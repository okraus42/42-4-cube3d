/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:33:22 by plouda            #+#    #+#             */
/*   Updated: 2024/03/03 16:02:23 by okraus           ###   ########.fr       */
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
	t_qmath	qm;

	qm.z.x = 0.;
	qm.z.y = 0.;
	qm.z.z = 1.;
	if (norm.z < -0.999)
	{
		qm.q.q0 = 0;
		qm.q.q1 = 0;
		qm.q.q2 = 1;
		qm.q.q3 = 0;
		return (qm.q);
	}
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y.x = 0.;
	qm.y.y = 1.;
	qm.y.z = 0.;
	qm.newup.x = up.x;
	qm.newup.y = up.y;
	qm.newup.z = up.z;
	normalize(&qm.newup);
	rotate_vect(&qm.newup, qm.first);
	normalize(&qm.newup);
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}

//sphere
t_quat	get_sphere_tan_quat(t_vect3f norm)
{
	t_qmath	qm;

	qm.tmp = (t_vect3f){0, 1, 0};
	if (norm.x == 0. && norm.y == 1. && norm.z == 0.)
	{
		qm.right = (t_vect3f){1, 0, 0};
		qm.up = (t_vect3f){0, 0, -1};
	}
	else if (norm.x == 0. && norm.y == -1 && norm.z == 0.)
	{
		qm.right = (t_vect3f){1, 0, 0};
		qm.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		qm.right = cross_product(qm.tmp, norm);
		normalize(&(qm.right));
		qm.up = cross_product(norm, qm.right);
		normalize(&(qm.up));
	}
	qm.z.x = 0.;
	qm.z.y = 0.;
	qm.z.z = 1.;
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y.x = 0.;
	qm.y.y = 1.;
	qm.y.z = 0.;
	rotate_vect(&norm, qm.first);
	rotate_vect(&qm.up, qm.first);
	normalize(&qm.up);
	qm.newup.x = qm.up.x;
	qm.newup.y = qm.up.y;
	qm.newup.z = qm.up.z;
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	rotate_vect(&qm.newup, qm.second);
	rotate_vect(&norm, qm.second);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}

//cylinder
t_quat	get_cylinder_tan_quat(t_vect3f norm)
{
	t_qmath	qm;

	qm.up.x = 0.;
	qm.up.y = 0.;
	qm.up.z = 1.;
	qm.z.x = 0.;
	qm.z.y = 0.;
	qm.z.z = 1.;
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y.x = 0.;
	qm.y.y = 1.;
	qm.y.z = 0.;
	rotate_vect(&norm, qm.first);
	rotate_vect(&qm.up, qm.first);
	normalize(&qm.up);
	qm.newup.x = qm.up.x;
	qm.newup.y = qm.up.y;
	qm.newup.z = qm.up.z;
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	rotate_vect(&qm.newup, qm.second);
	rotate_vect(&norm, qm.second);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}

//cone to be done

t_quat	get_cone_tan_quat(t_vect3f norm, t_cone *cone, t_vect3f	i)
{
	t_qmath	qm;

	qm.up.x = cone->pinnacle->coords[X] - i.x;
	qm.up.y = cone->pinnacle->coords[Y] - i.y;
	qm.up.z = cone->pinnacle->coords[Z] - i.z;
	normalize(&qm.up);
	rotate_vect(&qm.up, cone->q);
	normalize(&qm.up);
	qm.z.x = 0.;
	qm.z.y = 0.;
	qm.z.z = 1.;
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y.x = 0.;
	qm.y.y = 1.;
	qm.y.z = 0.;
	rotate_vect(&norm, qm.first);
	rotate_vect(&qm.up, qm.first);
	qm.up.z = 0;
	normalize(&qm.up);
	qm.newup.x = qm.up.x;
	qm.newup.y = qm.up.y;
	qm.newup.z = qm.up.z;
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	rotate_vect(&qm.newup, qm.second);
	rotate_vect(&norm, qm.second);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}
