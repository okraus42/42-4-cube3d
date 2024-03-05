/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:33:22 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:43:51 by plouda           ###   ########.fr       */
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