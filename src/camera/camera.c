/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:47:11 by plouda            #+#    #+#             */
/*   Updated: 2024/01/16 09:12:13 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_camera(t_camera *camera)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = camera->normal;
	right = camera->right;
	up = camera->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	display_camera_matrix(t_camera *camera)
{
	double		**matrix;

	matrix = camera->matrix;
	printf("WorldToCamera:\nR: %.5f %.5f %.5f;\
	\nU: %.5f %.5f %.5f;\nF: %.5f %.5f %.5f;\nC: %.5f %.5f %.5f;\n", \
	matrix[0][0], matrix[0][1], matrix[0][2], \
	matrix[1][0], matrix[1][1], matrix[1][2], \
	matrix[2][0], matrix[2][1], matrix[2][2], \
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
	normalize(direction);
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
	t_camera	*camera;

	camera = master->rt->camera;
	rotate(keydata.key, camera->normal, camera->right, camera->up);
	/* normalize(camera->normal);
	normalize(camera->right);
	normalize(camera->up); */
	master->rt->camera = camera;
	find_rays(master);
}

void	shift_camera(t_master *master, mlx_key_data_t keydata)
{
	move(keydata.key, master->rt->camera, master->rt->camera->coords);
	if (keydata.key == MLX_KEY_MINUS)
		master->rt->camera->fov -= 5;
	if (keydata.key == MLX_KEY_EQUAL)
		master->rt->camera->fov += 5;
	clamp(1, 179, &master->rt->camera->fov);
	printf("FOV: %d\n", master->rt->camera->fov);
	find_rays(master);
}
