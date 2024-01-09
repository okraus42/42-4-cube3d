/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:21:25 by plouda            #+#    #+#             */
/*   Updated: 2024/01/09 11:27:54 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_plane_vects(t_plane *plane)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = plane->normal;
	right = plane->right;
	up = plane->up;
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

void	tilt_plane(t_plane *plane, double angle)
{
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*plane->right, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*plane->normal);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->normal->x = res2.q1;
	plane->normal->y = res2.q2;
	plane->normal->z = res2.q3;
	point = get_point_quat(*plane->up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->up->x = res2.q1;
	plane->up->y = res2.q2;
	plane->up->z = res2.q3;
}

void	pan_plane(t_plane *plane, double angle)
{	
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*plane->up, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*plane->normal);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->normal->x = res2.q1;
	plane->normal->y = res2.q2;
	plane->normal->z = res2.q3;
	point = get_point_quat(*plane->right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->right->x = res2.q1;
	plane->right->y = res2.q2;
	plane->right->z = res2.q3;
}

void	cant_plane(t_plane *plane, double angle)
{
	t_quat	res1;
	t_quat	res2;
	t_quat	point;
	t_quat	inv;
	t_quat	rot;

	rot = get_rot_quat(*plane->normal, angle);
	inv = get_inverse_quat(rot);
	point = get_point_quat(*plane->right);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->right->x = res2.q1;
	plane->right->y = res2.q2;
	plane->right->z = res2.q3;
	point = get_point_quat(*plane->up);
	res1 = mult_quat(inv, point);
	res2 = mult_quat(res1, rot);
	plane->up->x = res2.q1;
	plane->up->y = res2.q2;
	plane->up->z = res2.q3;
}


void	set_cylinder_vects(t_cylinder *cylinder)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = cylinder->normal;
	right = cylinder->right;
	up = cylinder->up;
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

void	set_disc_vects(t_disc *disc)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = disc->normal;
	right = disc->right;
	up = disc->up;
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
	else if (keydata.modifier && keydata.key == MLX_KEY_Q && keydata.action != MLX_RELEASE)
	{
		cant_plane(master->rt->planes[0], 5);
		normalize(master->rt->planes[0]->normal);
		normalize(master->rt->planes[0]->right);
		normalize(master->rt->planes[0]->up);
	}
	else if (keydata.modifier && keydata.key == MLX_KEY_A && keydata.action != MLX_RELEASE)
	{
		/* printf("Object vectors:\nNormal: %.5f %.5f %.5f;\
			\nRight: %.5f %.5f %.5f;\nUp: %.5f %.5f %.5f;\n", \
			master->rt->planes[0]->normal->x, master->rt->planes[0]->normal->y, master->rt->planes[0]->normal->z, \
			master->rt->planes[0]->right->x, master->rt->planes[0]->right->y, master->rt->planes[0]->right->z, \
			master->rt->planes[0]->up->x, master->rt->planes[0]->up->y, master->rt->planes[0]->up->z); */
		pan_plane(master->rt->planes[0], 5);
		normalize(master->rt->planes[0]->normal);
		normalize(master->rt->planes[0]->right);
		normalize(master->rt->planes[0]->up);
	}
	else if (keydata.modifier && keydata.key == MLX_KEY_S && keydata.action != MLX_RELEASE)
	{
		tilt_plane(master->rt->planes[0], 5);
		normalize(master->rt->planes[0]->normal);
		normalize(master->rt->planes[0]->right);
		normalize(master->rt->planes[0]->up);
	}
	find_rays(master);
}