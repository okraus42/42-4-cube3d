/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion_rotations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:21:43 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:22:14 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

//quaternion 
void	qtilt(t_quat *q, double angle)
{
	t_quat	rot;
	t_quat	inv;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
		*q = (t_quat){0.9999, 0.01, 0., 0.};
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 1 * sin(rad(angle) / 2);
	rot.q2 = 0;
	rot.q3 = 0;
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
		*q = (t_quat){0., 1., 0., 0.};
}

// rotation along up/y-axis
void	qpan(t_quat *q, double angle)
{
	t_quat	rot;
	t_quat	inv;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
		*q = (t_quat){0.9999, 0., 0.01, 0.};
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 0;
	rot.q2 = 1 * sin(rad(angle) / 2);
	rot.q3 = 0;
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
		*q = (t_quat){0., 0., 1., 0.};
}

// rotation along forward/z-axis
void	qcant(t_quat *q, double angle)
{
	t_quat	rot;
	t_quat	inv;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
	{
		*q = (t_quat){0.9999, 0., 0., 0.01};
	}
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 0;
	rot.q2 = 0;
	rot.q3 = 1 * sin(rad(angle) / 2);
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
		*q = (t_quat){0., 0., 0., 1.};
}

void	qrotate_o(keys_t key, t_quat *q, t_camera *camera)
{
	(void)camera->coords;
	if (key == MLX_KEY_A)
		qpan(q, 5);
	else if (key == MLX_KEY_D)
		qpan(q, -5);
	else if (key == MLX_KEY_W)
		qtilt(q, 5);
	else if (key == MLX_KEY_S)
		qtilt(q, -5);
	else if (key == MLX_KEY_Q)
		qcant(q, 5);
	else if (key == MLX_KEY_E)
		qcant(q, -5);
	normalize_quat(q);
}
