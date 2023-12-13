/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:47:11 by plouda            #+#    #+#             */
/*   Updated: 2023/12/13 16:19:37 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

// axis = axis along which we're rotating
t_quat	get_rot_quat(t_vect3f axis, double angle)
{
	t_quat	rot;
	
	//camera->quat = malloc(sizeof(malloc));
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = axis.x * sin(rad(angle) / 2);
	rot.q2 = axis.y * sin(rad(angle) / 2);
	rot.q3 = axis.z * sin(rad(angle) / 2);
	return (rot);
}

// axis = point that we're rotating
t_quat	get_point_quat(t_vect3f axis)
{
	t_quat	point;

	point.q0 = 0;
	point.q1 = axis.x;
	point.q2 = axis.y;
	point.q3 = axis.z;
	return (point);
}

t_quat	get_inverse_quat(t_quat quat)
{
	t_quat	inverse;

	inverse.q0 = quat.q0;
	inverse.q1 = quat.q1 * -1;
	inverse.q2 = quat.q2 * -1;
	inverse.q3 = quat.q3 * -1;
	return (inverse);
}

void	set_camera(t_camera *camera)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = camera->normal;
	tmp.x = 0;
	tmp.y = 1;
	tmp.z = 0;
	forward->x = camera->normal->x;
	forward->y = camera->normal->y;
	forward->z = camera->normal->z;
	right = camera->right;
	up = camera->up;
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		right->x = 1;
		right->y = 0;
		right->z = 0;
		up->x = 0;
		up->y = 0;
		up->z = 1;
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		right->x = 1;
		right->y = 0;
		right->z = 0;
		up->x = 0;
		up->y = 0;
		up->z = -1;
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right, right->x, right->y, right->z);
		*up = cross_product(*forward, *right);
		normalize(up, up->x, up->y, up->z);
	}
}

void	update_camera_matrix(t_camera *camera)
{
	double		**matrix;
	t_vect3f	*forward;

	matrix = camera->matrix;
	forward = camera->normal;
	matrix[0][0] = camera->right->x;
	matrix[0][1] = camera->right->y;
	matrix[0][2] = camera->right->z;
	matrix[0][3] = 0;
	matrix[1][0] = camera->up->x;
	matrix[1][1] = camera->up->y;
	matrix[1][2] = camera->up->z;
	matrix[1][3] = 0;
	matrix[2][0] = forward->x;
	matrix[2][1] = forward->y;
	matrix[2][2] = forward->z;
	matrix[2][3] = 0;
	matrix[3][0] = camera->coords[X];
	matrix[3][1] = camera->coords[Y];
	matrix[3][2] = camera->coords[Z];
	matrix[3][3] = 1;
	printf("WorldToCamera:\nR: %.5f %.5f %.5f;\nU:%.5f %.5f %.5f;\nF:%.5f %.5f %.5f;\nC:%.5f %.5f %.5f;\n", \
	matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], \
	matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0], matrix[3][1], matrix[3][2]);
}

/* since origin is 0,0,0, it's just shifting it to "from" point from origin.
Proper matrix multiplication becomes relevant when:
1/ origin is not 0,0,0
2/ homogenous coordinate is not 1 (e.g. after projection matrix)
This could also be encoded in the matrix itself instead of 0,0,0,1
*/
t_vect3f	shift_origin(double **cam)
{
	t_vect3f	origin;

	origin.x = cam[3][0];
	origin.y = cam[3][1];
	origin.z = cam[3][2];
	return (origin);
}

void	change_ray_direction(double **cam, t_vect3f *direction, t_vect3f temp)
{
	direction->x = temp.x * cam[0][0] + temp.y * cam[1][0] + temp.z * cam[2][0];
	direction->y = temp.x * cam[0][1] + temp.y * cam[1][1] + temp.z * cam[2][1];
	direction->z = temp.x * cam[0][2] + temp.y * cam[1][2] + temp.z * cam[2][2];
	normalize(direction, direction->x, direction->y, direction->z);
}

// rotation along right/x-axis
// p' = q−1 pq
void	tilt(t_camera *camera, double angle)
{
	/*
	double	y;
	double	z;

	y = camera->normal->y;
	z = camera->normal->z;
	camera->normal->y = y * cos(rad(angle)) - z * sin(rad(angle));
	camera->normal->z = y * sin(rad(angle)) + z * cos(rad(angle));
	y = camera->up->y;
	z = camera->up->z;
	camera->up->y = y * cos(rad(angle)) - z * sin(rad(angle));
	camera->up->z = y * sin(rad(angle)) + z * cos(rad(angle));
	*/
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	point = get_point_quat(*camera->normal);
	rot = get_rot_quat(*camera->right, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->normal->x = res2.q1;
	camera->normal->y = res2.q2;
	camera->normal->z = res2.q3;

	point = get_point_quat(*camera->up);
	rot = get_rot_quat(*camera->right, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->up->x = res2.q1;
	camera->up->y = res2.q2;
	camera->up->z = res2.q3;
}
// rotation along up/y-axis
void	pan(t_camera *camera, double angle)
{
	/* double	x;
	double	z;

	x = camera->normal->x;
	z = camera->normal->z;
	camera->normal->z = z * cos(rad(angle)) - x * sin(rad(angle));
	camera->normal->x = z * sin(rad(angle)) + x * cos(rad(angle));
	x = camera->right->x;
	z = camera->right->z;
	camera->right->z = z * cos(rad(angle)) - x * sin(rad(angle));
	camera->right->x = z * sin(rad(angle)) + x * cos(rad(angle)); */
	
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	point = get_point_quat(*camera->normal);
	rot = get_rot_quat(*camera->up, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->normal->x = res2.q1;
	camera->normal->y = res2.q2;
	camera->normal->z = res2.q3;

	point = get_point_quat(*camera->right);
	rot = get_rot_quat(*camera->up, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->right->x = res2.q1;
	camera->right->y = res2.q2;
	camera->right->z = res2.q3;
}

// rotation along forward/z-axis
void	cant(t_camera *camera, double angle)
{
	/* double	x;
	double	y;

	x = camera->up->x;
	y = camera->up->y;
	camera->up->x = x * cos(rad(angle)) - y * sin(rad(angle));
	camera->up->y = x * sin(rad(angle)) + y * cos(rad(angle));
	x = camera->right->x;
	y = camera->right->y;
	camera->right->x = x * cos(rad(angle)) - y * sin(rad(angle));
	camera->right->y = x * sin(rad(angle)) + y * cos(rad(angle)); */
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	point = get_point_quat(*camera->right);
	rot = get_rot_quat(*camera->normal, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->right->x = res2.q1;
	camera->right->y = res2.q2;
	camera->right->z = res2.q3;

	point = get_point_quat(*camera->up);
	rot = get_rot_quat(*camera->normal, angle);
	inv = get_inverse_quat(rot);
	
	res1.q0 = inv.q0 * point.q0 - inv.q1 * point.q1 - inv.q2 * point.q2 - inv.q3 * point.q3;
	res1.q1 = inv.q0 * point.q1 + inv.q1 * point.q0 - inv.q2 * point.q3 + inv.q3 * point.q2;
	res1.q2 = inv.q0 * point.q2 + inv.q1 * point.q3 + inv.q2 * point.q0 - inv.q3 * point.q1;
	res1.q3 = inv.q0 * point.q3 - inv.q1 * point.q2 + inv.q2 * point.q1 + inv.q3 * point.q0;

	res2.q0 = res1.q0 * rot.q0 - res1.q1 * rot.q1 - res1.q2 * rot.q2 - res1.q3 * rot.q3;
	res2.q1 = res1.q0 * rot.q1 + res1.q1 * rot.q0 - res1.q2 * rot.q3 + res1.q3 * rot.q2;
	res2.q2 = res1.q0 * rot.q2 + res1.q1 * rot.q3 + res1.q2 * rot.q0 - res1.q3 * rot.q1;
	res2.q3 = res1.q0 * rot.q3 - res1.q1 * rot.q2 + res1.q2 * rot.q1 + res1.q3 * rot.q0;

	camera->up->x = res2.q1;
	camera->up->y = res2.q2;
	camera->up->z = res2.q3;
}

void	rotate_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_A)
		pan(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_D)
		pan(master->rt->camera, -5);
	if (keydata.key == MLX_KEY_W)
		tilt(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_S)
		tilt(master->rt->camera, -5);
	if (keydata.key == MLX_KEY_Q)
		cant(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_E)
		cant(master->rt->camera, -5);
	normalize(master->rt->camera->normal, master->rt->camera->normal->x, master->rt->camera->normal->y, \
	master->rt->camera->normal->z);
	normalize(master->rt->camera->right, master->rt->camera->right->x, master->rt->camera->right->y, \
	master->rt->camera->right->z);
	normalize(master->rt->camera->up, master->rt->camera->up->x, master->rt->camera->up->y, \
	master->rt->camera->up->z);
	find_rays(master);
}

void	shift_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
	{
		//master->rt->camera->coords[X] += 5;
		master->rt->camera->coords[X] += (master->rt->camera->right->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->right->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->right->z * 5);
	}
	if (keydata.key == MLX_KEY_LEFT)
	{
		//master->rt->camera->coords[X] -= 5;
		master->rt->camera->coords[X] -= (master->rt->camera->right->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->right->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->right->z * 5); 
	}
	if (keydata.key == MLX_KEY_UP)
	{
		//master->rt->camera->coords[Y] += 5;
		master->rt->camera->coords[X] += (master->rt->camera->up->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->up->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->up->z * 5);
	}
	if (keydata.key == MLX_KEY_DOWN)
	{
		//master->rt->camera->coords[Y] -= 5;
		master->rt->camera->coords[X] -= (master->rt->camera->up->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->up->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->up->z * 5);
	}
	// maybe actually bind this to a superkey later
	if (keydata.key == MLX_KEY_PAGE_UP)
	{
		//master->rt->camera->coords[Z] -= 5;
		master->rt->camera->coords[X] -= (master->rt->camera->normal->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->normal->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->normal->z * 5);
	}
	if (keydata.key == MLX_KEY_PAGE_DOWN)
	{
		//master->rt->camera->coords[Z] += 5;
		master->rt->camera->coords[X] += (master->rt->camera->normal->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->normal->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->normal->z * 5);
	}
	if (keydata.key == MLX_KEY_KP_ADD)
		master->rt->camera->fov += 5;
	if (keydata.key == MLX_KEY_KP_SUBTRACT)
		master->rt->camera->fov -= 5;
	find_rays(master);
}
