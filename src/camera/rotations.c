/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:07:57 by plouda            #+#    #+#             */
/*   Updated: 2024/01/10 15:01:17 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	rotate(keys_t key, t_vect3f *forward, t_vect3f *right, t_vect3f *up)
{
	if (key == MLX_KEY_A)
		pan(forward, right, up, 5);
	else if (key == MLX_KEY_D)
		pan(forward, right, up, -5);
	else if (key == MLX_KEY_W)
		tilt(forward, right, up, 5);
	else if (key == MLX_KEY_S)
		tilt(forward, right, up, -5);
	else if (key == MLX_KEY_Q)
		cant(forward, right, up, 5);
	else if (key == MLX_KEY_E)
		cant(forward, right, up, -5);
	normalize(forward);
	normalize(right);
	normalize(up);
}

void	tilt(t_vect3f *forward, t_vect3f *right, t_vect3f *up, double angle)
{
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*right, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*forward);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	forward->x = res2.q1;
	forward->y = res2.q2;
	forward->z = res2.q3;
	point = get_point_quat(*up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	up->x = res2.q1;
	up->y = res2.q2;
	up->z = res2.q3;
}

// rotation along up/y-axis
void	pan(t_vect3f *forward, t_vect3f *right, t_vect3f *up, double angle)
{	
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*up, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*forward);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	forward->x = res2.q1;
	forward->y = res2.q2;
	forward->z = res2.q3;
	point = get_point_quat(*right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	right->x = res2.q1;
	right->y = res2.q2;
	right->z = res2.q3;
}

// rotation along forward/z-axis
void	cant(t_vect3f *forward, t_vect3f *right, t_vect3f *up, double angle)
{
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*forward, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	right->x = res2.q1;
	right->y = res2.q2;
	right->z = res2.q3;
	point = get_point_quat(*up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	up->x = res2.q1;
	up->y = res2.q2;
	up->z = res2.q3;
}

