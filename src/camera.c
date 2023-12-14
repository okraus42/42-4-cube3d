/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:47:11 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 10:33:44 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

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
		up->z = -1;
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		right->x = 1;
		right->y = 0;
		right->z = 0;
		up->x = 0;
		up->y = 0;
		up->z = 1;
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right, right->x, right->y, right->z);
		*up = cross_product(*forward, *right);
		normalize(up, up->x, up->y, up->z);
	}
}

void	display_camera_matrix(t_camera *camera)
{
	double		**matrix;

	matrix = camera->matrix;
	printf("WorldToCamera:\nR: %.5f %.5f %.5f;\
	\nU: %.5f %.5f %.5f;\nF: %.5f %.5f %.5f;\nC: %.5f %.5f %.5f;\n",\
	matrix[0][0], matrix[0][1], matrix[0][2],\
	matrix[1][0], matrix[1][1], matrix[1][2],\
	matrix[2][0], matrix[2][1], matrix[2][2],\
	matrix[3][0], matrix[3][1], matrix[3][2]);
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
	display_camera_matrix(camera);
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
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*camera->right, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*camera->normal);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->normal->x = res2.q1;
	camera->normal->y = res2.q2;
	camera->normal->z = res2.q3;
	point = get_point_quat(*camera->up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->up->x = res2.q1;
	camera->up->y = res2.q2;
	camera->up->z = res2.q3;
}
// rotation along up/y-axis
void	pan(t_camera *camera, double angle)
{	
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*camera->up, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*camera->normal);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->normal->x = res2.q1;
	camera->normal->y = res2.q2;
	camera->normal->z = res2.q3;
	point = get_point_quat(*camera->right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->right->x = res2.q1;
	camera->right->y = res2.q2;
	camera->right->z = res2.q3;
}

// rotation along forward/z-axis
void	cant(t_camera *camera, double angle)
{
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*camera->normal, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*camera->right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->right->x = res2.q1;
	camera->right->y = res2.q2;
	camera->right->z = res2.q3;
	point = get_point_quat(*camera->up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	camera->up->x = res2.q1;
	camera->up->y = res2.q2;
	camera->up->z = res2.q3;
}

/*
Rotating camera basically means rotating two of its defining axes
along the third. It is theoretically possible to rotate all three
along an arbitrary axis as well. All rotations are active, i.e. 
the given point (or axis) is rotated with regard to the camera 
coordinate system.
*/
void	rotate_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_A)
		pan(master->rt->camera, 5);
	else if (keydata.key == MLX_KEY_D)
		pan(master->rt->camera, -5);
	else if (keydata.key == MLX_KEY_W)
		tilt(master->rt->camera, 5);
	else if (keydata.key == MLX_KEY_S)
		tilt(master->rt->camera, -5);
	else if (keydata.key == MLX_KEY_Q)
		cant(master->rt->camera, 5);
	else if (keydata.key == MLX_KEY_E)
		cant(master->rt->camera, -5);
	normalize(master->rt->camera->normal, master->rt->camera->normal->x,\
	master->rt->camera->normal->y, master->rt->camera->normal->z);
	normalize(master->rt->camera->right, master->rt->camera->right->x,\
	master->rt->camera->right->y, master->rt->camera->right->z);
	normalize(master->rt->camera->up, master->rt->camera->up->x,\
	master->rt->camera->up->y, master->rt->camera->up->z);
	find_rays(master);
}


void	shift_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
	{
		master->rt->camera->coords[X] += (master->rt->camera->right->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->right->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->right->z * 5);
	}
	if (keydata.key == MLX_KEY_LEFT)
	{
		master->rt->camera->coords[X] -= (master->rt->camera->right->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->right->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->right->z * 5); 
	}
	if (keydata.key == MLX_KEY_UP)
	{
		master->rt->camera->coords[X] += (master->rt->camera->up->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->up->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->up->z * 5);
	}
	if (keydata.key == MLX_KEY_DOWN)
	{
		master->rt->camera->coords[X] -= (master->rt->camera->up->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->up->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->up->z * 5);
	}
	// maybe actually bind this to a superkey later
	if (keydata.key == MLX_KEY_PAGE_UP)
	{
		master->rt->camera->coords[X] -= (master->rt->camera->normal->x * 5);
		master->rt->camera->coords[Y] -= (master->rt->camera->normal->y * 5);
		master->rt->camera->coords[Z] -= (master->rt->camera->normal->z * 5);
	}
	if (keydata.key == MLX_KEY_PAGE_DOWN)
	{
		master->rt->camera->coords[X] += (master->rt->camera->normal->x * 5);
		master->rt->camera->coords[Y] += (master->rt->camera->normal->y * 5);
		master->rt->camera->coords[Z] += (master->rt->camera->normal->z * 5);
	}
	if (keydata.key == MLX_KEY_KP_ADD)
		master->rt->camera->fov += 5;
	if (keydata.key == MLX_KEY_KP_SUBTRACT)
		master->rt->camera->fov -= 5;
	if (master->rt->camera->fov <= 0)
		master->rt->camera->fov = 1;
	else if (master->rt->camera->fov >= 180)
		master->rt->camera->fov = 179;
	printf("FOV: %d\n", master->rt->camera->fov);
	find_rays(master);
}
