/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:43:08 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:43:19 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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

	qm.z = (t_vect3f){0., 0., 1.};
	if (norm.z < -0.999)
	{
		qm.q = (t_quat){0., 0., 1., 0.};
		return (qm.q);
	}
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y = (t_vect3f){0., 1., 0.};
	qm.newup = (t_vect3f){up.x, up.y, up.z};
	normalize(&qm.newup);
	rotate_vect(&qm.newup, qm.first);
	normalize(&qm.newup);
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}
