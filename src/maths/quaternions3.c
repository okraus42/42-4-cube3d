/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:43:26 by plouda            #+#    #+#             */
/*   Updated: 2024/03/13 16:56:56 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

//sphere
void	get_sphere_tan_quat2(t_vect3f norm, t_qmath *qm)
{
	qm->tmp = (t_vect3f){0, 1, 0};
	if (norm.x == 0. && norm.y == 1. && norm.z == 0.)
	{
		qm->right = (t_vect3f){1, 0, 0};
		qm->up = (t_vect3f){0, 0, -1};
	}
	else if (norm.x == 0. && norm.y == -1 && norm.z == 0.)
	{
		qm->right = (t_vect3f){1, 0, 0};
		qm->up = (t_vect3f){0, 0, 1};
	}
	else
	{
		qm->right = cross_product(qm->tmp, norm);
		normalize(&(qm->right));
		qm->up = cross_product(norm, qm->right);
		normalize(&(qm->up));
	}
}

t_quat	get_sphere_tan_quat(t_vect3f norm)
{
	t_qmath	qm;

	get_sphere_tan_quat2(norm, &qm);
	qm.z = (t_vect3f){0., 0., 1.};
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y = (t_vect3f){0., 1., 0.};
	rotate_vect(&qm.up, qm.first);
	normalize(&qm.up);
	qm.newup = (t_vect3f){qm.up.x, qm.up.y, qm.up.z};
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}

//cylinder
t_quat	get_cylinder_tan_quat(t_vect3f norm)
{
	t_qmath	qm;

	qm.up = (t_vect3f){0., 0., 1.};
	qm.z = (t_vect3f){0., 0., 1.};
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y = (t_vect3f){0., 1., 0.};
	rotate_vect(&qm.up, qm.first);
	normalize(&qm.up);
	qm.newup = (t_vect3f){qm.up.x, qm.up.y, qm.up.z};
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}

//cone
t_quat	get_cone_tan_quat(t_vect3f norm, t_cone *cone, t_vect3f	i)
{
	t_qmath	qm;

	qm.up.x = cone->pinnacle->coords[X] - i.x;
	qm.up.y = cone->pinnacle->coords[Y] - i.y;
	qm.up.z = cone->pinnacle->coords[Z] - i.z;
	normalize(&qm.up);
	printf("qm.up.1x %f qm.up.y %f qm.up.z %f\n", qm.up.x, qm.up.y, qm.up.z);
	rotate_vect(&qm.up, cone->q);
	printf("cone->q.q0 %f cone->q.q1 %f cone->q.q2 %f cone->q.q3 %f\n", cone->q.q0, cone->q.q1, cone->q.q2, cone->q.q3);
	normalize(&qm.up);
	printf("qm.up.2x %f qm.up.y %f qm.up.z %f\n", qm.up.x, qm.up.y, qm.up.z);
	qm.z = (t_vect3f){0., 0., 1.};
	printf("norm.x %f %f %f\n", norm.x, norm.y, norm.z);
	qm.first = get_rotvect_quat(norm, qm.z);
	printf("qm.first.q0 %f qm.first.q1 %f qm.first.q2 %f qm.first.q3 %f\n", qm.first.q0, qm.first.q1, qm.first.q2, qm.first.q3);
	qm.y = (t_vect3f){0., 1., 0.};
	rotate_vect(&qm.up, qm.first);
	qm.up.z = 0;
	normalize(&qm.up);
	qm.newup = (t_vect3f){qm.up.x, qm.up.y, qm.up.z};
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	printf("qm.second.q0 %f qm.second.q1 %f qm.second.q2 %f qm.second.q3 %f\n", qm.second.q0, qm.second.q1, qm.second.q2, qm.second.q3);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	printf("qm.q.q0 %f qm.q.q1 %f qm.q.q2 %f qm.q.q3 %f\n", qm.q.q0, qm.q.q1, qm.q.q2, qm.q.q3);
	return (qm.q);
}
