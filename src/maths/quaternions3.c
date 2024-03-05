/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:43:26 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:43:59 by plouda           ###   ########.fr       */
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
	rotate_vect(&qm.up, cone->q);
	normalize(&qm.up);
	qm.z = (t_vect3f){0., 0., 1.};
	qm.first = get_rotvect_quat(norm, qm.z);
	qm.y = (t_vect3f){0., 1., 0.};
	rotate_vect(&qm.up, qm.first);
	qm.up.z = 0;
	normalize(&qm.up);
	qm.newup = (t_vect3f){qm.up.x, qm.up.y, qm.up.z};
	qm.second = get_rotvect_quat(qm.newup, qm.y);
	qm.q = mult_quat(qm.first, qm.second);
	normalize_quat(&qm.q);
	return (qm.q);
}
