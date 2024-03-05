/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:47:11 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:12:30 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_camera(t_camera *camera)
{
	t_vect3f	tmp;
	t_vect33f	c;

	c.normal = camera->normal;
	c.right = camera->right;
	c.up = camera->up;
	tmp = (t_vect3f){0, 1, 0};
	if (c.normal->x == 0. && c.normal->y == 1. && c.normal->z == 0.)
	{
		*c.right = (t_vect3f){1, 0, 0};
		*c.up = (t_vect3f){0, 0, -1};
	}
	else if (c.normal->x == 0. && c.normal->y == -1 && c.normal->z == 0.)
	{
		*c.right = (t_vect3f){1, 0, 0};
		*c.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*c.right = cross_product(tmp, *c.normal);
		normalize(c.right);
		*c.up = cross_product(*c.normal, *c.right);
		normalize(c.up);
	}
}

void	display_camera_matrix(t_camera *camera)
{
	double		**matrix;

	matrix = camera->matrix;
	printf("WorldToCamera:\nR: %.2f %.2f %.2f;\
	\nU: %.2f %.2f %.2f;\nF: %.2f %.2f %.2f;\nC: %.2f %.2f %.2f;\n", \
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
	find_rays(master);
}
