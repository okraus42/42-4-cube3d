/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:21:25 by plouda            #+#    #+#             */
/*   Updated: 2024/01/08 14:59:25 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

/* void	set_object_vects(t_camera *camera)
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
} */

void	o_move_up(t_camera *camera, t_plane *plane)
{
	plane->coords[X] += (camera->up->x * 1);
	plane->coords[Y] += (camera->up->y * 1);
	plane->coords[Z] += (camera->up->z * 1);
	//printf("%f %f %f\n", sphere->coords[X], sphere->coords[Y], sphere->coords[Z]);
}

void	rotate_objects(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.modifier && keydata.key == MLX_KEY_UP && keydata.action != MLX_RELEASE)
		o_move_up(master->rt->camera, master->rt->planes[0]);
	// if (keydata.modifier && keydata.key == MLX_KEY_ && keydata.action != MLX_RELEASE)
	// 	o_move_up(master->rt->camera, master->rt->planes[0]);
	find_rays(master);
}