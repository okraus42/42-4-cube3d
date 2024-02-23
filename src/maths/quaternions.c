/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:33:22 by plouda            #+#    #+#             */
/*   Updated: 2024/02/23 10:05:22 by plouda           ###   ########.fr       */
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

/* res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0; */

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
	//printf("dot %f", dot);
	//printf("cross %f %f %f\n", v.x, v.y, v.z);
	//q = get_point_quat(v);
	q.q1 = v.x;
	q.q2 = v.y;
	q.q3 = v.z;
	q.q0 = 1 + dot;
	if (q.q0 < 0.00001 && q.q0 > -0.00001)
	{
		q.q1 +=0.01;
		q.q2 +=0.01;
		q.q3 +=0.01;
	}
	normalize_quat(&q);
	// printf("nq %f %f %f %f\n", q.q0, q.q1, q.q2, q.q3);
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
	//t_quat		upq;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	newup;
	//t_vect3f	newnorm;

	// rotate norm to z 0,0,1
	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	first = get_rotvect_quat(norm, z);
	// rotate up to y 0,1,0
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	//upq = get_point_quat(up);
	//rotate newup by first quat
	// newnorm.x = norm.x;
	// newnorm.y = norm.y;
	// newnorm.z = norm.z;
	// printf("first %f %f %f %f\n", first.q0, first.q1, first.q2, first.q3);
	// printf("NN0 %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	// normalize(&newnorm);
	// printf("NN1 %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	// rotate_vect(&newnorm, first);
	// printf("NN2 %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	// normalize(&newnorm);
	// printf("NN3 %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	normalize(&newup);
	// printf("NU1 %f %f %f\n", newup.x, newup.y, newup.z);
	rotate_vect(&newup, first);
	normalize(&newup);
	// printf("NU2 %f %f %f\n", newup.x, newup.y, newup.z);
	second = get_rotvect_quat(newup, y);
	// printf("second %f %f %f %f\n", second.q0, second.q1, second.q2, second.q3);
	// rotate_vect(&newup, second);
	// normalize(&newup);
	// rotate_vect(&newnorm, second);
	// normalize(&newnorm);
	// printf("NN4 %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	// printf("NU3 %f %f %f\n", newup.x, newup.y, newup.z);
	//calculate the whole quaternion
	
	q = mult_quat(first, second);
	//printf("N %f %f %f\n", norm.x, norm.y, norm.z);
	//printf("NN %f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	// printf("U %f %f %f\n", up.x, up.y, up.z);
	// printf("NU %f %f %f\n", newup.x, newup.y, newup.z);
	// printf("Q1 %f %f %f %f\n", q.q0, q.q1, q.q2, q.q3);
	normalize_quat(&q);
	// printf("Q2 %f %f %f %f\n", q.q0, q.q1, q.q2, q.q3);
	return (q);
}

t_quat	get_tan_quat(t_vect3f norm)
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
	// rotate norm to z 0,0,1
	z.x = 0.;
	z.y = 0.;
	z.z = 1.;
	//printf("qnorm0 %f %f %f\n", norm.x, norm.y, norm.z);
	first = get_rotvect_quat(norm, z);
	// rotate up to y 0,1,0
	y.x = 0.;
	y.y = 1.;
	y.z = 0.;
	// newup.x = up.x;
	// newup.y = up.y;
	// newup.z = up.z;
	rotate_vect(&norm, first);
	//printf("qnorm1 %f %f %f\n", norm.x, norm.y, norm.z);
	rotate_vect(&up, first);
	normalize(&up);
	//printf("qup %f %f %f\n", up.x, up.y, up.z);
	newup.x = up.x;
	newup.y = up.y;
	newup.z = up.z;
	second = get_rotvect_quat(newup, y);
	rotate_vect(&newup, second);
	//printf("qnewup %f %f %f\n", newup.x, newup.y, newup.z);
	rotate_vect(&norm, second);
	//printf("qnorm2 %f %f %f\n", norm.x, norm.y, norm.z);
	q = mult_quat(first, second);
	normalize_quat(&q);
	return (q);
}
